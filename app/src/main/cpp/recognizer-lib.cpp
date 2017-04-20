#include <jni.h>
#include <string>
#include "YJYRecognizer.h"

extern "C" {
JNIEXPORT jstring JNICALL
Java_com_yjtc_yjy_mark_unite_utils_recognize_recognizer_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}
JNIEXPORT jboolean JNICALL
Java_com_yjtc_yjy_mark_unite_utils_recognize_recognizer_recognizer_1init(JNIEnv *env, jclass type) {
    return recognizer_init();
}
JNIEXPORT jboolean JNICALL
Java_com_yjtc_yjy_mark_unite_utils_recognize_recognizer_recognizer_1clear(JNIEnv *env,
                                                                          jclass type) {
    return recognizer_clear();
}
JNIEXPORT jboolean JNICALL
Java_com_yjtc_yjy_mark_unite_utils_recognize_recognizer_recognizer_1add(JNIEnv *env, jclass type,
                                                                        jint x, jint y) {
    return recognizer_add(x, y);
}
JNIEXPORT jint JNICALL
Java_com_yjtc_yjy_mark_unite_utils_recognize_recognizer_recognizer_1classify(JNIEnv *env,
                                                                             jclass type) {
    return recognizer_classify();
}
JNIEXPORT jint JNICALL
Java_com_yjtc_yjy_mark_unite_utils_recognize_recognizer_recognizer_1getx(JNIEnv *env, jclass type,
                                                                         jint i) {
    return recognizer_getx(i);
}
JNIEXPORT jint JNICALL
Java_com_yjtc_yjy_mark_unite_utils_recognize_recognizer_recognizer_1gety(JNIEnv *env, jclass type,
                                                                         jint i) {
    return recognizer_gety(i);
}
JNIEXPORT jboolean JNICALL
Java_com_yjtc_yjy_mark_unite_utils_recognize_recognizer_recognizer_1deInit(JNIEnv *env,
                                                                           jclass type) {
    return recognizer_deInit();
}
JNIEXPORT jstring JNICALL
Java_com_yjtc_yjy_mark_unite_utils_recognize_recognizer_recognizer_1getDirectionStr(JNIEnv *env,
                                                                                    jclass type) {
    return env->NewStringUTF(recognizer_getDirectionStr());
}
JNIEXPORT jint JNICALL
Java_com_yjtc_yjy_mark_unite_utils_recognize_recognizer_recognizer_1getResultCnt(JNIEnv *env,
                                                                                 jclass type) {
    return recognizer_getResultCnt();
}
}

