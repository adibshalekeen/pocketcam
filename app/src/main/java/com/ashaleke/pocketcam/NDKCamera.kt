package com.ashaleke.pocketcam

import android.system.Int64Ref
import android.util.Size
import android.view.Surface

class NDKCamera(val displayWidth : Int, val displayHeight : Int) {
    var handle : Long? = null
    var compatibleSize : Size? = null
    init {
        handle = createCamera(displayWidth, displayHeight)
        handle?.run{
            compatibleSize = getMinimumCompatiblePreviewSize(this)
        }
    }

    fun setPreviewSurface(surface : Surface) {
        handle?.run{
            setPreviewSurface(this, surface)
        }
    }

    fun takePhoto() {
        handle?.run{
            takePhoto(this)
        }
    }

    private external fun createCamera(displayWidth : Int, displayHeight: Int) : Long
    private external fun deleteCamera(handle : Long)
    private external fun setPreviewSurface(handle: Long, surface : Surface)
    private external fun takePhoto(handle : Long)
    private external fun getMinimumCompatiblePreviewSize(handle : Long) : Size
    private external fun getCameraSensorOrientation(handle: Long) : Int
}