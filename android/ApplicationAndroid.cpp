#include "../src/Common.h"
#include "ApplicationAndroid.h"

#include <jni.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

using namespace FlappyBird;

ApplicationAndroid::ApplicationAndroid():
	Application()
{
}

ApplicationAndroid::~ApplicationAndroid()
{
}

static AAssetManager* assetManager = nullptr;

static int android_read(void* cookie, char* buf, int size)
{
	return AAsset_read((AAsset*)cookie, buf, size);
}

static int android_write(void* cookie, const char* buf, int size)
{
	return EACCES;
}

static fpos_t android_seek(void* cookie, fpos_t offset, int whence)
{
	return AAsset_seek((AAsset*)cookie, offset, whence);
}

static int android_close(void* cookie)
{
	AAsset_close((AAsset*)cookie);
	return 0;
}

FILE* ApplicationAndroid::OpenFile(const std::string& path, const std::string& mode)
{
	if(mode[0] == 'w') return nullptr;

	AAsset* asset = AAssetManager_open(assetManager, path.c_str(), 0);
	if(!asset) return nullptr;

	return funopen(asset, android_read, android_write, android_seek, android_close);
}

extern "C" {
	JNIEXPORT void JNICALL Java_com_android_flappycube_GameLib_init(JNIEnv * env, jobject obj, jobject assetManager,  jint width, jint height);
	JNIEXPORT void JNICALL Java_com_android_flappycube_GameLib_step(JNIEnv * env, jobject obj);
	JNIEXPORT void JNICALL Java_com_android_flappycube_GameLib_tap(JNIEnv * env, jobject obj);
	JNIEXPORT void JNICALL Java_com_android_flappycube_GameLib_pause(JNIEnv * env, jobject obj);
};

JNIEXPORT void JNICALL Java_com_android_flappycube_GameLib_init(JNIEnv * env, jobject obj, jobject inAssetManager, jint width, jint height)
{
	LOG("started\n");
	assetManager = AAssetManager_fromJava(env, inAssetManager);
	if ( Application::GetInstance() == nullptr )
	{
		new ApplicationAndroid();
	}
	Application::GetInstance()->Initialize(width,height);
	LOG("Initialize w%i h%i\n", width, height);
}

JNIEXPORT void JNICALL Java_com_android_flappycube_GameLib_step(JNIEnv * env, jobject obj)
{
	Application::GetInstance()->Tick();
}

JNIEXPORT void JNICALL Java_com_android_flappycube_GameLib_tap(JNIEnv * env, jobject obj)
{
	Application::GetInstance()->MouseDown(0,0);
}

JNIEXPORT void JNICALL Java_com_android_flappycube_GameLib_pause(JNIEnv * env, jobject obj)
{
	Application::GetInstance()->Deactivate();
}
