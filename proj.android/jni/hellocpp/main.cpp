#include "bridge.h"
#include "GameScene.h"
#include "AppDelegate.h"
#include "platform/android/jni/JniHelper.h"
#include <jni.h>
#include <android/log.h>
#include <string>

#define  LOG_TAG    "main"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

using namespace cocos2d;

#ifndef CRASH
#define CRASH() do { \
	*(int *)(uintptr_t)0xbbadbeef = 0; \
	((void(*)())0)(); /* More reliable, but doesn't say BBADBEEF */ \
}while(false)
#endif

bool checkException(JNIEnv* env)
{
	if (env->ExceptionCheck() != 0) {
		LOGE("*** Uncaught exception returned from Java call!");
		env->ExceptionDescribe();
		return true;
	}
	return false;
}

static jmethodID getJMethod(JNIEnv* env, jclass clazz, const char name[], const char signature[])
{
	jmethodID m = env->GetMethodID(clazz, name, signature);
	return m;
}

struct JavaGlue {
	jobject   popStarObj;
	jobject   renderObj;
	jmethodID winPuzzle;
	jmethodID gameOver;
	jmethodID rate;
	//jmethodID handleOnCreate;
};

JavaGlue glue;

void onWinPazzule(int stage) {
	JNIEnv *env = JniHelper::getEnv();
	if (!env) {
		return;
	}
	env->CallVoidMethod(glue.popStarObj, glue.winPuzzle, stage);
	//env->CallVoidMethod(glue.renderObj, glue.handleOnCreate);
	checkException(env);
}

void onGameOver(int score, int highScore) {
	JNIEnv *env = JniHelper::getEnv();
	if (!env) {
		return;
	}
	env->CallVoidMethod(glue.popStarObj, glue.gameOver, score, highScore);
	checkException(env);
}

void rate() {
	JNIEnv *env = JniHelper::getEnv();
	if (!env) {
		return;
	}
	env->CallVoidMethod(glue.popStarObj, glue.rate);
	checkException(env);
}

extern "C"
{

jint JNI_OnLoad(JavaVM *vm, void *reserved)
{
    JniHelper::setJavaVM(vm);
    return JNI_VERSION_1_4;
}

void Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeInit(JNIEnv*  env, jobject thiz, jint w, jint h)
{
    if (!CCDirector::sharedDirector()->getOpenGLView())
    {
        CCEGLView *view = CCEGLView::sharedOpenGLView();
        view->setFrameSize(w, h);

        AppDelegate *pAppDelegate = new AppDelegate();
        CCApplication::sharedApplication()->run();
    }

    glue.renderObj = env->NewLocalRef(thiz);
    //glue.handleOnCreate = getJMethod(env,
    //		env->FindClass("org/cocos2dx/lib/Cocos2dxRenderer"), "handleOnCreate", "()V");
}

void Java_com_crazyboy_popstar_PopStar_nativeInit(JNIEnv* env, jobject thiz, jstring id)
{
	std::string key = JniHelper::jstring2string(id);
	if (key != "52d8a4678feb471e8432160d99e65dc6") {
		CRASH();
	}

	jclass clazz = env->FindClass("com/crazyboy/popstar/PopStar");
	glue.popStarObj = env->NewLocalRef(thiz);
	glue.winPuzzle = getJMethod(env, clazz, "winPuzzle", "(I)V");
	glue.gameOver = getJMethod(env, clazz, "gameOver", "(II)V");
	glue.rate = getJMethod(env, clazz, "rate", "()V");
}

void Java_com_crazyboy_popstar_PopStar_nativeHit(JNIEnv* env, jobject thiz)
{
	GameScene::instance()->autoHitTest();
}

}
