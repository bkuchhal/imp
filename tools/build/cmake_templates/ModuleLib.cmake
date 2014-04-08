# Autogenerated file, run tools/build/setup_cmake.py to regenerate

FILE(GLOB gensources
     "${CMAKE_BINARY_DIR}/src/%(name)s/*.cpp")

FILE(GLOB genheaders
     "${CMAKE_BINARY_DIR}/include/IMP/%(name)s/*.h")

include_directories(%(includepath)s)
link_directories(%(libpath)s)
add_definitions("-DIMP%(CPPNAME)s_EXPORTS")

include(Files.cmake)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${%(NAME)s_CXX_FLAGS}")

set(headers %(headers)s)

if(DEFINED IMP_%(name)s_LIBRARY_EXTRA_SOURCES)
  set_source_files_properties(${IMP_%(name)s_LIBRARY_EXTRA_SOURCES}
                              PROPERTIES GENERATED 1)
endif()
set_source_files_properties(${CMAKE_BINARY_DIR}/src/%(name)s_config.cpp
  PROPERTIES GENERATED 1)

if(DEFINED IMP_%(name)s_IS_PER_CPP)
  set(sources ${cppfiles})
  add_library(IMP.%(name)s-lib  ${IMP_LIB_TYPE} ${gensources} ${genheaders}
              ${headers} ${sources}
              ${CMAKE_BINARY_DIR}/src/%(name)s_config.cpp
              ${IMP_%(name)s_LIBRARY_EXTRA_SOURCES}
              )
else()
  add_library(IMP.%(name)s-lib  ${IMP_LIB_TYPE} ${gensources} ${genheaders}
              ${headers} ${CMAKE_BINARY_DIR}/src/%(name)s_all.cpp
              ${CMAKE_BINARY_DIR}/src/%(name)s_config.cpp
              ${IMP_%(name)s_LIBRARY_EXTRA_SOURCES}
              )
endif()
set_target_properties(IMP.%(name)s-lib PROPERTIES
                      OUTPUT_NAME imp_%(name)s)
set_property(TARGET "IMP.%(name)s-lib" PROPERTY FOLDER "IMP.%(name)s")

INSTALL(TARGETS IMP.%(name)s-lib DESTINATION ${CMAKE_INSTALL_LIBDIR})

add_dependencies(IMP.%(name)s-lib IMP.%(name)s-version)

if(DEFINED IMP_%(name)s_LIBRARY_EXTRA_DEPENDENCIES)
  add_dependencies(IMP.%(name)s-lib ${IMP_%(name)s_LIBRARY_EXTRA_DEPENDENCIES})
endif()

target_link_libraries(IMP.%(name)s-lib ${imp_%(name)s_libs})

set(IMP_%(name)s_LIBRARY IMP.%(name)s-lib CACHE INTERNAL "" FORCE)
