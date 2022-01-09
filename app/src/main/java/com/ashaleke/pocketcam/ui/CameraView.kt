package com.ashaleke.pocketcam.ui

import android.content.Context
import android.opengl.GLSurfaceView
import android.util.AttributeSet
import android.view.TextureView
import com.ashaleke.pocketcam.ui.listeners.CameraSurfaceAvailableListener

class CameraView(context: Context?,
                 attrs: AttributeSet?) : GLSurfaceView(context, attrs) {
    var cameraRenderer : CameraRenderer = CameraRenderer()

    init {
        setEGLContextClientVersion(3)
        setRenderer(cameraRenderer)
    }

    fun changeResolution()
    {
        cameraRenderer.invalidateTextureMatrix()
    }

    fun setCameraSurfaceAvailableListener(listener : CameraSurfaceAvailableListener) {
        cameraRenderer.cameraSurfaceAvailableListener = listener
    }

    fun setBufferSize(width : Int, height : Int) {
        cameraRenderer.surfaceTexture.setDefaultBufferSize(width, height);
    }
}