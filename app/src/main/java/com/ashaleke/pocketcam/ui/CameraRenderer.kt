package com.ashaleke.pocketcam.ui

import android.graphics.SurfaceTexture
import android.opengl.GLES11Ext.GL_TEXTURE_EXTERNAL_OES
import android.opengl.GLES30
import android.opengl.GLSurfaceView
import android.view.Surface
import com.ashaleke.pocketcam.Constants
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

import com.ashaleke.pocketcam.ui.listeners.CameraSurfaceAvailableListener

/**
 * Renderer used to draw camera frames to CameraView GLSurfaceView
 * */
class CameraRenderer() : GLSurfaceView.Renderer {
    // Logcat tag
    val TAG = Constants.APP_TAG + ":CameraRenderer"

    // Texture drawn onto surface
    lateinit var surfaceTexture : SurfaceTexture

    // Handle to renderer surface
    lateinit var surface : Surface

    // Matrix holding transform matrix for surface texture
    var texMatrix : FloatArray = FloatArray(16)

    // Boolean field toggled when new frame is ready for drawing
    @Volatile
    var frameAvailable : Boolean = false
    @Volatile
    var textureMatrixUpdateRequired : Boolean = true

    // Thread lock protecting frameAvailable
    val FRAME_LOCK = Object()

    var surfaceTextureCreated : Boolean = false
    var cameraSurfaceAvailableListener: CameraSurfaceAvailableListener? = null
        set(listener) {
            if(surfaceTextureCreated)
            {
                listener?.created(surface)
            }
            field = listener
        }

    override fun onSurfaceCreated(p0: GL10?, p1: EGLConfig?) {
        var textures : IntArray = IntArray(1)
        GLES30.glGenTextures(1, textures, 0)
        GLES30.glBindTexture(GL_TEXTURE_EXTERNAL_OES, textures[0])

        surfaceTexture = SurfaceTexture(textures[0])
        surfaceTexture.setOnFrameAvailableListener {
            synchronized(FRAME_LOCK)
            {
                frameAvailable = true
            }
        }
        surface = Surface(surfaceTexture)

        textureMatrixUpdateRequired = true
        surfaceTextureCreated = true

        onSurfaceCreated(textures[0], surface)
        cameraSurfaceAvailableListener?.run{
            this.created(surface)
        }
    }

    override fun onSurfaceChanged(p0: GL10?, width: Int, height: Int) {
        onSurfaceChanged(width, height)
        cameraSurfaceAvailableListener?.run{
            this.resized(width, height)
        }
    }

    override fun onDrawFrame(p0: GL10?) {
        var frame : Boolean = false
        synchronized(FRAME_LOCK) {
            frame = frameAvailable
        }
        if(frame) {
            synchronized(FRAME_LOCK) {
                surfaceTexture.updateTexImage()
                setTextureMatrix()
                frameAvailable = false
            }
        }
        onDrawFrame()
    }

    fun invalidateTextureMatrix()
    {
        synchronized(FRAME_LOCK)
        {
            textureMatrixUpdateRequired = true
        }
    }

    fun setTextureMatrix()
    {
        var updateRequired : Boolean
        synchronized(FRAME_LOCK)
        {
            updateRequired = textureMatrixUpdateRequired
        }
        if(updateRequired)
        {
            surfaceTexture.getTransformMatrix(texMatrix)
            setTextureMatrix(texMatrix)
            synchronized(FRAME_LOCK)
            {
                textureMatrixUpdateRequired = false
            }
        }
    }


    external fun onSurfaceCreated(textureId: Int, surface: Surface)
    external fun onSurfaceChanged(width: Int, height: Int)
    external fun onDrawFrame()
    external fun setTextureMatrix(textureMatrix : FloatArray)
}