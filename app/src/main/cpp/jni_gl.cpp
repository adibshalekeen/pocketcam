#include <jni.h>

#include <native_debug.h>
#include <android/native_window.h>
#include <android/native_window_jni.h>

extern "C" {
    JNIEXPORT void JNICALL
    Java_com_ashaleke_pocketcam_ui_CameraRenderer_onSurfaceCreated(JNIEnv *env,
                                                                   jobject,
                                                                   jint texId,
                                                                   jobject surface)
    {

    }

    JNIEXPORT void JNICALL
    Java_com_ashaleke_pocketcam_ui_CameraRenderer_onSurfaceChanged(JNIEnv *env,
                                                                   jobject,
                                                                   jint w,
                                                                   jint h)
    {

    }

    JNIEXPORT void JNICALL
    Java_com_ashaleke_pocketcam_ui_CameraRenderer_onDrawFrame(JNIEnv *env,
                                                              jobject,
                                                              jfloatArray texMatArray)
    {

    }
}