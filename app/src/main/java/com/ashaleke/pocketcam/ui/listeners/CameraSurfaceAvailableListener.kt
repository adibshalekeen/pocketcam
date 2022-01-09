package com.ashaleke.pocketcam.ui.listeners

import android.view.Surface

interface CameraSurfaceAvailableListener {
    fun created(surface : Surface)
    fun resized(width : Int, height : Int)
    fun destroyed()
}