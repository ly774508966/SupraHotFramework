#include <stdio.h>
#include <string.h>
#include <jni.h>
#include <android/log.h>
#include <App.h>

#include "../Bootstrap.h"
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))


// ----
// App
// ----
static SupraHot::Bootstrap::MAIN_APP* app;

extern "C"
JNIEXPORT void JNICALL
Java_com_suprahot_SupraHotSandbox_initApp(JNIEnv* env, jobject thiz, jint width, jint height)
{
	int w = width;
	int h = height;

	app = new SupraHot::Bootstrap::MAIN_APP();
	app->Init(w, h, "SupraHotSandbox");
}

extern "C"
JNIEXPORT void JNICALL
Java_com_suprahot_SupraHotSandbox_progressApp(JNIEnv* env, jobject thiz)
{
	SupraHot::App::GetMainApp()->ProgressApp();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_suprahot_SupraHotSandbox_resizeApp(JNIEnv* env, jobject thiz, jint width, jint height)
{
	int w = width;
	int h = height;
	SupraHot::App::GetMainApp()->Resize(w, h);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_suprahot_SupraHotSandbox_destroyApp(JNIEnv* env, jobject thiz)
{
	SupraHot::App::GetMainApp()->Destroy();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_suprahot_SupraHotSandbox_setFileSystemPath(JNIEnv* env, jobject thiz, jstring fileSystemPath)
{

}