package com.bosma.jnithread;

import android.util.Log;

public class ThreadDemo {

    public static final String TAG = ThreadDemo.class.getSimpleName();

    static {
        System.loadLibrary("native-lib");
    }


    private OnErrorListener onErrorListener;

    public void setOnErrorListener(OnErrorListener onErrorListener) {
        this.onErrorListener = onErrorListener;
    }

    public native void normalThread();

    public native void mutexThread();

    public native void stopMutexThread();

    public native void callBackFromC();

    public void onError(int code, String msg) {
        Log.i(TAG, "C ++ Call onError");
        if (onErrorListener != null) {
            onErrorListener.onError(code, msg);
        }

    }

    public interface OnErrorListener {
        public void onError(int code, String msg);
    }




}
