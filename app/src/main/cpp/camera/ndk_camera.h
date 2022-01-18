//
// Created by ashal on 2022-01-11.
//

#ifndef POCKETCAM_NDK_CAMERA_H
#define POCKETCAM_NDK_CAMERA_H

#include <string>
#include <vector>

#include <camera/NdkCameraManager.h>
#include <camera/NdkCameraError.h>
#include <camera/NdkCameraDevice.h>
#include <camera/NdkCameraMetadataTags.h>
#include <camera/common/camera_utils.h>

#include "display_dimension.h"

enum SettingsIndex {
    MIN=0,
    MAX=1,
    CURRENT=2
};

enum class CaptureSessionState {
    READY=0,
    ACTIVE,
    CLOSED,
    MAX_STATE
};

typedef struct CameraSettings {
    int32_t direction;
    int32_t orientation;
    float focusDistance[3];
    float digitalZoom[3];
    int64_t exposure[3];
    int32_t sensitivity[3];
};

typedef struct ImageFormat {
    int32_t format;
    DisplayDimension display;
};

enum PREVIEW_INDICES {
    PREVIEW_REQUEST_IDX = 0,
    JPG_CAPTURE_REQUEST_IDX,
    RAW_CAPTURE_REQUEST_IDX,
    CAPTURE_REQUEST_COUNT,
};

typedef void (*onCameraDisconnected)(std::string id);
typedef void (*onCameraError)(std::string id, int err);
typedef void (*onCameraConnected)();
typedef void (*onCameraSessionState)(ACameraCaptureSession* session, CaptureSessionState state);
typedef void (*onCameraRequestState)();

/** 
 * Struct of callbacks for camera manager to get information
 * about camera status
 */
typedef struct CameraStatusCallback {
    onCameraDisconnected disconnectedCallback;
    onCameraError errorCallback;
    onCameraConnected connectedCallback;
    onCameraSessionState cameraSessionStateCallback;
    onCameraRequestState cameraRequestStateCallback;
};

/**
 * Struct used to store all of the information required to
 * start a camera request capture. One of these should be initialized
 * for each output frame format.
 */
struct CameraRequestInfo {
    ANativeWindow* outputWindow;
    ACaptureSessionOutput* sessionOutput;
    ACameraOutputTarget* sessionOutputTarget;
    ACaptureRequest* captureRequest;
    ACameraDevice_request_template captureRequestTemplate;
    int sequenceId;
}
class NDKCamera {
public:
    NDKCamera(ACameraManager* manager,
              ACameraDevice* device,
              std::string cameraId,
              CameraStatusCallback callback);
    ~NDKCamera();
    void setCameraDevice(ACameraDevice* cameraDevice);
    void signalError(std::string id, int err) {
        std::string id(ACameraDevice_getId(dev));
        std::stringstream errorString;
        _connected = false;
        LOGI("Error encountered in camera %s. Error Id %#x", id.c_str, err);
        
        switch(err) {
            case ERROR_CAMERA_IN_USE:
                stringStream << "Camera is already in use";
                break;
            case ERROR_CAMERA_SERVICE:
                stringStream << "Camera service encountered a fatal error";
                break;
            case ERROR_CAMERA_DEVICE:
                stringStream << "Camera device encountered a fatal error";
                break;
            case ERROR_CAMERA_DISABLED:
                stringStream << "Camera is disabled due to device policy";
                break;
            case ERROR_MAX_CAMERAS_IN_USE:
                stringStream << "Reached maximum number of open cameras";
                break;
            default:
                stringStream << "Unknown camera error";
                break;
        }
        stringStream << std::endl;
        LOGE("%s", stringStream.str().c_str());
        _callback.errorCallback(id, err);
    }
    void signalDisconnected(std::string id) {
        ACameraDevice_close(dev);
        _connected = false;
        std::string id(ACameraDevice_getId(dev));
        _callback.disconnectedCallback(id);
    }
    void createSession(ANativeWindow* previewWindow,
                       ANativeWindow* jpgWindow,
                       ANativeWindow* rawWindow);
    void sessionStateCallback(ACameraCaptureSession* session, CaptureSessionState state) {
        _callback.cameraSessionStateCallback(session, state);
    }
private:
    ACameraDevice* _cameraDevice;
    ACameraMetadata* _metaData;
    CameraSettings _cameraSettings;
    CameraStatusCallback _callback;
    std::vector<CaptureRequestInfo> _requestInfo;
    ACaptureSessionOutputContainer* _sessionOutputContainer;
    ACameraCaptureSession* _captureSession;
    CaptureSessionState _captureSessionState;
    std::vector<ImageFormat> _imageFormats;
    char _cameraId;
    bool _connected;
    void onDisconnected(void* ctx, ACameraDevice* dev) {
       reinterpret_case<NDKCamera*>(ctx)->signalDisconnected(id);
    }
    void onError(void* ctx, ACameraDevice* dev, int err) {
        reinterpret_case<NDKCamera*>(ctx)->signalError(id, err);
    }
    void onSessionClosed(void* ctx, ACameraCaptureSession* session) {
        LOGI("Session closed %p", session);
        reinterpret_cast<NDKCamera*>(ctx)->sessionStateCallback(session, CaptureSessionState::CLOSED);
    }
    void onSessionReady(void* ctx, ACameraCaptureSession* session) {
        LOGI("Session ready %p", session);
        reinterpret_cast<NDKCamera>(ctx)->sessionStateCallback(session, CaptureSessionState::READY);
    }
    void onSessionActive(void* ctx, ACameraCaptureSession* session) {
        LOGI("Session active %p", session);
        reinterpret_cast<NDKCamera>(ctx)->sessionStateCallback(session, CaptureSessionState::ACTIVE);
    }
    bool getMetadataCharacteristic(uint32_t tag, ACameraMetadata_const_entry* val) {
        camera_status_t status = ACameraMetadata_getConstEntry(
            _metaData,
            tag,
            val
        );
        if(status == ACAMERA_OK) {
            return true;
        }
        if(status == ACAMERA_ERROR_IVALID_PARAMETER) {
            LOGE("Attempting to extract property %d from null metadata", tag);
        }
        if(status == ACAMERA_ERROR_METADATA_NOT_FOUND) {
            LOGE("Input metadata doesn't contain input tag %d", tag);
        }
        return false
    }
    void loadImageFormats();
    void loadExposure();
    void loadSensitivity();
    void loadFocusDistance();
    void loadDigitalZoom();
    void loadWhiteBalance();
};


#endif //POCKETCAM_NDK_CAMERA_H
