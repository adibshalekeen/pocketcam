#ifndef POCKETCAM_IMAGE_READER_H
#define POCKETCAM_IMAGE_READER_H

#include <vector>
#include <string>
#include <media/NdkImageReader.h>

#include "native_debug.h"
#include "media_utils.h"

struct ImageFormat {
    uint32_t width;
    uint32_t height;
    uint32_t format;
};

typedef void (*ImageSavedCallback)(void* ctx, const char* fileName);

class ImageReader {
public:
    ImageReader(ImageFormat res,
                enum AIMAGE_FORMATS format,
                char* dirName,
                char* fileName);
    ~ImageReader();
    ANativeWindow* getNativeWindow(void);
    void registerCallback(void* ctx, ImageSavedCallback callback);
private:
    AImageReader* _reader;
    char* _dirName;
    char* _fileName;
    void* _callbackContext;
    ImageSavedCallback _callback;

    void writeFile(AImage* image);

    void imageCallback(AImageReader* reader);

    static void onImageCallback(void* ctx, AImageReader *reader) {
        reinterpret_cast<ImageReader *>(ctx)->imageCallback(reader);
    }
};
#endif