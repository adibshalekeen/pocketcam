package com.ashaleke.pocketcam

import android.Manifest
import android.content.Context
import android.content.pm.PackageManager
import android.content.res.Configuration
import android.hardware.camera2.CameraManager
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.util.Log
import android.view.Display
import android.view.Surface
import android.view.View
import android.view.WindowMetrics
import android.widget.RelativeLayout
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

    private lateinit var cameraFragment : CameraFragment
    private lateinit var cameraManager : CameraManager

    private var ndkCameraManager : NDKCameraManager? = null
    private var screenOrientation : Int? = null

    var cameraSurfaceListener : CameraSurfaceAvailableListener =
        object : CameraSurfaceAvailableListener {
            override fun created(surface : Surface) {
                val display : Display = windowManager.defaultDisplay
                Log.e(Constants.APP_TAG, "Camera Surface Available Listener")
                ndkCameraManager = NDKCameraManager(cameraManager,
                                                    display.mode.physicalHeight,
                                                    display.mode.physicalWidth)
                ndkCameraManager?.cameraPreviewSize?.run{
                    ndkCameraManager?.surface = surface
                    cameraFragment.setBufferSize(this.width, this.height)
                    resizeView(this.width, this.height, screenOrientation)
                }
                val captureListener = View.OnClickListener {
                    ndkCameraManager?.takePhoto()
                }
                cameraFragment.setOnCaptureButtonClicked(captureListener)
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
        screenOrientation = resources.configuration.orientation

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

    override fun onConfigurationChanged(newConfig: Configuration) {
        screenOrientation = newConfig.orientation
        ndkCameraManager?.cameraPreviewSize?.run{
            resizeView(this.width, this.height, screenOrientation)
        }
        super.onConfigurationChanged(newConfig)
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

    fun resizeView(width : Int, height : Int, orientation : Int?){
        if(orientation == null){
            return
        }

        var windowMetrics : WindowMetrics = windowManager.currentWindowMetrics
        var displayWidth = windowMetrics.bounds.width()
        var displayHeight = windowMetrics.bounds.height()

        var params : RelativeLayout.LayoutParams =
            binding.previewLayout.layoutParams as RelativeLayout.LayoutParams
        var aspectRatio : Float = 1.0f * width / height
        if(orientation == Configuration.ORIENTATION_LANDSCAPE) {
            params.height = displayHeight
            params.width = (displayHeight * aspectRatio).toInt()
        }
        else {
            params.height = (displayWidth * aspectRatio).toInt()
            params.width = displayWidth
        }
        runOnUiThread(Runnable {
            binding.previewLayout.layoutParams = params
        })
    }
}