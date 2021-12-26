package com.ashaleke.pocketcam.ui

import android.content.Context
import android.opengl.GLSurfaceView
import android.util.AttributeSet
import android.view.TextureView
import com.ashaleke.pocketcam.ui.listeners.CameraSurfaceAvailableListener

class CameraView(context: Context?,
                 attrs: AttributeSet?,
                 var surfaceAvailableListener: TextureView.SurfaceTextureListener?) : GLSurfaceView(context, attrs) {
    var surfaceAvailable : Boolean = false

    val cameraSurfaceAvailableListener : CameraSurfaceAvailableListener =
        object : CameraSurfaceAvailableListener {
            override fun onSurfaceAvailable() {
                surfaceAvailable = true
                surfaceAvailableListener?.onSurfaceTextureAvailable(cameraRenderer.surfaceTexture,
                                                                    getWidth(),
                                                                    getHeight())
            }
        }

    var cameraRenderer : CameraRenderer = CameraRenderer(cameraSurfaceAvailableListener)

    init {
        setEGLContextClientVersion(2)
        setRenderer(cameraRenderer)
    }
}