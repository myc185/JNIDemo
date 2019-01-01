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

import android.content.Context;
import android.opengl.GLES20;
import android.opengl.GLSurfaceView;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

/**
 * @ClassName: YCRender
 * @Description: 作用描述
 * @Author: Administrator
 * @Date: 2019/1/1 10:16
 */
public class YCRender implements GLSurfaceView.Renderer {


    private Context mContext;
    private int mProgram;
    private int avPosition;

    private final float[] vertexData = {
            -1f, 0f,
            0f, 1f,
            1f, 0f
    };

    private FloatBuffer vertexBuffer;
    private int afColor;

    public YCRender(Context context) {
        this.mContext = context;
        vertexBuffer = ByteBuffer.allocateDirect(vertexData.length * 4)
                .order(ByteOrder.nativeOrder())
                .asFloatBuffer()
                .put(vertexData);
        vertexBuffer.position(0);
    }


    @Override
    public void onSurfaceCreated(GL10 gl10, EGLConfig eglConfig) {
        String vertexSource= YCShaderUtil.readRawTxt(mContext, R.raw.vertex_shader);
        String fragmentSource= YCShaderUtil.readRawTxt(mContext, R.raw.fragment_shader);
        mProgram = YCShaderUtil.createProgram(vertexSource, fragmentSource);
        if(mProgram > 0)
        {
            avPosition = GLES20.glGetAttribLocation(mProgram, "av_Position");
            afColor = GLES20.glGetUniformLocation(mProgram, "af_Color");
        }

    }

    @Override
    public void onSurfaceChanged(GL10 gl10, int with, int height) {
        GLES20.glViewport(0, 0, with, height);

    }

    @Override
    public void onDrawFrame(GL10 gl10) {

        GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT);
        //用什么颜色清屏，显示的时候就是什么背景的颜色
//        GLES20.glClearColor(1.0f, 0.0f, 0.0f, 1.0f);

        GLES20.glUseProgram(mProgram);
        GLES20.glUniform4f(afColor, 1f, 0f, 0f, 1f);
        GLES20.glEnableVertexAttribArray(avPosition);
        GLES20.glVertexAttribPointer(avPosition, 2, GLES20.GL_FLOAT, false, 8, vertexBuffer);
        GLES20.glDrawArrays(GLES20.GL_TRIANGLES, 0, 3);

    }
}