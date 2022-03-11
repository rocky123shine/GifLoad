## Gif 原生加载

这里是学习使用NDK技术 实现Gif的加载

- 去源码找到gif的库 直接引入 我使用的是Android 8 文件目录 /external/giflib 全部copy 到cpp 其中： dgif 是解码gif工具 egif 是编码gif的工具


- 创建GifLoad
    - System.loadLibrary("gifload"); 引入lib资源
    - 创建native方法
        1. loadGif 使用 gif_lib.h

            - 获取内存地址
            - 使用入参 保存gif信息

        2. getWith/getHeight 获取宽高
        3. 使用链式方式 对外暴露方法
    - 更新显示gif的下一帧

      这里是gif加载的核心

        1. 得到native的地址 作为操作的入手点

        2. 使用系统库 jnigraphics 中的 AndroidBitmap_getInfo函数 获取图片信息

        3. 绘制当前帧 即 把当前帧的颜色信息 填充到native内存对应的坐标中 然后即可显示

        4. 更新帧的索引 注意 越界的问题

        5. 使用 handler 循环播放gif的每一帧

- 使用

      ```

         Bitmap.Config config = Bitmap.Config.ARGB_8888;
         File file=new File(Environment.getExternalStorageDirectory(),"demo.gif");

         GifLoad.load(file.getAbsolutePath())
                .config(config)
                .into(binding.image);

      ```

## libjpeg-turbo

    这个库是 Skia渲染引擎的基础 
    现在原生系统都是使用的skia渲染的  在7.0之前是默认关闭的哈夫曼压缩的 使用的是质量压缩
    本次目的是直接跳过skia 使用jpeg库 开启哈夫曼压缩 
