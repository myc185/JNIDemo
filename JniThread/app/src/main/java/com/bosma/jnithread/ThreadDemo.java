package com.bosma.jnithread;

public class ThreadDemo {


    static {
        System.loadLibrary("native-lib");
    }



    public native void normalThread();


    public native void mutexThread();

}
