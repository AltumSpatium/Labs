#include <jni.h>

#ifndef ENDLESSNEWS_SORTNDK_H
#define ENDLESSNEWS_SORTNDK_H

extern "C" {
    JNIEXPORT jintArray JNICALL Java_smart_endlessnews_NewsActivity_sortNDK(JNIEnv *env, jclass clazz, jintArray news, jintArray freqs, jint size);
}

#endif //ENDLESSNEWS_SORTNDK_H
