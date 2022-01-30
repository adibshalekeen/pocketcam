#include <thread>
#include <cstdlib>
#include <ctime>
#include <dirent.h>
#include <string>

#include "image_reader.h"

#define DEFAULT_DIR_NAME "/sdcard/DCIM/Camera/"
#define DEFAULT_FILE_NAME "PocketCamCapture"

#define MAX_BUF_COUNT (6)

ImageReader::ImageReader(ImageFormat res,
                         enum AIMAGE_FORMATS format, char* dirName, char* fileName)
        : _dirName(dirName),
          _fileName(fileName),
          _reader(nullptr),
          _callback(nullptr),
          _callbackContext(nullptr) {
    CALL_IMAGE_READER(new(res.width, res.height, format, MAX_BUF_COUNT, &_reader));

    AImageReader_ImageListener listener {
        .context = this,
        .onImageAvailable = onImageCallback
    };
    CALL_IMAGE_READER(setImageListener(_reader, &listener));
}

ImageReader::~ImageReader() {
    if(_reader) {
        AImageReader_delete(_reader);
    }
}

void ImageReader::registerCallback(void *ctx, ImageSavedCallback callback) {
    _callbackContext = ctx;
    _callback = callback;
}

ANativeWindow* ImageReader::getNativeWindow(void) {
    if(!_reader) return nullptr;
    ANativeWindow* nativeWindow;
    CALL_IMAGE_READER(getWindow(_reader, &nativeWindow));
    return nativeWindow;
}

void ImageReader::imageCallback(AImageReader *reader) {
    int32_t format;
    CALL_IMAGE_READER(getFormat(reader, &format));
    if(format && format == AIMAGE_FORMAT_JPEG) {
        AImage* image = nullptr;
        CALL_IMAGE_READER(acquireNextImage(reader, &image));
        std::thread writeFileHandlerThread(&ImageReader::writeFile, this, image);
        writeFileHandlerThread.detach();
    }
}

void ImageReader::writeFile(AImage *image) {
    int planeCount;
    CALL_IMAGE(getNumberOfPlanes(image, &planeCount));
    uint8_t* data = nullptr;
    int len = 0;
    CALL_IMAGE(getPlaneData(image, 0, &data, &len));

    DIR* dir = _dirName ? dir = opendir(_dirName) : dir = opendir(DEFAULT_DIR_NAME);
    if(dir) {
        closedir(dir);
    } else {
        std::string cmd = "mkdir -p ";
        cmd += _dirName ? _dirName : DEFAULT_DIR_NAME;
        system(cmd.c_str());
    }

    struct timespec ts {
        0, 0
    };
    clock_gettime(CLOCK_REALTIME, &ts);
    struct tm localTime;
    localtime_r(&ts.tv_sec, &localTime);

    std::string fileName = _dirName ? _dirName : DEFAULT_DIR_NAME;
    std::string dash("-");

    fileName += _fileName ? _fileName : DEFAULT_FILE_NAME;
    fileName += dash;

    fileName += std::to_string(localTime.tm_mon) + std::to_string(localTime.tm_mday) + dash;
    fileName += std::to_string(localTime.tm_hour) + std::to_string(localTime.tm_min) +
                std::to_string(localTime.tm_sec);
    fileName += ".jpg";

    FILE* file = fopen(fileName.c_str(), "wb");
    if(file && data && len) {
        fwrite(data, 1, len ,file);
        fclose(file);

        if(_callback) {
            _callback(_callbackContext, fileName.c_str());
        }
    } else {
        if(file) fclose(file);
        AImage_delete(image);
    }
}