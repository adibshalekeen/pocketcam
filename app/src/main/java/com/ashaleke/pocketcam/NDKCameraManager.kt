package com.ashaleke.pocketcam

import android.hardware.camera2.CameraCharacteristics
import android.hardware.camera2.CameraManager
import android.util.Size
import android.view.Surface

class NDKCameraManager(val cameraManager : CameraManager,
                       val width : Int,
                       val height : Int) {
    companion object {
        fun camera2Available(camManager : CameraManager) : Boolean {
            var camera2ApiAvailable : Boolean = true
            val cameraIds : Array<String> = camManager.cameraIdList
            if(cameraIds.isNotEmpty())
            {
                cameraIds.forEach {
                    val characteristics : CameraCharacteristics =
                        camManager.getCameraCharacteristics(it)
                    val  deviceLevel : Int? =
                        characteristics.get(CameraCharacteristics.INFO_SUPPORTED_HARDWARE_LEVEL)
                    if(deviceLevel == CameraCharacteristics.INFO_SUPPORTED_HARDWARE_LEVEL_LEGACY)
                    {
                        camera2ApiAvailable = false
                    }
                }
            }
            return camera2ApiAvailable
        }
    }

    var camera : NDKCamera = NDKCamera(width, height)
    var cameraPreviewSize : Size? = camera.compatibleSize

    var surface : Surface? = null
        set(view) {
            field = view?.let{
                camera.setPreviewSurface(it)
                it
            }
        }

    fun takePhoto() {
        camera.takePhoto()
    }
}