
# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.10.2)

# Declares and names the project.

project("pocketcam")

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

add_library(pocketcam
            SHARED
            ${CMAKE_CURRENT_SOURCE_DIR}/camera/ndk_jni.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/camera/camera_manager.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/camera/camera_engine.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/camera/camera_listeners.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/camera/common/camera_utils.cpp)

target_include_directories(pocketcam
                           PRIVATE
                           ${CMAKE_CURRENT_SOURCE_DIR}
                           ${CMAKE_CURRENT_SOURCE_DIR}/camera/
                           ${CMAKE_CURRENT_SOURCE_DIR}/camera/common/)

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library(log-lib log )
find_library(camera-lib camera2ndk)
find_library( gl-lib GLESv2 )
find_library(media-lib mediandk)
# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
        pocketcam
        # Links the target library to the log library
        # included in the NDK.
        ${log-lib}
        ${camera-lib}
        ${gl-lib}
        android
        ${media-lib}
        m)
