#include <jni.h>
#include <string>

#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#include "AndroidLog.h"

//引擎接口
SLObjectItf engineObject = NULL;
SLEngineItf engineEngine = NULL;

//混音器
SLObjectItf outputMixObject = NULL;
SLEnvironmentalReverbItf outputMixEnvironmentReverb = NULL;
SLEnvironmentalReverbSettings reverbSettings = SL_I3DL2_ENVIRONMENT_PRESET_STONECORRIDOR;

//PCM
SLObjectItf pcmPlayerObject = NULL;
SLPlayItf pcmPlayerPlay = NULL;

//缓冲队列接口
SLAndroidSimpleBufferQueueItf pcmBufferQueue = NULL;

FILE *pcmFile;
void *buffer;
uint8_t *out_buffer;

int getPcmData(void **pcm) {
    int size = 0;
    while (!feof(pcmFile)) {
        size = fread(out_buffer, 1, 44100 * 2 * 2, pcmFile);
        if (out_buffer == NULL) {
            if (LOG_DEBUG) {
                LOGE("Read end");
            }
            break;
        } else {
            if (LOG_DEBUG) {
                LOGE("Reading");
            }
        }

        *pcm = out_buffer;
        break;
    }
    return size;
}

void pcmBufferCallback(SLAndroidSimpleBufferQueueItf buf, void *context) {

    int size = getPcmData(&buffer);
    if (buffer != NULL) {
        (*pcmBufferQueue)->Enqueue(pcmBufferQueue, buffer, size);
    }

}


extern "C"
JNIEXPORT void JNICALL
Java_com_bosma_openglslaudiodemo_MainActivity_playpcm(JNIEnv *env, jobject instance, jstring url_) {
    const char *url = env->GetStringUTFChars(url_, 0);

    pcmFile = fopen(url, "r");
    if (pcmFile == NULL) {
        if (LOG_DEBUG) {
            LOGE("file read permission failed.")
        }
        return;
    }

    out_buffer = (uint8_t *) malloc(44100 * 2 * 2);


    SLresult result;
    //第一步
    //创建
    result = slCreateEngine(&engineObject, 0, 0, 0, 0, 0);
    //realize
    result = (*engineObject)->Realize(engineObject, SL_BOOLEAN_FALSE);
    //Get engineEngine
    result = (*engineObject)->GetInterface(engineObject, SL_IID_ENGINE, &engineEngine);

    //第二步：创建混音器
    const SLInterfaceID mids[1] = {SL_IID_ENVIRONMENTALREVERB};//都只放一个
    const SLboolean mreq[1] = {SL_BOOLEAN_FALSE};

    result = (*engineEngine)->CreateOutputMix(engineEngine, &outputMixObject, 1, mids, mreq);
    (void) result;
    result = (*outputMixObject)->Realize(outputMixObject, SL_BOOLEAN_FALSE);
    (void) result;
    result = (*outputMixObject)->GetInterface(outputMixObject, SL_IID_ENVIRONMENTALREVERB,
                                              &outputMixEnvironmentReverb);
    if (SL_RESULT_SUCCESS == result) {
        (*outputMixEnvironmentReverb)->SetEnvironmentalReverbProperties(outputMixEnvironmentReverb,
                                                                        &reverbSettings);
        (void) result;
    }


    SLDataLocator_OutputMix outputMix = {SL_DATALOCATOR_OUTPUTMIX, outputMixObject};
    SLDataSink audioSnk = {&outputMix, NULL};
    //创建播放器
    SLDataLocator_AndroidBufferQueue androidBufferQueue = {SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE,
                                                           2};
    SLDataFormat_PCM pcm = {
            SL_DATAFORMAT_PCM,
            2,
            SL_SAMPLINGRATE_44_1,//采样率
            SL_PCMSAMPLEFORMAT_FIXED_16,//位数
            SL_PCMSAMPLEFORMAT_FIXED_16,//位数
            SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT,//左前方右前方
            SL_BYTEORDER_LITTLEENDIAN

    };


    const SLInterfaceID ids[1] = {SL_IID_BUFFERQUEUE};
    const SLboolean req[1] = {SL_BOOLEAN_TRUE};

    SLDataSource slDataSource = {&androidBufferQueue, &pcm};

    (*engineEngine)->CreateAudioPlayer(engineEngine, &pcmPlayerObject, &slDataSource, &audioSnk, 1,
                                       ids, req);

    (*pcmPlayerObject)->Realize(pcmPlayerObject, SL_BOOLEAN_FALSE);
    (*pcmPlayerObject)->GetInterface(pcmPlayerObject, SL_IID_PLAY, &pcmPlayerPlay);
    (*pcmPlayerObject)->GetInterface(pcmPlayerObject, SL_IID_BUFFERQUEUE, &pcmBufferQueue);

    (*pcmBufferQueue)->RegisterCallback(pcmBufferQueue, pcmBufferCallback, NULL);

    (*pcmPlayerPlay)->SetPlayState(pcmPlayerPlay, SL_PLAYSTATE_PLAYING);//播放状态
    pcmBufferCallback(pcmBufferQueue, NULL);//执行回调


    env->ReleaseStringUTFChars(url_, url);
}