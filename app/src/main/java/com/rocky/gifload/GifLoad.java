package com.rocky.gifload;

import android.graphics.Bitmap;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.widget.ImageView;

import androidx.annotation.NonNull;

import java.lang.ref.WeakReference;


/**
 * <pre>
 *     author : rocky
 *     time   : 2022/03/07
 * </pre>
 */
public class GifLoad {
    static {
        //引入资源lib
        System.loadLibrary("gifload");
    }

    private static long gifHandler;
    private static Bitmap bitmap;
    private static GifLoad gifLoad;
    private Bitmap.Config config = Bitmap.Config.ARGB_8888;
    private Handler handler = new Handler(Looper.getMainLooper()) {
        @Override
        public void handleMessage(@NonNull Message msg) {
            if (msg.what == 1) {
                WeakReference<ImageView> reference = (WeakReference<ImageView>) msg.obj;
                ImageView imageView = reference.get();
                if (imageView != null) {
                    int delay = updateGiftCurrentFrame();
                    Message message = Message.obtain();
                    message.what = 1;
                    message.obj = reference;
                    handler.sendMessageDelayed(message, delay);
                    imageView.setImageBitmap(bitmap);
                }
            }
        }
    };


    private GifLoad(long gifHandler) {
        //这里全局保存 native对应的内存地址
        GifLoad.gifHandler = gifHandler;
    }

    public static GifLoad load(String path) {
        long gifHandler = loadGif(path);
        if (gifHandler == 0) {
            throw new IllegalArgumentException("文件打开失败 请检查路径");
        }

        gifLoad = new GifLoad(gifHandler);

        return gifLoad;
    }

    public static int getWith() {
        return getWidth(gifHandler);
    }

    public static int getHeight() {
        return getHeight(gifHandler);
    }

    public GifLoad config(Bitmap.Config config) {
        this.config = config;
        return gifLoad;
    }


    public void into(ImageView img) {
        if (bitmap == null) {
            bitmap = Bitmap.createBitmap(getWith(), getHeight(), config);
        }

        img.setImageBitmap(bitmap);
        int delay = updateGiftCurrentFrame();
        //绑定handler delay刷新当前帧
        Message message = Message.obtain();
        message.what = 1;
        message.obj = new WeakReference<>(img);
        handler.sendMessageDelayed(message, delay);

    }

    private int updateGiftCurrentFrame() {
        return updateFrame(gifHandler, bitmap);
    }


    //*******************************native方法 start **************************
    //把文件路径传递给native
    //返回值就是native数据缓冲区的地址
    private static native long loadGif(String _path);

    //获取gif的宽高 参数为地址
    private static native int getWidth(long gifHandler);

    private static native int getHeight(long gifHandler);

    //更新gif
    private native int updateFrame(long gifHandler, Bitmap bitmap);


    //*******************************native方法  end **************************


}
