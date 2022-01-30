//
// Created by ashal on 2022-01-11.
//

#ifndef POCKETCAM_NDK_CAMERA_MANAGER_H
#define POCKETCAM_NDK_CAMERA_MANAGER_H

#include <jni.h>
#include <android/native_activity.h>
#include <android/native_window_jni.h>
#include <android/native_window.h>

#include "ndk_camera.h"
#include "image_reader.h"

class NDKCameraManager {
public:
    NDKCameraManager(JNIEnv* env, jobject instance);
    ~NDKCameraManager();
    void setPreviewSurface(jobject surface);
    void startPreview();
    void takePhoto();
private:
    JNIEnv* _env;
    jobject _javaInstance;
    jobject _surface;
    ImageReader* _JPEGReader;
    ACameraManager* _cameraManager;
    NDKCamera* _camera;
    std::string enumerateCameras();
};


#endif //POCKETCAM_NDK_CAMERA_MANAGER_H
