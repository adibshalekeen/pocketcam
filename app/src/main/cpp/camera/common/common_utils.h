#ifndef POCKETCAM_COMMON_UTILS_H
#define POCKETCAM_COMMON_UTILS_H

#include <vector>
#include "native_debug.h"

#define UNKNOWN_TAG "UNKNOWN_TAG"
#define MAKE_PAIR(val) std::make_pair(val, #val)

template <typename T>
static const char* GetPairStr(T key, std::vector<std::pair<T, const char*>>& store) {
    typedef typename std::vector<std::pair<T, const char*>>::iterator iterator;
    for (iterator it = store.begin(); it != store.end(); ++it) {
        if (it->first == key) {
            return it->second;
        }
    }
    LOGW("(%#08x) : UNKNOWN_TAG for %s", key, typeid(store[0].first).name());
    return UNKNOWN_TAG;
}

#endif
