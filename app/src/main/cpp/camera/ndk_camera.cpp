//
// Created by ashal on 2022-01-11.
//

#include "ndk_camera.h"

void NDKCamera::NDKCamera(ACameraManager* manager,
                          ACameraDevice* device,
                          std::string cameraId,
                          CameraStatusCallback callback) : _connected(false){
    _cameraDevice = device;
    _callback = callback;
    _requestInfo.resize(CAPTURE_REQUEST_COUNT);
    memset(_requestInfo.data(), 0, _requestInfo.size() * sizeof(_requestInfo[0]));
    static ACameraDevice_stateCallbacks cameraDeviceListener = {
        .context = this,
        .onDisconnected = ::onDisconnected,
        .onError = ::onError,
    };
    CALL_MGR(openCamera(manager,
                        cameraId.c_str(),
                        &cameraDeviceListener,
                        &_cameraDevice));
    _connected = true;
    CALL_MGR(getCameraCharacteristics(manager, cameraId.c_str(), &_metaData));
    loadExposure();
    loadSensitivity();
    loadFocusDistance();
    loadDigitalZoom();
    loadWhiteBalance();
    loadImageFormats();
}

void NDKCamera::createSession(ANativeWindow* previewWindow,
                              ANativeWindow* jpgWindow,
                              ANativeWindow* rawWindow) {
    _requestInfo[PREVIEW_REQUEST_IDX].outputWindow = previewWindow;
    _requestInfo[PREVIEW_REQUEST_IDX].captureRequestTemplate = TEMPLATE_MANUAL;
    _requestInfo[JPG_CAPTURE_REQUEST_IDX].outputWindow = jpgWindow;
    _requestInfo[JPG_CAPTURE_REQUEST_IDX].captureRequestTemplate = TEMPLATE_MANUAL;
    _requestInfo[RAW_CAPTURE_REQUEST_IDX].outputWindow = rawWindow;
    _requestInfo[RAW_CAPTURE_REQUEST_IDX].captureRequestTemplate = TEMPLATE_MANUAL;
    CALL_CONTAINER(create(&_sessionOutputContainer));
    for(CameraRequestInfo info : _requestInfo) {
        ANativeWindow_acquire(info.outputWindow);
        CALL_OUTPUT(create(info.outputWindow, &info.sessionOutput));
        CALL_CONTAINER(add(_sessionOutputContainer, info.sessionOutput));
        CALL_TARGET(create(info.outputWindow, &info.sessionOutputTarget));
        CALL_DEV(createCaptureRequest(_cameraDevice, info.template, &info.captureRequest));
        CALL_REQUEST(addTarget(info.captureRequest, info.sessionOutputTarget));
    }
    ACameraCaptureSession_stateCallbacks sessionStateCallbacks = {
        .context = this,
        .onClosed = ::onSessionClosed,
        .onReady = ::onSessionReady,
        .onActive = ::onSessionActive,
    };
    CALL_DEV(createCaptureSession(_cameraDevice,
                                  _sessionOutputContainer,
                                  &sessionStateCallback,
                                  &_captureSession));
}

void NDKCamera::loadImageFormats(){
    ACameraMetadata_const_entry formats;
    CALL_METADATA(getConstEntry(
        _metaData, ACAMERA_SCALER_AVAILABLE_STREAM_CONFIGURATIONS, &formats
    ));

    for(int i = 0; i < formats.count; ++i) {
        int32_t input = entry.data.i32[i + 3];
        if (input) continue;
        int32_t format = entry.data.i32[i + 0];
        DisplayDimension disp(entry.data.i32[i + 1], entry.data.i32[i + 2]);
        _formats.push_back({
            .format = format,
            .display = display,
        });
    }
}

void NDKCamera::loadExposure() {
    ACameraMetadata_const_entry exposures;
    if (!getMetadataCharacteristic(ACAMERA_SENSOR_INFO_EXPOSURE_TIME_RANGE, &exposures)) return;
    int64_t exposureMax, exposureMin;
    exposureMin = exposures.data.i64[0];
    exposureMax = exposures.data.i64[1];
    _cameraSettings.exposure[MIN] = exposureMin;
    _cameraSettings.exposure[MAX] = exposureMax;
    _cameraSettings.exposure[CURRENT] = exposureMin + (0.2 * (exposureMax - exposureMin));
}

void NDKCamera::loadSensitivity() {
    ACameraMetadata_const_entry sensitivities;
    if (!getMetadataCharacteristic(ACAMERA_SENSOR_INFO_SENSITIVITY_RANGE, &sensitivities)) return;
    int32_t sensitivityMin = sensitivities.data.i32[0];
    int32_t sensitivityMax = sensitivities.data.i32[1];
    _cameraSettings.sensitivity[MIN] = sensitivityMin;
    _cameraSettings.sensitivity[MAX] = sensitivityMax;
    _cameraSettings.sensitivity[CURRENT] = sensitivityMin + (0.2 * (sensitivityMax - sensitivityMin));
}

void NDKCamera::loadFocusDistance() {
    ACameraMetadata_const_entry focusDistance;
    if (!getMetadataCharacteristic(ACAMERA_LENS_FOCUS_RANGE, &focusDistance)) return;
    _cameraSettings.focusDistance[MIN] = focusDistance.data.f[0];
    _cameraSettings.focusDistance[MAX] = focusDistance.data.f[1];
    if (!getMetadataCharacteristic(ACAMERA_LENS_FOCUS_DISTANCE, &focusDistance)) return;
    _cameraSettings.focusDistance[CURRENT] = focusDistance.data.float32[0];
}

void NDKCamera::loadDigitalZoom() {
    ACameraMetadata_const_entry digitalZoom;
    if (!getMetadataCharacteristic(ACAMERA_CONTROL_ZOOM_RATIO_RANGE, &focusDistance)) return;
    _cameraSettings.digitalZoom[MIN] = digitalZoom.data.f[0];
    _cameraSettings.digitalZoom[MAX] = digitalZoom.data.f[1];
    _cameraSettings.digitalZoom[CURRENT] = 1.0;
}

void NDKCamera::~NDKCamera() {

}