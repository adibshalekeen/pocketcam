//
// Created by ashal on 2022-01-11.
//

#include "ndk_camera_manager.h"

NDKCameraManager::NDKCameraManager(JNIEnv* env, jobject instance) : _env(env),
                                                                    _javaInstance(instance),
                                                                    _surface(nullptr),
                                                                    _camera(nullptr) {
    _cameraManager = ACameraManager_create();
    std::string cid = enumerateCameras();
    struct CameraStatusCallback cb = {
            .cameraRequestStateCallback = nullptr,
            .cameraSessionStateCallback = nullptr,
            .connectedCallback = nullptr,
            .disconnectedCallback = nullptr,
            .errorCallback = nullptr
    };
    _camera = new NDKCamera(_cameraManager,
                            cid,
                            cb);
}

NDKCameraManager::~NDKCameraManager() {
    if(_surface) {
        _env->DeleteGlobalRef(_surface);
        _surface = nullptr;
    }
}

void NDKCameraManager::setPreviewSurface(jobject surface) {
    _surface = _env->NewGlobalRef(surface);
    LOGE("SET SURFACE GLOBAL");
    _camera->createSession(ANativeWindow_fromSurface(_env, _surface), nullptr, nullptr);
    _camera->startPreview();
}

std::string NDKCameraManager::enumerateCameras() {
  ACameraIdList* cameraIds = nullptr;
  CALL_MGR(getCameraIdList(_cameraManager, &cameraIds));

  for (int i = 0; i < cameraIds->numCameras; ++i) {
    const char* id = cameraIds->cameraIds[i];

    ACameraMetadata* metadataObj;
    CALL_MGR(getCameraCharacteristics(_cameraManager, id, &metadataObj));

    int32_t count = 0;
    const uint32_t* tags = nullptr;
    ACameraMetadata_getAllTags(metadataObj, &count, &tags);
    for (int tagIdx = 0; tagIdx < count; ++tagIdx) {
      if (ACAMERA_LENS_FACING == tags[tagIdx]) {
        ACameraMetadata_const_entry lensInfo = {
            0,
        };
        CALL_METADATA(getConstEntry(metadataObj, tags[tagIdx], &lensInfo));
        auto lensDir = static_cast<acamera_metadata_enum_android_lens_facing_t>(lensInfo.data.u8[0]);
        if (lensDir == ACAMERA_LENS_FACING_BACK) {
            std::string camId(id);
            return camId;
        }
        }
      }
      ACameraMetadata_free(metadataObj);
  }
    return nullptr;
}