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
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.opengl.GLES20;
import android.opengl.GLSurfaceView;
import android.opengl.GLUtils;

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
    private int afPosition;


    private final float[] vertexData = {

            -1f, -1f,
            1f, -1f,
            -1f, 1f,
            1f, 1f
    };

    /***
     * 纹理坐标点(要与顶点一一对应)
     */
    private final float[] textureData = {
//            0f, 1f,
//            1f, 1f,
//            0f, 0f,
//            1f, 0f


            //倒放
            1f, 0f,
            0f, 0f,
            1f, 1f,
            0f, 1f
    };


    private FloatBuffer vertexBuffer;
    private FloatBuffer textureBuffer;
    private int textureid;


    public YCRender(Context context) {
        this.mContext = context;
        vertexBuffer = ByteBuffer.allocateDirect(vertexData.length * 4)
                .order(ByteOrder.nativeOrder())
                .asFloatBuffer()
                .put(vertexData);
        vertexBuffer.position(0);

        textureBuffer = ByteBuffer.allocateDirect(textureData.length * 4)
                .order(ByteOrder.nativeOrder())
                .asFloatBuffer()
                .put(textureData);
        textureBuffer.position(0);

    }


    @Override
    public void onSurfaceCreated(GL10 gl10, EGLConfig eglConfig) {
        String vertexSource = YCShaderUtil.readRawTxt(mContext, R.raw.vertex_shader);
        String fragmentSource = YCShaderUtil.readRawTxt(mContext, R.raw.fragment_shader);
        mProgram = YCShaderUtil.createProgram(vertexSource, fragmentSource);
        if (mProgram > 0) {
            avPosition = GLES20.glGetAttribLocation(mProgram, "av_Position");//顶点
            afPosition = GLES20.glGetAttribLocation(mProgram, "af_Position");//纹理


            //创建纹理
            int[] textureIds = new int[1];
            GLES20.glGenTextures(1, textureIds, 0);
            if (textureIds[0] == 0) {
                return;
            }

            //绑定纹理
            textureid = textureIds[0];
            GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, textureid);

            GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_WRAP_S, GLES20.GL_REPEAT);
            GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_WRAP_T, GLES20.GL_REPEAT);
            GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MIN_FILTER, GLES20.GL_LINEAR);
            GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MAG_FILTER, GLES20.GL_LINEAR);

            Bitmap bitmap = BitmapFactory.decodeResource(mContext.getResources(), R.mipmap.og);
            if (bitmap == null) {
                return;
            }
            GLUtils.texImage2D(GLES20.GL_TEXTURE_2D, 0, bitmap, 0);
            bitmap.recycle();
            bitmap = null;

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
        GLES20.glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

        GLES20.glUseProgram(mProgram);


        GLES20.glEnableVertexAttribArray(avPosition);
        GLES20.glVertexAttribPointer(avPosition, 2, GLES20.GL_FLOAT, false, 8, vertexBuffer);

        GLES20.glEnableVertexAttribArray(afPosition);
        GLES20.glVertexAttribPointer(afPosition, 2, GLES20.GL_FLOAT, false, 8, textureBuffer);


        GLES20.glDrawArrays(GLES20.GL_TRIANGLE_STRIP, 0, 4);//两个三角形，6个顶点

    }
}