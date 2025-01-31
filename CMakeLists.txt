cmake_minimum_required(VERSION 3.1)
project(tasep LANGUAGES C CXX)

# Find Python interpreter and extract version information
find_package(PythonInterp REQUIRED)
execute_process(
    COMMAND ${PYTHON_EXECUTABLE} -c "import sys; print(f'{sys.version_info.major}.{sys.version_info.minor}')"
    OUTPUT_VARIABLE PYTHON_VERSION
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Check if CONDA_PREFIX is set (meaning we're in a Conda environment)
if(DEFINED ENV{CONDA_PREFIX})
    set(CMAKE_PREFIX_PATH "$ENV{CONDA_PREFIX}/lib/python${PYTHON_VERSION}/site-packages/pybind11/share/cmake/pybind11" ${CMAKE_PREFIX_PATH})
    message(${CMAKE_PREFIX_PATH})
endif()

find_package(pybind11 REQUIRED)


# Specify the source files and create the library
add_library(
    mysim MODULE
    src/binding.cpp
    src/new.cpp
)



# Include directories
target_include_directories(mysim PRIVATE include)
target_link_libraries(mysim PRIVATE pybind11::module)
target_compile_options(mysim PRIVATE -O3 -march=native)


if(WIN32)
    set_target_properties(mysim PROPERTIES SUFFIX ".pyd")
else()
    set_target_properties(mysim PROPERTIES PREFIX "" SUFFIX ".so")
endif()


# Install the module to the Python site-packages directory dynamically
execute_process(
    COMMAND ${PYTHON_EXECUTABLE} -c "import site; print(site.getsitepackages()[0])"
    OUTPUT_VARIABLE PYTHON_SITE_PACKAGES
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

message(STATUS "Python site-packages: ${PYTHON_SITE_PACKAGES}")

install(TARGETS mysim
    LIBRARY DESTINATION ${PYTHON_SITE_PACKAGES}
)


