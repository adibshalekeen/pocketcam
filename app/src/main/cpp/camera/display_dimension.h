//
// Created by ashal on 2022-01-12.
//

#ifndef POCKETCAM_DISPLAY_DIMENSION_H
#define POCKETCAM_DISPLAY_DIMENSION_H
/**
 * A helper class to assist image size comparison, by comparing the absolute
 * size
 * regardless of the portrait or landscape mode.
 */
 // This helper class was taken from
 // https://github.com/android/ndk-samples/tree/main/camera
class DisplayDimension {
public:
    DisplayDimension(int32_t w, int32_t h, int32_t orientation) : w_(w), h_(h), portrait_(false) {
        if (h > w) {
            // make it landscape
            w_ = h;
            h_ = w;
            _orientation = orientation;
            portrait_ = (_orientation % 180 == 0);
        }
    }
    DisplayDimension(const DisplayDimension& other) {
        w_ = other.w_;
        h_ = other.h_;
        portrait_ = other.portrait_;
    }

    DisplayDimension(void) {
        w_ = 0;
        h_ = 0;
        portrait_ = false;
    }
    DisplayDimension& operator=(const DisplayDimension& other) {
        w_ = other.w_;
        h_ = other.h_;
        portrait_ = other.portrait_;

        return (*this);
    }

    bool IsSameRatio(DisplayDimension& other) {
        return (w_ * other.h_ == h_ * other.w_);
    }
    bool operator>(DisplayDimension& other) {
        return (w_ >= other.w_ & h_ >= other.h_);
    }
    bool operator==(DisplayDimension& other) {
        return (w_ == other.w_ && h_ == other.h_ && portrait_ == other.portrait_);
    }
    DisplayDimension operator-(DisplayDimension& other) {
        DisplayDimension delta(w_ - other.w_, h_ - other.h_, _orientation);
        return delta;
    }
    void Flip(void) { portrait_ = !portrait_; }
    bool IsPortrait(void) { return portrait_; }
    int32_t width(void) { return w_; }
    int32_t height(void) { return h_; }
    int32_t org_width(void) { return (portrait_ ? h_ : w_); }
    int32_t org_height(void) { return (portrait_ ? w_ : h_); }
    int32_t getOrientation(void) { return _orientation; }

private:
    int32_t w_, h_, _orientation;
    bool portrait_;
};

#endif //POCKETCAM_DISPLAY_DIMENSION_H
