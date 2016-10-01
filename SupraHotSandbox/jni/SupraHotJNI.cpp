#include <stdio.h>
#include <string.h>
#include <jni.h>
#include "../Wurst.h"
#include <Vec3.h>

/* This is a trivial JNI example where we use a native method
 * to return a new VM String. See the corresponding Java source
 */

static Wurst* wurst;


extern "C"
JNIEXPORT jstring JNICALL
Java_com_suprahot_SupraHotSandbox_stringFromJNI(JNIEnv* env, jobject thiz)
{
	if (wurst == nullptr)
	{
		wurst = new Wurst();
		return env->NewStringUTF("Created new wurst");
	}
	else
	{
		return env->NewStringUTF(wurst->getStuff().c_str());
	}
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_suprahot_SupraHotSandbox_getString(JNIEnv* env, jobject thiz)
{
	return env->NewStringUTF("TEST");
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_suprahot_SupraHotSandbox_getVector(JNIEnv* env, jobject thiz)
{
	SupraHot::Math::Vec3 vector(1.1234, 2.2345, 3.3456);

	std::string result = std::to_string(vector.x) + " " + std::to_string(vector.y) + " " + std::to_string(vector.z);

	return env->NewStringUTF(result.c_str());
}