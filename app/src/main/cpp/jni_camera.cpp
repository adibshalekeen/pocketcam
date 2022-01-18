/**
 * Copyright (C) 2017 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* Description
 *     Demonstrate NDKCamera's PREVIEW mode -- hooks camera directly into
 *     display.
 *     Control:
 *         Double-Tap on Android device's screen to toggle start/stop preview
 *     Tested:
 *         Google Pixel and Nexus 6 phones
 */
#include <jni.h>
#include <cstring>
#include <native_debug.h>
#include "ndk_camera_manager.h"

/**
 * Application object:
 *   the top level camera application object, maintained by native code only
 */
CameraAppEngine *pEngineObj = nullptr;

/**
 * createCamera() Create application instance and NDK camera object
 * @param  width is the texture view window width
 * @param  height is the texture view window height
 * In this sample, it takes the simplest approach in that:
 * the android system display size is used to view full screen
 * preview camera images. Onboard Camera most likely to
 * support the onboard panel size on that device. Camera is most likely
 * to be oriented as landscape mode, the input width/height is in
 * landscape mode. TextureView on Java side handles rotation for
 * portrait mode.
 * @return application object instance ( not used in this sample )
 */
extern "C" JNIEXPORT jlong JNICALL
Java_com_ashaleke_pocketcam_NDKCamera_createCamera(JNIEnv *env,
                       jobject instance,
                       jint width,
                       jint height) {
}

/**
 * deleteCamera():
 *   releases native application object, which
 *   triggers native camera object be released
 */
extern "C" JNIEXPORT void JNICALL
Java_com_ashaleke_pocketcam_NDKCamera_deleteCamera(JNIEnv *env,
                       jobject instance,
                       jlong ndkCameraObj) {
}

/**
 * getCameraCompatibleSize()
 * @returns minimium camera preview window size for the given
 * requested camera size in CreateCamera() function, with the same
 * ascpect ratio. essentially,
 *   1) Display device size decides NDKCamera object preview size
 *   2) Chosen NDKCamera preview size passed to TextView to
 *      reset textureView size
 *   3) textureView size is stretched when previewing image
 *      on display device
 */
extern "C" JNIEXPORT jobject JNICALL
Java_com_ashaleke_pocketcam_NDKCamera_getMinimumCompatiblePreviewSize(JNIEnv *env,
                                          jobject instance,
                                          jlong ndkCameraObj) {
}

/**
 * getCameraSensorOrientation()
 * @ return camera sensor orientation angle relative to Android device's
 * display orientation. This sample only deal to back facing camera.
 */
extern "C" JNIEXPORT jint JNICALL
Java_com_ashaleke_pocketcam_NDKCamera_getCameraSensorOrientation(JNIEnv *env,
                                     jobject instance,
                                     jlong ndkCameraObj) {
}

/**
 * OnPreviewSurfaceCreated()
 *   Notification to native camera that java TextureView is ready
 *   to preview video. Simply create cameraSession and
 *   start camera preview
 */
extern "C" JNIEXPORT void JNICALL
Java_com_ashaleke_pocketcam_NDKCamera_setPreviewSurface(JNIEnv *env,
                                                        jobject instance,
                                                        jlong ndkCameraObj,
                                                        jobject surface) {
}
