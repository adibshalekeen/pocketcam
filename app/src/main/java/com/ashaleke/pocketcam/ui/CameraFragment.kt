package com.ashaleke.pocketcam.ui

import android.os.Bundle
import android.util.Log
import androidx.fragment.app.Fragment
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import com.ashaleke.pocketcam.R
import com.ashaleke.pocketcam.databinding.CameraFragmentBinding
import com.ashaleke.pocketcam.ui.listeners.CameraSurfaceAvailableListener

/**
 * Fragment holding all camera view related UI components
 */
class CameraFragment(var cameraSurfaceListener : CameraSurfaceAvailableListener) : Fragment(R.layout.camera_fragment) {

    private lateinit var binding : CameraFragmentBinding

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        binding = CameraFragmentBinding.inflate(inflater)
        binding.cameraView.setCameraSurfaceAvailableListener(cameraSurfaceListener)
        // Inflate the layout for this fragment
        return binding.root
    }

    fun setBufferSize(width : Int, height: Int) {
        Log.e("PocketCam", "SETTING BUFFER SIZE ${width} ${height}")
        binding.cameraView.setBufferSize(width, height)
    }
}