#include "sortNDK.h"

extern "C" {
    JNIEXPORT jintArray JNICALL Java_smart_endlessnews_NewsActivity_sortNDK(JNIEnv *env, jclass clazz, jintArray news, jintArray freqs, jint size) {
        jint* c_news = (*env).GetIntArrayElements(news, false);
        jint* c_freqs = (*env).GetIntArrayElements(freqs, false);

        for (int i = size - 1; i >= 0; i--) {
            for (int j = 0; j < i; j++) {
                if (c_freqs[j] < c_freqs[j+1]) {
                    jint tmp = c_freqs[j];
                    c_freqs[j] = c_freqs[j+1];
                    c_freqs[j+1] = tmp;

                    tmp = c_news[j];
                    c_news[j] = c_news[j+1];
                    c_news[j+1] = tmp;
                }
            }
        }

        jintArray result = (*env).NewIntArray(size);
        (*env).SetIntArrayRegion(result, 0, size, c_news);

        return result;
    }
};
