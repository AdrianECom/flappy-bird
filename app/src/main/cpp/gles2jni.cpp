#include <jni.h>

#include <time.h>
#include <string.h>

#include "Game.h"

static Game* game = NULL;

extern "C" {
JNIEXPORT void JNICALL Java_com_android_gles2jni_GLES2JNILib_init(JNIEnv* env, jobject obj);
JNIEXPORT void JNICALL Java_com_android_gles2jni_GLES2JNILib_resize(JNIEnv* env, jobject obj, jint width, jint height);
JNIEXPORT void JNICALL Java_com_android_gles2jni_GLES2JNILib_step(JNIEnv* env, jobject obj);
JNIEXPORT void JNICALL Java_com_android_gles2jni_GLES2JNILib_touch(JNIEnv *env, jclass type);
};

JNIEXPORT void JNICALL
Java_com_android_gles2jni_GLES2JNILib_init(JNIEnv* env, jobject obj) {
    if (game) {
        delete game;
        game = NULL;
    }

    game = Game::createGame();
}

JNIEXPORT void JNICALL
Java_com_android_gles2jni_GLES2JNILib_resize(JNIEnv* env, jobject obj, jint width, jint height) {
    if (game) {
        game->resize(width, height);
    }
}

JNIEXPORT void JNICALL
Java_com_android_gles2jni_GLES2JNILib_step(JNIEnv* env, jobject obj) {
    if (game) {
        game->render();
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_android_gles2jni_GLES2JNILib_touch(JNIEnv *env, jclass type) {
    if(game) {
        game->touch();
    }
}