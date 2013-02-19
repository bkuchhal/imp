# Autogenerated file, run build/tools/setup_cmake.py to regenerate

INCLUDE (CheckCXXSourceCompiles)

CHECK_CXX_SOURCE_COMPILES("/**
 *  ile nothing.cpp
 *  rief nothing
 *
 *  Copyright 2007-2013 IMP Inventors. All rights reserved.
 *
 */

#include <debug/vector>
int main(void)
{ return 0; }

"
 IMP_COMPILER_HAS_DEBUG_VECTOR)

if(${IMP_COMPILER_HAS_DEBUG_VECTOR} MATCHES 1)
set(IMP_COMPILER_HAS_DEBUG_VECTOR 1 CACHE INTERNAL "" FORCE )
else()
set(IMP_COMPILER_HAS_DEBUG_VECTOR 0 CACHE INTERNAL "" FORCE)
endif()