#include <jni.h>
#include <string>

#include "pthread.h"
#include "AndroidLog.h"

#include "queue"
#include "unistd.h"


pthread_t  thread;

void *normalCallBack(void *data)
{
    LOGD("create normal thread from C++");
    pthread_exit(&thread);

}

extern "C"
JNIEXPORT void JNICALL
Java_com_bosma_jnithread_ThreadDemo_normalThread(JNIEnv *env, jobject instance) {

    pthread_create(&thread,NULL,normalCallBack,NULL);

}


pthread_t product;//生产者
pthread_t consumer;//消费者
pthread_mutex_t mutex;//线程锁
pthread_cond_t cound;//


std::queue<int> queue;

void *productCallBack(void *data)
{
    while (1)
    {
        pthread_mutex_lock(&mutex);
        queue.push(1);
        LOGD("生产者生产产品，通知消费者消费，产品数量为 ： %d", queue.size());
        pthread_cond_signal(&cound);
        pthread_mutex_unlock(&mutex);
        sleep(5);
    }

    pthread_exit(&product);

}

void *consumerCallBack(void *data)
{

    while (1)
    {
        pthread_mutex_lock(&mutex);
        if(queue.size() > 0)
        {
            queue.pop();
            LOGD("消费者消费产品，产品剩余数量为 ： %d", queue.size());
        } else {
            LOGD("没有产品消费，等待中...");
            pthread_cond_wait(&cound, &mutex);
        }
        pthread_mutex_unlock(&mutex);
        usleep(1000*500);

    }

    pthread_exit(&consumer);

}

extern "C"
JNIEXPORT void JNICALL
Java_com_bosma_jnithread_ThreadDemo_mutexThread(JNIEnv *env, jobject instance) {


    for (int i = 0; i < 10; ++i) {
        queue.push(i);
    }

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cound,NULL);
    pthread_create(&product, NULL, productCallBack,NULL);
    pthread_create(&consumer, NULL, consumerCallBack,NULL);

}