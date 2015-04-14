add_definitions(-DIMP_SWIG_WRAPPER)

include_directories(%(includepath)s)
link_directories(%(libpath)s)

GET_DIRECTORY_PROPERTY(includes INCLUDE_DIRECTORIES)

# SWIG doesn't need Python.h in its own include path
include_directories(${PYTHON_INCLUDE_DIRS})

# this is needed for some reason that I don't understand
set(swig_path %(swigpath)s)
list(APPEND swig_path ${IMP_SWIG_PATH})

file(STRINGS "${CMAKE_BINARY_DIR}/src/%(name)s_swig.deps" swigdeps)

set(wrap_py "${CMAKE_BINARY_DIR}/lib/%(subdir)s/__init__.py")
set(wrap_py_orig "${CMAKE_BINARY_DIR}/src/%(name)s_swig/%(pymod)s.py")
set(source "${CMAKE_BINARY_DIR}/src/%(name)s_swig/wrap.cpp"
                          "${CMAKE_BINARY_DIR}/src/%(name)s_swig/wrap.h")

set(PATH_ARGS )
foreach(path ${includes})
  list(APPEND PATH_ARGS "--includepath=${path}")
endforeach(path)
foreach(path ${swig_path})
  list(APPEND PATH_ARGS "--swigpath=${path}")
endforeach(path)
add_custom_command(OUTPUT ${source} ${wrap_py} ${wrap_py_orig}
   COMMAND  python
            "${CMAKE_SOURCE_DIR}/tools/build/make_swig_wrapper.py"
            "--swig=${SWIG_EXECUTABLE}"
            "--module=%(name)s"
            ${PATH_ARGS}
   DEPENDS ${swigdeps} ${IMP_%(name)s_PYTHON_EXTRA_DEPENDENCIES}
   WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
   COMMENT "Running SWIG on %(name)s")

add_library(IMP.%(name)s-python MODULE ${source})
# Apple linkers complain by default if there are undefined symbols
if(APPLE)
  set_target_properties(IMP.%(name)s-python PROPERTIES LINK_FLAGS "-flat_namespace -undefined suppress")
endif(APPLE)


set_target_properties(IMP.%(name)s-python PROPERTIES PREFIX ""
                      OUTPUT_NAME _IMP_%(name)s)
set_property(TARGET "IMP.%(name)s-python" PROPERTY FOLDER "IMP.%(name)s")


if(WIN32 AND NOT CYGWIN)
  set_target_properties(IMP.%(name)s-python PROPERTIES SUFFIX ".pyd")
endif()

target_link_libraries(IMP.%(name)s-python
    IMP.%(name)s-lib
    ${imp_%(name)s_libs}
    ${IMP_SWIG_LIBRARIES}
  )

set(IMP_%(name)s_PYTHON ${source} IMP.%(name)s-python ${wrap_py} %(other_pythons)s
                   CACHE INTERNAL "" FORCE)

INSTALL(TARGETS IMP.%(name)s-python DESTINATION ${CMAKE_INSTALL_PYTHONDIR})
# Install the original wrapper, since wrap_py is a symlink and install does
# not dereference symlinks
INSTALL(FILES ${wrap_py_orig}
        DESTINATION ${CMAKE_INSTALL_PYTHONDIR}/IMP/%(name)s
        RENAME __init__.py)
