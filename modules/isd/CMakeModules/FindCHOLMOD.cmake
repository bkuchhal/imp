# Autogenerated file, run tools/build/setup_cmake.py to regenerate

if(NOT DEFINED CHOLMOD_LIBRARIES)

set(CHECK_COMPILES_BODY "")

check_compiles("_environment" CHOLMOD CHOLMOD "#include <ufsparse/cholmod.h>" "" "cholmod amd metis colamd ccolamd camd blas" CHOLMOD_ok)
if("${CHOLMOD_ok}" MATCHES "1")
message(STATUS "Found CHOLMOD in environment")
else()
include(LibFindMacros)

# Use pkg-config to get hints about paths
libfind_pkg_check_modules("CHOLMOD_PKGCONF" "cholmod")

# Include dir
find_path("CHOLMOD_INCLUDE_DIR"
  NAMES ufsparse/cholmod.h
  PATHS ${CHOLMOD_PKGCONF_INCLUDE_DIRS}
)

# Finally the library itself
foreach(lib cholmod amd metis colamd ccolamd camd blas)
find_library("${lib}_LIBRARY"
  NAMES ${lib}
  PATHS ${CHOLMOD_PKGCONF_LIBRARY_DIRS}
)
set("CHOLMOD_LIBRARY" ${CHOLMOD_LIBRARY} ${${lib}_LIBRARY})
endforeach(lib)


# Set the include dir variables and the libraries and let libfind_process do the rest.
# NOTE: Singular variables for this library, plural for libraries this this lib depends on.
set(CHOLMOD_PROCESS_INCLUDES CHOLMOD_INCLUDE_DIR)
set(CHOLMOD_PROCESS_LIBS CHOLMOD_LIBRARY)
libfind_process(CHOLMOD)

if ("${CHOLMOD_LIBRARY}" MATCHES ".*NOTFOUND.*"
    OR "${CHOLMOD_INCLUDE_DIR}" MATCHES ".*NOTFOUND.*")
  message("CHOLMOD not found")
file(WRITE "${PROJECT_BINARY_DIR}/data/build_info/CHOLMOD" "ok=False")
else()
  check_compiles("_found" "CHOLMOD" "CHOLMOD" "#include <ufsparse/cholmod.h>" "${CHOLMOD_INCLUDE_DIR}" "${CHOLMOD_LIBRARY}" CHOLMOD_ok_ok)
  if(${CHOLMOD_ok_ok} MATCHES "1")
    message(STATUS "Found CHOLMOD at ""${CHOLMOD_INCLUDE_DIR}" " " "${CHOLMOD_LIBRARY}")
  else()
    message("CHOLMOD not found")
file(WRITE "${PROJECT_BINARY_DIR}/data/build_info/CHOLMOD" "ok=False")
  endif()
endif()
endif()
else()
message(STATUS "CHOLMOD already setup")

endif(NOT DEFINED CHOLMOD_LIBRARIES)