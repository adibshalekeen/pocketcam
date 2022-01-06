package com.ashaleke.pocketcam.ui

import android.content.Context
import android.opengl.GLSurfaceView
import android.util.AttributeSet
import android.view.TextureView
import com.ashaleke.pocketcam.ui.listeners.CameraSurfaceAvailableListener

class CameraView(context: Context?,
                 attrs: AttributeSet?) : GLSurfaceView(context, attrs) {
    var surfaceAvailableListener: TextureView.SurfaceTextureListener? = null
    var surfaceAvailable : Boolean = false

    val cameraSurfaceAvailableListener : CameraSurfaceAvailableListener =
        object : CameraSurfaceAvailableListener {
            override fun onSurfaceAvailable() {
                surfaceAvailable = true
                notifySurfaceAvailable()
            }
        }

    var cameraRenderer : CameraRenderer = CameraRenderer(cameraSurfaceAvailableListener)

    init {
        setEGLContextClientVersion(3)
        setRenderer(cameraRenderer)
    }

    fun changeResolution()
    {
        cameraRenderer.invalidateTextureMatrix()
    }

    fun setOnSurfaceAvailableListener(listener : TextureView.SurfaceTextureListener)
    {
        surfaceAvailableListener = listener
        if(surfaceAvailable)
        {
            notifySurfaceAvailable()
        }
    }

    private fun notifySurfaceAvailable()
    {
        surfaceAvailableListener?.onSurfaceTextureAvailable(cameraRenderer.surfaceTexture,
            getWidth(),
            getHeight())
    }
}