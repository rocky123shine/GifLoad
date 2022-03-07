#include <jni.h>
#include <string>
#include <android/bitmap.h>

extern "C" {
//引入c的资源
#include "gif_lib.h"
}
#define  argb(a, r, g, b) ( ((a) & 0xff) << 24 ) | ( ((b) & 0xff) << 16 ) | ( ((g) & 0xff) << 8 ) | ((r) & 0xff)

struct GifBean {
    int current_frame;//GIF 播放的当前帧的索引
    int total_frame;//gif的帧数
    int *delays;//每一帧播放的时间间隔
};

void drawFrame(GifFileType *gifFileType, AndroidBitmapInfo info, void *pixels);

/**
 *
 * @param gif_handler  内存地址
 * @param info  bitmap 的信息
 * @param pixels  图片像素数组
 */
void drawFrame(GifFileType *gifFileType, AndroidBitmapInfo info, void *pixels) {
    //获取当前帧
    GifBean *gifBean = static_cast<GifBean *>(gifFileType->UserData);
    int currentIndex = gifBean->current_frame;
    SavedImage savedImage = gifFileType->SavedImages[currentIndex];
    //图像描述信息
    GifImageDesc frameInfo = savedImage.ImageDesc;

    //颜色表
    ColorMapObject *colorMap = frameInfo.ColorMap;
    int *px = (int *) pixels;//这是创建的空的bitmap相当与一个白板 后面得到的gif每一帧所对应的像素 放入进去 即可显示图像
    int *line;//保存当前行的临时索引
    //    索引
    int pointPixel;
    GifByteType gifByteType;
    GifColorType gifColorType;
    //遍历像素 赋值给pixels
    for (int y = frameInfo.Top; y < frameInfo.Top + frameInfo.Height; ++y) {
        //首次进来
        line = px;
        for (int x = frameInfo.Left; x < frameInfo.Left + frameInfo.Width; ++x) {
            // 当前行数据对应赋值
            //当前 像素的索引为
            pointPixel = (y - frameInfo.Top) * frameInfo.Width + x - frameInfo.Left;
            //获取颜色表对应的索引
            gifByteType = savedImage.RasterBits[pointPixel];
            //到颜色表获取颜色
            gifColorType = colorMap->Colors[gifByteType];
            //gifColorType: 每一个rgb值 都是 一个char
            //            gifColorType.Red;
            //            gifColorType.Green
            //            gifColorType.Blue
            //注意1.gif中没有透明通道  咱们再次默认为不透明
            //注意2. 数组中的argb 的顺序和实际显示的顺序 agbr
            //使用 argb 组合成为当前索引的颜色值
            line[x] = argb(255, gifColorType.Red, gifColorType.Green, gifColorType.Blue);
        }
        //更新到下一行
        px = reinterpret_cast<int *>((char *) px + info.stride);

    }

}

extern "C"
JNIEXPORT jint JNICALL
Java_com_rocky_gifload_GifLoad_getWidth(JNIEnv *env, jclass clazz, jlong gif_handler) {
    GifFileType *type = reinterpret_cast<GifFileType *>(gif_handler);
    return type->SWidth;
}
extern "C"
JNIEXPORT jint JNICALL
Java_com_rocky_gifload_GifLoad_getHeight(JNIEnv *env, jclass clazz, jlong gif_handler) {
    GifFileType *type = reinterpret_cast<GifFileType *>(gif_handler);
    return type->SHeight;
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_rocky_gifload_GifLoad_updateFrame(JNIEnv *env, jobject thiz, jlong gif_handler,
                                           jobject bitmap) {
    //通过gif_handler 拿到 GifFileType
    GifFileType *type = reinterpret_cast<GifFileType *>(gif_handler);
    //获取 图片信息
    // 设置参数
    AndroidBitmapInfo info;
    AndroidBitmap_getInfo(env, bitmap, &info);
    //获取像素数据  操作bitmap 也是 对这个数组操作
    int *pixels;
    //对bitmap 操作 需要加锁
    AndroidBitmap_lockPixels(env, bitmap, reinterpret_cast<void **>(&pixels));
    //绘制bitmap
    drawFrame(type, info, pixels);
    AndroidBitmap_unlockPixels(env, bitmap);
    //更新gifbean
    GifBean *gifBean = static_cast<GifBean *>(type->UserData);
    gifBean->current_frame++;
    if (gifBean->current_frame >= gifBean->total_frame - 1) {
        gifBean->current_frame = 0;
    }
    return 100;
}

extern "C"
JNIEXPORT jlong JNICALL
Java_com_rocky_gifload_GifLoad_loadGif(JNIEnv *env, jclass clazz, jstring _path) {
    //jsting  转换成c的字符串
    const char *path = env->GetStringUTFChars(_path, NULL);
    //使用gif原生库 打开gif文件
    //传入入参 出参
    int Error;
    //返回值就是native 对应的内存的地址
    GifFileType *gifFileType = DGifOpenFileName(path, &Error);
    //if (Error == 0) {//打开成功
        //初始化缓冲区  数组 SaveImages
        DGifSlurp(gifFileType);
        //这个参数 类似于message中的obj 可以携带任何数据类型
        //声明一个实体类  保存gif的参数
        //创建GIFBean
        GifBean *gifBean = static_cast<GifBean *>(malloc(sizeof(GifBean)));
        //清空内存的数据 以免错乱
        memset(gifBean, 0, sizeof(GifBean));
        gifFileType->UserData = gifBean;
        gifBean->current_frame = 0;
        gifBean->total_frame = gifFileType->ImageCount;
   // }

    env->ReleaseStringUTFChars(_path, path);

    return (jlong)gifFileType;
}
