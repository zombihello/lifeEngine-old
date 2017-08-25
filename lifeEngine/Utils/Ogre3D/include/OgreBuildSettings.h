#ifndef __Custom_Config_H_
#define __Custom_Config_H_

// CMake auto-generated configuration options

/* #undef OGRE_STATIC_LIB */
#define OGRE_DEBUG_MODE 0
#define OGRE_RESOURCEMANAGER_STRICT 0

/* #undef OGRE_BUILD_RENDERSYSTEM_D3D9 */
/* #undef OGRE_BUILD_RENDERSYSTEM_D3D11 */
#define OGRE_BUILD_RENDERSYSTEM_GL
#define OGRE_BUILD_RENDERSYSTEM_GL3PLUS
/* #undef OGRE_BUILD_RENDERSYSTEM_GLES */
/* #undef OGRE_BUILD_RENDERSYSTEM_GLES2 */
#define OGRE_BUILD_PLUGIN_BSP
#define OGRE_BUILD_PLUGIN_OCTREE
#define OGRE_BUILD_PLUGIN_PCZ
#define OGRE_BUILD_PLUGIN_PFX
/* #undef OGRE_BUILD_PLUGIN_CG */
#define OGRE_BUILD_COMPONENT_PAGING
#define OGRE_BUILD_COMPONENT_MESHLODGENERATOR
#define OGRE_BUILD_COMPONENT_TERRAIN
#define OGRE_BUILD_COMPONENT_VOLUME
#define OGRE_BUILD_COMPONENT_PROPERTY
#define OGRE_BUILD_COMPONENT_OVERLAY
#define OGRE_BUILD_COMPONENT_RTSHADERSYSTEM
#define OGRE_BUILD_COMPONENT_HLMS

#define OGRE_CONFIG_LITTLE_ENDIAN
/* #undef OGRE_CONFIG_BIG_ENDIAN */

#define OGRE_USE_STD11 0
#define OGRE_NODE_STORAGE_LEGACY 1

/** If set to 1, Real is typedef'ed to double. Otherwise, Real is typedef'ed
    to float. Setting this allows you to perform mathematical operations in the
    CPU (Quaternion, Vector3 etc) with more precision, but bear in mind that the
    GPU still operates in single-precision mode.
*/
#define OGRE_DOUBLE_PRECISION 0

#define OGRE_NODE_INHERIT_TRANSFORM 0

#define OGRE_MEMORY_ALLOCATOR 1

// Whether to use the custom memory allocator in STL containers
#define OGRE_CONTAINERS_USE_CUSTOM_MEMORY_ALLOCATOR 0

// if you want to make Ogre::String use the custom memory allocator
// Doing this will mean Ogre's strings will not be compatible with std::string however
#define OGRE_STRING_USE_CUSTOM_MEMORY_ALLOCATOR 0

// enable or disable the memory tracker, recording the memory allocations & tracking leaks
// default is to disable since it's expensive, but you can enable if needed per build target
#define OGRE_MEMORY_TRACKER_DEBUG_MODE 0

#define OGRE_MEMORY_TRACKER_RELEASE_MODE 0

/** There are three modes for handling asserts in OGRE:
0 - STANDARD - Standard asserts in debug builds, nothing in release builds
1 - RELEASE_EXCEPTIONS - Standard asserts in debug builds, exceptions in release builds
2 - EXCEPTIONS - Exceptions in debug builds, exceptions in release builds
*/
#define OGRE_ASSERT_MODE 1

/** Support for multithreading, there are 3 options

OGRE_THREAD_SUPPORT = 0
    No support for threading.       
OGRE_THREAD_SUPPORT = 1
    Thread support for background loading, by both loading and constructing resources
    in a background thread. Resource management and SharedPtr handling becomes
    thread-safe, and resources may be completely loaded in the background. 
    The places where threading is available are clearly
    marked, you should assume state is NOT thread safe unless otherwise
    stated in relation to this flag.
OGRE_THREAD_SUPPORT = 2
    Thread support for background resource preparation. This means that resource
    data can streamed into memory in the background, but the final resource
    construction (including RenderSystem dependencies) is still done in the primary
    thread. Has a lower synchronisation primitive overhead than full threading
    while still allowing the major blocking aspects of resource management (I/O)
    to be done in the background.
OGRE_THREAD_SUPPORT = 3
    No synchronisation primitive overhead. WorkQueue is threaded.
*/
#define OGRE_THREAD_SUPPORT 0

/** Provider for threading functionality, there are 4 options.

OGRE_THREAD_PROVIDER = 0
    No support for threading.
OGRE_THREAD_PROVIDER = 1
    Boost libraries provide threading functionality.
OGRE_THREAD_PROVIDER = 2
    Poco libraries provide threading functionality.
OGRE_THREAD_PROVIDER = 3
    TBB library provides threading functionality.
OGRE_THREAD_PROVIDER = 4
    Standard library provides threading functionality. (requires compiler support)
*/
#define OGRE_THREAD_PROVIDER 0

#define OGRE_NO_MESHLOD 0

/** Disables use of the FreeImage image library for loading images. */
#define OGRE_NO_FREEIMAGE 1

/** Disables use of the internal image codec for loading DDS files. */
#define OGRE_NO_DDS_CODEC 0

/** Disables use of the internal image codec for loading PVRTC files. */
#define OGRE_NO_PVRTC_CODEC 1

/** Disables use of the internal image codec for loading ETC files. */
#define OGRE_NO_ETC_CODEC 0

/** Disables use of the internal image codec for loading image files. */
#define OGRE_NO_STBI_CODEC 0

/** Disables use of the ZIP archive support.
WARNING: Disabling this will make the samples unusable.
*/
#define OGRE_NO_ZIP_ARCHIVE 0

#define OGRE_NO_VIEWPORT_ORIENTATIONMODE 1

#define OGRE_NO_GLES2_CG_SUPPORT 1

#define OGRE_NO_GLES2_GLSL_OPTIMISER 1

#define OGRE_NO_GL_STATE_CACHE_SUPPORT 1

#define OGRE_NO_GLES3_SUPPORT 1

#define OGRE_NO_TBB_SCHEDULER 0

/** If set to 1, profiling code will be included in the application. When you
    are deploying your application you will probably want to set this to 0 */
#define OGRE_PROFILING 0

#define OGRE_NO_QUAD_BUFFER_STEREO 1

#define OGRE_BITES_HAVE_SDL 1

#define RTSHADER_SYSTEM_BUILD_CORE_SHADERS

#define RTSHADER_SYSTEM_BUILD_EXT_SHADERS

#endif
