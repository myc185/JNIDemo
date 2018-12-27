//
// Created by 莫运川 on 2018/12/27.
//

#include "jni.h"

#ifndef JNITHREAD_JAVALISTENER_H
#define JNITHREAD_JAVALISTENER_H


class JavaListener {

public:

    /***
     * 用于子线程调用
     */
    JavaVM *javaVM;

    _JNIEnv *jniEnv;

    jobject jobj;

    jmethodID jmid;

public:
    JavaListener(JavaVM *vm, _JNIEnv *env, jobject job);

    ~JavaListener();

    /***
     * threadType 1: 主线程
     * threadType 0: 子线程
     */
    void onError(int threadType, int code, const char *msg);

};


#endif //JNITHREAD_JAVALISTENER_H
