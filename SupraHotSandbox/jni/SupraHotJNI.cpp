#include <stdio.h>
#include <string.h>
#include <jni.h>
#include <GLES3/gl3.h>
#include <android/log.h>

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))

extern "C"
JNIEXPORT void JNICALL
Java_com_suprahot_SupraHotSandbox_initApp(JNIEnv* env, jobject thiz, jint width, jint height)
{
	int w = width;
	int h = height;
	LOGI("init: %d x %d", w, h);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_suprahot_SupraHotSandbox_progressApp(JNIEnv* env, jobject thiz)
{
	glClearColor(1, 0, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_suprahot_SupraHotSandbox_resizeApp(JNIEnv* env, jobject thiz, jint width, jint height)
{
	int w = width;
	int h = height;
	LOGI("resize: %d x %d", w, h);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_suprahot_SupraHotSandbox_destroyApp(JNIEnv* env, jobject thiz)
{
}