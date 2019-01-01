package com.bosma.openglesdemo;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;

public class YCGLSurfaceView extends GLSurfaceView {


    public YCGLSurfaceView(Context context) {
        super(context);
    }

    public YCGLSurfaceView(Context context, AttributeSet attrs) {
        super(context, attrs);
        setEGLContextClientVersion(2);
        setRenderer(new YCRender());

    }
}
