#include <jni.h>

#include <native_debug.h>
#include <android/native_window.h>
#include <android/native_window_jni.h>

#include "camera_preview.h"
#include "native_debug.h"

CameraPreview* CAMERA_PREVIEW = nullptr;
extern "C" JNIEXPORT void JNICALL
    Java_com_ashaleke_pocketcam_ui_CameraRenderer_onSurfaceCreated(JNIEnv *env,
                                    jobject,
                                    jint texId,
                                    jobject surface)
    {
        CAMERA_PREVIEW = new CameraPreview(texId);
        LOGD("Camera preview surface initialized");
    }

extern "C" JNIEXPORT void JNICALL
Java_com_ashaleke_pocketcam_ui_CameraRenderer_onSurfaceChanged(JNIEnv *env,
                                jobject,
                                jint w,
                                jint h)
{
    if(!CAMERA_PREVIEW)
    {
        LOGE("Attempting to change surface dimensions without intializing surface");
        return;
    }

    LOGD("Setting surface dimensions %d %d", w, h);
    CAMERA_PREVIEW->onSurfaceChanged(w, h);
}

extern "C" JNIEXPORT void JNICALL
Java_com_ashaleke_pocketcam_ui_CameraRenderer_onDrawFrame(JNIEnv *env,
                           jobject)
{
    if(!CAMERA_PREVIEW)
    {
        LOGE("Attempting to draw frame without initializing surface");
        return;
    }

    CAMERA_PREVIEW->onDrawFrame();
}

extern "C" JNIEXPORT void JNICALL
Java_com_ashaleke_pocketcam_ui_CameraRenderer_setTextureMatrix(JNIEnv *env,
                                jobject,
                                jfloatArray matrix)
{
    if(!CAMERA_PREVIEW)
    {
        LOGE("Attempting to set texture matrix before surface initialization");
        return;
    }

    unsigned int matLen = env->GetArrayLength(matrix);
    if(matLen != 16)
    {
        LOGE("Invalid array supplied as texture matrix array. Array has size %d, expected 16",
             matLen);
        return;
    }

    LOGE("Setting texture transformaton matrix");
    float textureMatrix[16];
    float* JNImatrix = env->GetFloatArrayElements(matrix, NULL);
    for(int i=0; i < matLen; ++i)
    {
        textureMatrix[i] = JNImatrix[i];
    }

    CAMERA_PREVIEW->setTextureMatrix(textureMatrix);
}