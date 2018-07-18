cmake_minimum_required(VERSION 3.1)
project(TestPass)
add_library(TestPass MODULE
    # List your source files here.
    controllerConversion.cpp IRssa.h utilities.h DataStructures.h optIRssa.h optDataStructures.h coreWrapperSSA.h  allStackVariables.h coreWrapperSSA.cpp  IRssa.cpp utilities.cpp allStackVariables.cpp optIRssa.cpp)

# Use C++11 to compile our pass (i.e., supply -std=c++11).
target_compile_features(TestPass PRIVATE cxx_range_for cxx_auto_type)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# LLVM is (typically) built with no C++ RTTI. We need to match that;
# otherwise, we'll get linker errors about missing RTTI data.
set_target_properties(TestPass PROPERTIES
    COMPILE_FLAGS "-fno-rtti"
)

# Get proper shared-library behavior (where symbols are not necessarily
# resolved when the shared library is linked) on OS X.
if(APPLE)
    set_target_properties(TestPass PROPERTIES
        LINK_FLAGS "-undefined dynamic_lookup"
    )
endif(APPLE)
#set( EXECUTABLE_OUTPUT_PATH "build/")
