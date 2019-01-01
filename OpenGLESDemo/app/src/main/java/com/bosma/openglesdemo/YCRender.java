/**
 * Copyright (C), 2015-2019, (Bosma)博冠智能技术有限公司
 * FileName: YCRender
 * Author: Administrator
 * Date: 2019/1/1 10:16
 * Description: 渲染器
 * History:
 * <author> <time> <version> <desc>
 * 作者姓名 修改时间 版本号 描述
 */
package com.bosma.openglesdemo;

import android.opengl.GLES20;
import android.opengl.GLSurfaceView;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

/**
 * @ClassName: YCRender
 * @Description: 作用描述
 * @Author: Administrator
 * @Date: 2019/1/1 10:16
 */
public class YCRender implements GLSurfaceView.Renderer {


    @Override
    public void onSurfaceCreated(GL10 gl10, EGLConfig eglConfig) {

    }

    @Override
    public void onSurfaceChanged(GL10 gl10, int with, int height) {
        GLES20.glViewport(0, 0, with, height);

    }

    @Override
    public void onDrawFrame(GL10 gl10) {

        GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT);
        //用什么颜色清屏，显示的时候就是什么背景的颜色
        GLES20.glClearColor(1.0f, 0.0f, 0.0f, 1.0f);


    }
}