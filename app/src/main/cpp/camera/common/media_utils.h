#ifndef POCKETCAM_MEDIA_UTILS_H
#define POCKETCAM_MEDIA_UTILS_H
#include <vector>
#include <media/NdkImageReader.h>

#define CALL_MEDIA(func)                         \
{                                                \
    media_status_t status = func;                \
    LOGE("%s STATUS %p", #func, status);         \
    ASSERT(status == AMEDIA_OK,                  \
           "%s call failed with code: %#x, %s",  \
           __FUNCTION__,                         \
           status,                               \
           getMediaErrorStr(status));            \
}                                                \

#define CALL_IMAGE_READER(func) CALL_MEDIA(AImageReader_##func)
#define CALL_IMAGE(func) CALL_MEDIA(AImage_##func)
#define CALL_MEDIA_CODEC_GROUP(func) CALL_MEDIA(AMediaCodec##func)
#define CALL_MEDIA_CODEC_ACTION_CODE(func) CALL_MEDIA_CODEC_GROUP(ActionCode_##func)
#define CALL_MEDIA_CRYPT_INFO(func) CALL_MEDIA_CODEC_GROUP(CryptoInfo_##func)
#define CALL_MEDIA_CODEC(func) CALL_MEDIA(AMediaCodec_##func)
#define CALL_MEDIA_CRYPTO(func) CALL_MEDIA(AMediaCrypto_##func)
#define CALL_MEDIA_DRM(func) CALL_MEDIA(AMediaDrm_##func)
#define CALL_MEDIA_EXTRACTOR(func) CALL_MEDIA(AMediaExtractor_##func)
#define CALL_MEDIA_FORMAT(func) CALL_MEDIA(AMediaFormat_##func)
#define CALL_MEDIA_MUXER(func) CALL_MEDIA(AMediaMuxer_##func)

const char* getMediaErrorStr(media_status_t status);

#endif
