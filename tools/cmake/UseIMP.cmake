# UseIMP.cmake can be included in a project to build additional IMP modules.
#
# Usually this file is used with the following in CMakeLists.txt:
#
#     find_package(IMP REQUIRED)
#     include(${IMP_USE_FILE})
#
# The variables used here are defined in IMPConfig.cmake, which find_package
# should pull in.

if(NOT USE_IMP_FILE_INCLUDED)
  set(USE_IMP_FILE_INCLUDED 1)

  set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${RMF_MODULES_DIR})

  include(${RMF_MODULES_DIR}/IMPExecuteProcess.cmake)
  include(${RMF_MODULES_DIR}/IMPAddTests.cmake)
  include(${RMF_MODULES_DIR}/CheckCompiles.cmake)

  set(IMP_PYTHON python CACHE INTERNAL "The Python executable that IMP itself will use at runtime (the build system scripts will always use 'python')")

endif()

# Build IMP module(s) from sources in sourcedir. This can be either a
# top-level directory containing a 'modules' subdirectory under which one
# or more modules exist in subdirectories, or sourcedir can be the module
# directory itself. In the latter case, the module name is taken to be the
# last component of the sourcedir name, but this can be overridden by
# providing the module name as an optional second argument.
function(imp_build_module sourcedir)
  if(${ARGC} GREATER 1)
    set(modname "--module_name=${ARGV1}")
  else()
    set(modname "")
  endif()

  # Use same compiler flags as IMP itself
  include(${RMF_MODULES_DIR}/IMPFindCompilerFlags.cmake)

  # Add include directories of mandatory IMP dependencies
  include_directories(SYSTEM ${Boost_INCLUDE_DIR})
  include_directories(SYSTEM ${EIGEN3_INCLUDE_DIR})

  if(NOT DEFINED PYTHON_INCLUDE_DIRS)
    execute_process(COMMAND ${IMP_PYTHON} -c "import sys; print(sys.executable)"
                    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                    OUTPUT_VARIABLE python_full_path
                    OUTPUT_STRIP_TRAILING_WHITESPACE)
    execute_process(COMMAND ${IMP_PYTHON} -c "import sys; print('%d.%d.%d' % sys.version_info[:3])"
                    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                    OUTPUT_VARIABLE python_full_version
                    OUTPUT_STRIP_TRAILING_WHITESPACE)
    string(REGEX REPLACE "^([0-9]+\\.[0-9]+).*" "\\1" python_version
           ${python_full_version})
    message(STATUS "Python binary is " ${python_full_path} " (version " ${python_full_version} ")")

    FIND_PACKAGE(PythonLibs ${python_full_version} EXACT REQUIRED)
  endif()

  if(WIN32)
    set(IMP_SWIG_LIBRARIES ${PYTHON_LIBRARIES})
  endif(WIN32)

  include(GNUInstallDirs)
  # Add extra installation locations for SWIG .i files and Python code
  # As per GNUInstallDirs.cmake, set empty values in the cache and store
  # defaults in local variables for locations defined relative to existing
  # CMAKE_INSTALL_*. This auto-updates the defaults when the existing
  # CMAKE_INSTALL_* changes.
  if(NOT DEFINED CMAKE_INSTALL_SWIGDIR)
    set(CMAKE_INSTALL_SWIGDIR "share/IMP/swig" CACHE PATH "SWIG interface files")
  endif()
  if(NOT CMAKE_INSTALL_PYTHONDIR)
    set(CMAKE_INSTALL_PYTHONDIR "" CACHE PATH "Python modules")
    set(CMAKE_INSTALL_PYTHONDIR "${CMAKE_INSTALL_LIBDIR}/python${python_version}/site-packages")
  endif()

  # Visual Studio always adds Release or Debug to binary directories
  if (${CMAKE_GENERATOR} MATCHES "Visual Studio")
    SET(IMP_BINARY_PATH_SUFFIX ${CMAKE_BUILD_TYPE})
  else()
    SET(IMP_BINARY_PATH_SUFFIX )
  endif()

  if(NOT DEFINED PATH_SEP)
    if(WIN32)
      Set(PATH_SEP ";")
    else()
      Set(PATH_SEP ":")
    endif()
  endif()
  if(NOT DEFINED SETUP_EXT)
    if(WIN32)
      Set(SETUP_EXT "bat")
    else()
      Set(SETUP_EXT "sh")
    endif()
  endif()

  if("${SETUP_EXT}" STREQUAL "sh")
    set(IMP_TEST_SETUP "${CMAKE_BINARY_DIR}/setup_environment.sh")
  else()
    # On Windows the batch file is run once to set up the test environment, not
    # per test
    set(IMP_TEST_SETUP )
  endif()

  add_custom_target("IMP-version"
                    COMMAND ${IMP_TOOLS_DIR}/build/make_version.py
                    --source=${sourcedir}
                    ${modname}
                    --build_dir="${IMP_BUILD_INFO_DIR}"
                    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                    COMMENT "Computing version number")

  imp_execute_process("setup_cmake" ${sourcedir}
                      COMMAND ${IMP_TOOLS_DIR}/build/setup_cmake.py
                      ${modname}
                      "--build_dir=${IMP_BUILD_INFO_DIR}"
                      "--tools_dir=${IMP_TOOLS_DIR}"
                      --swig_include=${IMP_SWIG_DIR}
                      --swig_include=${RMF_SWIG_DIR}
                      --include=${IMP_INCLUDE_DIR}
                      --required)

  imp_execute_process("setup" ${CMAKE_BINARY_DIR}
                      COMMAND ${IMP_TOOLS_DIR}/build/setup.py
                      ${modname}
                      "--build_dir=${IMP_BUILD_INFO_DIR}"
                      --source=${sourcedir}
                      --disabled=${IMP_DISABLED_MODULES}
                      --datapath=${IMP_DATAPATH})

  imp_execute_process("setup_all" ${CMAKE_BINARY_DIR}
                      COMMAND ${IMP_TOOLS_DIR}/build/setup_all.py
                      ${modname}
                      "--build_dir=${IMP_BUILD_INFO_DIR}"
                      --source=${sourcedir})

  # todo: fail if foo wasn't configured
  if(EXISTS "${sourcedir}/modules" AND IS_DIRECTORY "${sourcedir}/modules")
    file(STRINGS "${CMAKE_BINARY_DIR}/build_info/sorted_modules" modules)
    foreach(mod ${modules})
      if(EXISTS "${sourcedir}/modules/${mod}")
        add_subdirectory("${sourcedir}/modules/${mod}")
      endif()
    endforeach()
  else()
    include(${sourcedir}/ModuleBuild.cmake)
  endif()

  imp_execute_process("setup_swig_dependencies" ${CMAKE_BINARY_DIR}
                      COMMAND ${IMP_TOOLS_DIR}/build/setup_swig_deps.py
                      ${modname}
                      --include=${IMP_INCLUDE_DIR}
                      "--build_dir=${IMP_BUILD_INFO_DIR}"
                      "--swig=${SWIG_EXECUTABLE}")
  if(EXISTS "${sourcedir}/modules" AND IS_DIRECTORY "${sourcedir}/modules")
    foreach(mod ${modules})
      if(EXISTS "${sourcedir}/modules/${mod}/pyext")
        add_subdirectory("${sourcedir}/modules/${mod}/pyext")
      endif()
    endforeach()
  else()
    add_subdirectory("${sourcedir}/pyext")
  endif()

  list(INSERT IMP_PYTHONPATH 0 "${IMP_PYTHON_DIR}")
  list(INSERT IMP_LDPATH 0 "${IMP_LIB_DIR}")

  list(INSERT IMP_PYTHONPATH 0 "${CMAKE_BINARY_DIR}/lib")
  list(INSERT IMP_LDPATH 0 "${CMAKE_BINARY_DIR}/lib")
  list(INSERT IMP_PATH 0 "${CMAKE_BINARY_DIR}/bin")
  set(PATH_ARGS )
  foreach(path ${IMP_PYTHONPATH})
    list(APPEND PATH_ARGS "--python_path=${path}")
  endforeach(path)
  foreach(path ${IMP_LDPATH})
    list(APPEND PATH_ARGS "--ld_path=${path}")
  endforeach(path)
  foreach(path ${IMP_PATH})
    list(APPEND PATH_ARGS "--path=${path}")
  endforeach(path)

  imp_execute_process("setup_imppy" ${CMAKE_BINARY_DIR}
                      COMMAND ${IMP_TOOLS_DIR}/build/setup_imppy.py
                      "--python=${IMP_PYTHON}"
                      "--external_data=${IMP_DATA_DIR}"
                      "--precommand="
                      "--propagate=yes"
                      "--suffix=${IMP_BINARY_PATH_SUFFIX}"
                      "--output=setup_environment.${SETUP_EXT}"
                      ${PATH_ARGS})

  # Make a suitable top-level IMP package file so that both
  # 'import IMP.<our module>' and regular 'import IMP.algebra' will
  # work, searching both our build directory and the IMP Python path
  file(WRITE "${CMAKE_BINARY_DIR}/lib/IMP/__init__.py"
       "import os\n"
       "from pkgutil import extend_path\n\n"
       "# extend_path ensures that 'import IMP.foo' will search for the "
       "module 'foo'\n"
       "# both in our build directory and in the regular IMP path\n"
       "__oldpathlen = len(__path__)\n"
       "__path__ = extend_path(__path__, __name__)\n\n"
       "# If 'import IMP' pulls in *this* __init__.py, make sure we also do "
       "everything\n"
       "# in the main IMP/__init__.py (should be the next element in the "
       "path) so that\n"
       "# top-level stuff like IMP.deprecated_function is available.\n"
       "with open(os.path.join(__path__[__oldpathlen], '__init__.py')) as fh:\n"
       "    exec(fh.read())\n"
       "del __oldpathlen, extend_path\n")


endfunction()
