# ----------------------------------------------------------------------------
#   An auxiliary function to show messages:
# ----------------------------------------------------------------------------
MACRO(SHOW_CONFIG_LINE MSG_TEXT VALUE_BOOL)
	IF(${VALUE_BOOL})
		SET(VAL_TEXT "Yes")
	ELSE(${VALUE_BOOL})
		SET(VAL_TEXT " No")
	ENDIF(${VALUE_BOOL})
	MESSAGE(STATUS " ${MSG_TEXT} : ${VAL_TEXT} ${ARGV2}")
ENDMACRO(SHOW_CONFIG_LINE)

MACRO(SHOW_CONFIG_LINE_SYSTEM MSG_TEXT VALUE_BOOL)
	IF(${VALUE_BOOL})
		IF(${VALUE_BOOL}_SYSTEM)
			SET(VAL_TEXT "Yes (System)")
		ELSE(${VALUE_BOOL}_SYSTEM)
			SET(VAL_TEXT "Yes (Built-in)")
		ENDIF(${VALUE_BOOL}_SYSTEM)
	ELSE(${VALUE_BOOL})
		SET(VAL_TEXT " No")
	ENDIF(${VALUE_BOOL})
	MESSAGE(STATUS " ${MSG_TEXT} : ${VAL_TEXT} ${ARGV2}")
ENDMACRO(SHOW_CONFIG_LINE_SYSTEM)

# ----------------------------------------------------------------------------
#   Summary:
# ----------------------------------------------------------------------------
MESSAGE(STATUS "")
IF(CMAKE_COMPILER_IS_GNUCXX)
	SET(MRPT_GCC_VERSION_STR "(GCC version: ${CMAKE_MRPT_GCC_VERSION})")
ENDIF(CMAKE_COMPILER_IS_GNUCXX)

MESSAGE(STATUS "List of MRPT libs/modules to be built (and dependencies):")
MESSAGE(STATUS "-----------------------------------------------------------------")
FOREACH(_LIB ${ALL_MRPT_LIBS})
	get_property(_LIB_DEP GLOBAL PROPERTY "${_LIB}_LIB_DEPS")
	get_property(_LIB_HDRONLY GLOBAL PROPERTY "${_LIB}_LIB_IS_HEADERS_ONLY")
	# Say whether each lib is a normal or header-only lib:
	set(_LIB_TYPE "")
	IF (_LIB_HDRONLY)
		SET(_LIB_TYPE "  (header-only)")
	ENDIF(_LIB_HDRONLY)
	
	MESSAGE(STATUS "  ${_LIB} : ${_LIB_DEP} ${_LIB_TYPE}")
ENDFOREACH(_LIB)
MESSAGE(STATUS "")

MESSAGE(STATUS "+===========================================================================+")
MESSAGE(STATUS "|         Resulting configuration for ${CMAKE_MRPT_COMPLETE_NAME}                            |")
MESSAGE(STATUS "+===========================================================================+")
MESSAGE(STATUS " _________________________ PLATFORM _____________________________")
MESSAGE(STATUS " Host                        : "             ${CMAKE_HOST_SYSTEM_NAME} ${CMAKE_HOST_SYSTEM_VERSION} ${CMAKE_HOST_SYSTEM_PROCESSOR})
if(CMAKE_CROSSCOMPILING)
MESSAGE(STATUS " Target                      : "         ${CMAKE_SYSTEM_NAME} ${CMAKE_SYSTEM_VERSION} ${CMAKE_SYSTEM_PROCESSOR})
endif(CMAKE_CROSSCOMPILING)
SHOW_CONFIG_LINE("Is the system big endian?  " CMAKE_MRPT_IS_BIG_ENDIAN)
MESSAGE(STATUS " Word size (32/64 bit)       : ${CMAKE_MRPT_WORD_SIZE}")
MESSAGE(STATUS " CMake version               : " ${CMAKE_VERSION})
MESSAGE(STATUS " CMake generator             : "  ${CMAKE_GENERATOR})
MESSAGE(STATUS " CMake build tool            : " ${CMAKE_BUILD_TOOL})
if(MSVC)
	MESSAGE(STATUS " MSVC                        : " ${MSVC_VERSION})
endif(MSVC)
if(CMAKE_GENERATOR MATCHES Xcode)
	MESSAGE(STATUS " Xcode                       : " ${XCODE_VERSION})
endif(CMAKE_GENERATOR MATCHES Xcode)
if(NOT CMAKE_GENERATOR MATCHES "Xcode|Visual Studio")
	MESSAGE(STATUS " Configuration               : "  ${CMAKE_BUILD_TYPE})
endif(NOT CMAKE_GENERATOR MATCHES "Xcode|Visual Studio")

MESSAGE(STATUS "")
MESSAGE(STATUS " __________________________ OPTIONS _____________________________")
SHOW_CONFIG_LINE("Build MRPT as a shared library?  " CMAKE_MRPT_BUILD_SHARED_LIB_ONOFF)
SHOW_CONFIG_LINE("Use SSE2?                        " CMAKE_MRPT_HAS_SSE2)
SHOW_CONFIG_LINE("Use SSE3/SSSE3?                  " CMAKE_MRPT_HAS_SSE3)
IF($ENV{VERBOSE})
	SHOW_CONFIG_LINE("Additional checks even in Release  " CMAKE_MRPT_ALWAYS_CHECKS_DEBUG)
	SHOW_CONFIG_LINE("Additional matrix checks           " CMAKE_MRPT_ALWAYS_CHECKS_DEBUG_MATRICES)
ENDIF($ENV{VERBOSE})
MESSAGE(STATUS " Install prefix                    : ${CMAKE_INSTALL_PREFIX}")
MESSAGE(STATUS " C++ config header                 : ${MRPT_CONFIG_FILE_INCLUDE_DIR}")
MESSAGE(STATUS  "")

IF($ENV{VERBOSE})
	MESSAGE(STATUS " _________________________ COMPILER OPTIONS _____________________")
	message(STATUS "Compiler:                  ${CMAKE_CXX_COMPILER} ${MRPT_GCC_VERSION_STR} ")
	message(STATUS "  C++ flags (Release):       ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE}")
	message(STATUS "  C++ flags (Debug):         ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG}")
	message(STATUS "  Executable link flags (Release):    ${CMAKE_EXE_LINKER_FLAGS} ${CMAKE_EXE_LINKER_FLAGS_RELEASE}")
	message(STATUS "  Executable link flags (Debug):      ${CMAKE_EXE_LINKER_FLAGS} ${CMAKE_EXE_LINKER_FLAGS_DEBUG}")
	message(STATUS "  Lib link flags (Release):    ${CMAKE_SHARED_LINKER_FLAGS} ${CMAKE_SHARED_LINKER_FLAGS_RELEASE}")
	message(STATUS "  Lib link flags (Debug):      ${CMAKE_SHARED_LINKER_FLAGS} ${CMAKE_SHARED_LINKER_FLAGS_DEBUG}")
	MESSAGE(STATUS "")
ENDIF($ENV{VERBOSE})

MESSAGE(STATUS  " ____________________ WILL USE THESE LIBRARIES __________________")

SHOW_CONFIG_LINE_SYSTEM("Has eigen3                              " CMAKE_MRPT_HAS_EIGEN "[Version: ${PKG_EIGEN3_VERSION}]")
SHOW_CONFIG_LINE_SYSTEM("Has ffmpeg libs (Video streaming)       " CMAKE_MRPT_HAS_FFMPEG "[avcodec ${LIBAVCODEC_VERSION}, avutil ${LIBAVUTIL_VERSION}, avformat ${LIBAVFORMAT_VERSION}]")
SHOW_CONFIG_LINE_SYSTEM("Has gtest (Google unit testing library) " CMAKE_MRPT_HAS_GTEST )
SHOW_CONFIG_LINE("Has Intel threading lib (TBB)           " CMAKE_MRPT_HAS_TBB)
IF (WIN32)
SHOW_CONFIG_LINE("Has Kinect CL NUI                       " CMAKE_MRPT_HAS_CL_NUI)
ENDIF(WIN32)
SHOW_CONFIG_LINE_SYSTEM("Has lib3ds (3DStudio scenes)            " CMAKE_MRPT_HAS_LIB3DS)
SHOW_CONFIG_LINE_SYSTEM("Has libclang (for ConvertUTF)           " CMAKE_MRPT_HAS_CLANG)
SHOW_CONFIG_LINE_SYSTEM("Has libdc1394-2 (FireWire capture)      " CMAKE_MRPT_HAS_LIBDC1394_2)
SHOW_CONFIG_LINE_SYSTEM("Has libjpeg (jpeg)                      " CMAKE_MRPT_HAS_JPEG)
IF(UNIX)
SHOW_CONFIG_LINE_SYSTEM("Has libftdi (USB)                       " CMAKE_MRPT_HAS_FTDI)
ENDIF(UNIX)
SHOW_CONFIG_LINE("Has National Instruments devices?       " CMAKE_MRPT_HAS_NATIONAL_INSTRUMENTS)
IF(CMAKE_MRPT_HAS_NATIONAL_INSTRUMENTS)
	SHOW_CONFIG_LINE(" -> Has NI845x?                         " CMAKE_MRPT_HAS_NI845x)
ENDIF(CMAKE_MRPT_HAS_NATIONAL_INSTRUMENTS)
SHOW_CONFIG_LINE_SYSTEM("Has OpenCV (Image manipulation)         " CMAKE_MRPT_HAS_OPENCV "[Version: ${MRPT_OPENCV_VERSION}, Has non-free: ${CMAKE_MRPT_HAS_OPENCV_NONFREE}]")
SHOW_CONFIG_LINE_SYSTEM("Has OpenGL                              " CMAKE_MRPT_HAS_OPENGL_GLUT)
SHOW_CONFIG_LINE_SYSTEM("Has GLUT                                " CMAKE_MRPT_HAS_GLUT)
SHOW_CONFIG_LINE_SYSTEM("Has OpenKinect libfreenect              " CMAKE_MRPT_HAS_FREENECT)
SHOW_CONFIG_LINE_SYSTEM("Has PCL (Pointscloud library)           " CMAKE_MRPT_HAS_PCL  "[Version: ${PCL_VERSION}]")
IF(NOT UNIX)	# In fact, it's not "support for Bumblebee" but for it thru PGR vendor libraries.
SHOW_CONFIG_LINE("Has PGR Digiclops/Triclops              " CMAKE_MRPT_HAS_BUMBLEBEE)
ENDIF(NOT UNIX)
SHOW_CONFIG_LINE_SYSTEM("Has SuiteSparse/CHOLMOD                 " CMAKE_MRPT_HAS_CHOLMOD)
SHOW_CONFIG_LINE_SYSTEM("Has SuiteSparse/CSparse                 " CMAKE_MRPT_HAS_CSPARSE)
SHOW_CONFIG_LINE_SYSTEM("Has SwissRanger 3/4000 3D camera        " CMAKE_MRPT_HAS_SWISSRANGE )
SHOW_CONFIG_LINE_SYSTEM("Has Phidget                             " CMAKE_MRPT_HAS_PHIDGET)
SHOW_CONFIG_LINE_SYSTEM("Has Videre SVS stereo camera            " CMAKE_MRPT_HAS_SVS)
SHOW_CONFIG_LINE_SYSTEM("Has wxWidgets                           " CMAKE_MRPT_HAS_WXWIDGETS)
SHOW_CONFIG_LINE_SYSTEM("Has xSENS MTi library (inertial sensor) " CMAKE_MRPT_HAS_xSENS)
SHOW_CONFIG_LINE_SYSTEM("Has zlib (compression)                  " CMAKE_MRPT_HAS_ZLIB)

MESSAGE(STATUS  "")

# Final warnings:
IF (NOT CMAKE_MRPT_HAS_OPENCV AND NOT DISABLE_OPENCV)
	MESSAGE(STATUS "")
	MESSAGE(STATUS "***********************************************************************")
	MESSAGE(STATUS "* WARNING: It's STRONGLY recommended to build MRPT with OpenCV support.")
	MESSAGE(STATUS "*  To do so, set OpenCV_DIR to its CMake build dir. If you want to go ")
	MESSAGE(STATUS "*  on without OpenCV, proceed to build instead. ")
	MESSAGE(STATUS "***********************************************************************")
	MESSAGE(STATUS "")
ENDIF(NOT CMAKE_MRPT_HAS_OPENCV AND NOT DISABLE_OPENCV)

