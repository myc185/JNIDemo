//
// Created by 莫运川 on 2018/12/27.
//

#include "JavaListener.h"
#include "AndroidLog.h"


JavaListener::~JavaListener() {

}

JavaListener::JavaListener(JavaVM *vm, _JNIEnv *env, jobject job) {

    javaVM = vm;
    jniEnv = env;
    jobj = job;

    jclass clz = env->GetObjectClass(job);
    if (!clz) {
        return;
    }
    jmid = env->GetMethodID(clz, "onError", "(ILjava/lang/String;)V");

}

void JavaListener::onError(int threadType, int code, const char *msg) {

    if (threadType == 0) {
        LOGD("onError : 子线程调用")
        JNIEnv *env;
        javaVM->AttachCurrentThread(&env, 0);

        jstring jmsg = env->NewStringUTF(msg);
        env->CallVoidMethod(jobj, jmid, code, jmsg);
        env->DeleteLocalRef(jmsg);

        javaVM->DetachCurrentThread();

    } else if (threadType == 1) {
        LOGD("onError : 主线程调用")
        jstring jmsg = jniEnv->NewStringUTF(msg);
        jniEnv->CallVoidMethod(jobj, jmid, code, jmsg);
        jniEnv->DeleteLocalRef(jmsg);

    }

}
