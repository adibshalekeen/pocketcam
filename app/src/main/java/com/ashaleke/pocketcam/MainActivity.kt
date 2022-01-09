package com.ashaleke.pocketcam

import android.Manifest
import android.content.Context
import android.content.pm.PackageManager
import android.hardware.camera2.CameraManager
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.util.Log
import android.view.Display
import android.view.Surface
import androidx.core.app.ActivityCompat
import com.ashaleke.pocketcam.databinding.ActivityMainBinding
import com.ashaleke.pocketcam.ui.CameraFragment
import com.ashaleke.pocketcam.ui.listeners.CameraSurfaceAvailableListener

class MainActivity : AppCompatActivity(), ActivityCompat.OnRequestPermissionsResultCallback {

    init {
        System.loadLibrary("pocketcam")
    }

    private val CAMERA_PERMISSION_REQ_CODE : Int = 1
    private lateinit var binding : ActivityMainBinding

    private lateinit var ndkCameraManager : NDKCameraManager
    private lateinit var cameraFragment : CameraFragment
    private lateinit var cameraManager : CameraManager

    var cameraSurfaceListener : CameraSurfaceAvailableListener =
        object : CameraSurfaceAvailableListener {
            override fun created(surface : Surface) {
                Log.e(Constants.APP_TAG, "Creating cam")
                val display : Display = windowManager.defaultDisplay
                Log.e(Constants.APP_TAG, "H ${display.mode.physicalHeight} W ${display.mode.physicalWidth}")
                ndkCameraManager = NDKCameraManager(cameraManager,
                                                    display.mode.physicalHeight,
                                                    display.mode.physicalWidth)
                ndkCameraManager.cameraPreviewSize?.run{
                    cameraFragment.setBufferSize(this.width, this.height)
                    ndkCameraManager.surface = surface
                }
            }

            override fun resized(width: Int, height: Int) {

            }

            override fun destroyed() {
            }
        }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        cameraManager = getSystemService(Context.CAMERA_SERVICE) as CameraManager
        if(NDKCameraManager.camera2Available(cameraManager)) {
            requestCamera()
        }
        else{
            System.exit(-1)
        }
    }

    override fun onRequestPermissionsResult(
        requestCode: Int,
        permissions: Array<out String>,
        grantResults: IntArray
    )
    {
        if(requestCode != CAMERA_PERMISSION_REQ_CODE)
        {
            super.onRequestPermissionsResult(requestCode, permissions, grantResults)
            return
        }

        if(grantResults.isNotEmpty() && grantResults[0] == PackageManager.PERMISSION_GRANTED)
        {
            requestCamera()
        }
    }

    fun requestCamera() {
        if(ActivityCompat.checkSelfPermission(this, Manifest.permission.CAMERA) !=
            PackageManager.PERMISSION_GRANTED) {
            requestPermissions(arrayOf(Manifest.permission.CAMERA), CAMERA_PERMISSION_REQ_CODE)
        }
        else
        {
            initialize()
        }
    }

    fun initialize() {
        runOnUiThread(Runnable {
            cameraFragment = CameraFragment(cameraSurfaceListener)
            val transaction = supportFragmentManager.beginTransaction()
            transaction.add(R.id.preview_layout, cameraFragment)
            transaction.commit()
        })
    }
}