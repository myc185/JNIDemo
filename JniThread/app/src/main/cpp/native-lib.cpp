#include <jni.h>
#include <string>

#include "pthread.h"
#include "AndroidLog.h"

#include "queue"
#include "unistd.h"
#include "JavaListener.h"

pthread_t thread;

void *normalCallBack(void *data) {
    LOGD("create normal thread from C++");
    pthread_exit(&thread);

}

extern "C"
JNIEXPORT void JNICALL
Java_com_bosma_jnithread_ThreadDemo_normalThread(JNIEnv *env, jobject instance) {

    pthread_create(&thread, NULL, normalCallBack, NULL);

}


pthread_t product;//生产者
pthread_t consumer;//消费者
pthread_mutex_t mutex;//线程锁
pthread_cond_t cound;//

bool isMutex = false;

std::queue<int> queue;

/**
 * 生产者回调
 */
void *productCallBack(void *data) {
    while (isMutex) {
        pthread_mutex_lock(&mutex);
        queue.push(1);
        LOGD("生产者生产产品，通知消费者消费，产品数量为 ： %d", queue.size());
        pthread_cond_signal(&cound);
        pthread_mutex_unlock(&mutex);
        sleep(5);
    }
    LOGD("退出生产者线程...");
    pthread_exit(&product);

}


/**
 * 消费者回调
 */
void *consumerCallBack(void *data) {

    while (isMutex) {
        pthread_mutex_lock(&mutex);
        if (queue.size() > 0) {
            queue.pop();
            LOGD("消费者消费产品，产品剩余数量为 ： %d", queue.size());
        } else {
            LOGD("没有产品消费，等待中...");
            pthread_cond_wait(&cound, &mutex);
        }
        pthread_mutex_unlock(&mutex);
        usleep(1000 * 500);//微秒
    }

    LOGD("退出消费者线程...");
    pthread_exit(&consumer);


}

extern "C"
JNIEXPORT void JNICALL
Java_com_bosma_jnithread_ThreadDemo_mutexThread(JNIEnv *env, jobject instance) {

    if (isMutex) {
        LOGD("生产消费者线程已经存在，不再创建")
        return;
    }

    isMutex = true;
    for (int i = 0; i < 10; ++i) {
        queue.push(i);
    }

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cound, NULL);
    pthread_create(&product, NULL, productCallBack, NULL);
    pthread_create(&consumer, NULL, consumerCallBack, NULL);

}

extern "C"
JNIEXPORT void JNICALL
Java_com_bosma_jnithread_ThreadDemo_stopMutexThread(JNIEnv *env, jobject instance) {
    if (isMutex) {
        LOGD("停止生产消费线程")
        isMutex = false;
        pthread_cond_signal(&cound);
        pthread_mutex_unlock(&mutex);
    }

}


JavaVM *javaVM;
JavaListener *javaListener;


extern "C"
JNIEXPORT void JNICALL
Java_com_bosma_jnithread_ThreadDemo_callBackFromC(JNIEnv *env, jobject instance) {

    javaListener = new JavaListener(javaVM, env, env->NewGlobalRef(instance));
    javaListener->onError(1, 100, "C++ Call Java Method from Main Thread");
}


JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env;
    javaVM = vm;
    if (vm->GetEnv((void **) (&env), JNI_VERSION_1_6) != JNI_OK) {
        return -1;
    }

}