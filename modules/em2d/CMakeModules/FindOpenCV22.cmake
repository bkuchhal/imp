# Autogenerated file, run tools/build/setup_cmake.py to regenerate

if(NOT DEFINED OpenCV22_LIBRARIES)

set(CHECK_COMPILES_BODY "new cv::Mat(); BOOST_STATIC_ASSERT(CV_MAJOR_VERSION>=2 && CV_MINOR_VERSION>=1);")

check_compiles("_environment" OpenCV22 OPENCV22 "#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/version.hpp>
#include <boost/static_assert.hpp>" "" "opencv_core opencv_imgproc opencv_highgui" OpenCV22_ok)
if("${OpenCV22_ok}" MATCHES "1")
message(STATUS "Found OpenCV22 in environment")
else()
include(LibFindMacros)

# Use pkg-config to get hints about paths
libfind_pkg_check_modules("OpenCV22_PKGCONF" "opencv22")

# Include dir
find_path("OpenCV22_INCLUDE_DIR"
  NAMES opencv2/core/core.hpp opencv2/imgproc/imgproc.hpp opencv2/highgui/highgui.hpp opencv2/core/version.hpp boost/static_assert.hpp
  PATHS ${OpenCV22_PKGCONF_INCLUDE_DIRS}
)

# Finally the library itself
foreach(lib opencv_core opencv_imgproc opencv_highgui)
find_library("${lib}_LIBRARY"
  NAMES ${lib}
  PATHS ${OpenCV22_PKGCONF_LIBRARY_DIRS}
)
set("OpenCV22_LIBRARY" ${OpenCV22_LIBRARY} ${${lib}_LIBRARY})
endforeach(lib)


# Set the include dir variables and the libraries and let libfind_process do the rest.
# NOTE: Singular variables for this library, plural for libraries this this lib depends on.
set(OpenCV22_PROCESS_INCLUDES OpenCV22_INCLUDE_DIR)
set(OpenCV22_PROCESS_LIBS OpenCV22_LIBRARY)
libfind_process(OpenCV22)

if ("${OpenCV22_LIBRARY}" MATCHES ".*NOTFOUND.*"
    OR "${OpenCV22_INCLUDE_DIR}" MATCHES ".*NOTFOUND.*")
  message("OpenCV22 not found")
file(WRITE "${PROJECT_BINARY_DIR}/data/build_info/OpenCV22" "ok=False")
else()
  check_compiles("_found" "OpenCV22" "OPENCV22" "#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/version.hpp>
#include <boost/static_assert.hpp>" "${OpenCV22_INCLUDE_DIR}" "${OpenCV22_LIBRARY}" OpenCV22_ok_ok)
  if(${OpenCV22_ok_ok} MATCHES "1")
    message(STATUS "Found OpenCV22 at ""${OpenCV22_INCLUDE_DIR}" " " "${OpenCV22_LIBRARY}")
  else()
    message("OpenCV22 not found")
file(WRITE "${PROJECT_BINARY_DIR}/data/build_info/OpenCV22" "ok=False")
  endif()
endif()
endif()
else()
message(STATUS "OpenCV22 already setup")

endif(NOT DEFINED OpenCV22_LIBRARIES)