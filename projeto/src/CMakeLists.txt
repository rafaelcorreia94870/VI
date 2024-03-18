cmake_minimum_required(VERSION 3.10)
project(VI-RT)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set optimization flags
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

# Add header files
set(HEADERS
    utils/RGB.hpp
    utils/vector.hpp
    3DSortingStruct/AccelStruct.hpp
    Camera/camera.hpp
    Camera/perspective.hpp
    Image/image.hpp
    Image/imagePPM.hpp
    Light/light.hpp
    Light/AmbientLight.hpp
    Light/AreaLight.hpp
    Light/PointLight.hpp
    Primitive/BB.hpp
    Primitive/primitive.hpp
    Primitive/BRDF/BRDF.hpp
    Primitive/BRDF/Phong.hpp
    Primitive/Geometry/geometry.hpp
    Primitive/Geometry/triangle.hpp
    Primitive/Geometry/mesh.hpp
    Rays/intersection.hpp
    Rays/ray.hpp
    Renderer/renderer.hpp
    Renderer/StandardRenderer.hpp
    Scene/scene.hpp
    Scene/tinyobjloader/tiny_obj_loader.h
    Shader/shader.hpp
    Shader/AmbientShader.hpp
    Shader/DistributedShader.hpp
    Shader/PathTracerShader.hpp
    Shader/WhittedShader.hpp
)

# Add source files
set(SOURCES
    main.cpp
    Camera/perspective.cpp
    Image/imagePPM.cpp
    Primitive/BRDF/Phong.cpp
    Primitive/Geometry/triangle.cpp
    Primitive/Geometry/mesh.cpp
    Renderer/StandardRenderer.cpp
    Scene/scene.cpp
    Shader/AmbientShader.cpp
    Shader/DistributedShader.cpp
    Shader/PathTracerShader.cpp
    Shader/WhittedShader.cpp
)
# Group source and header files by folder for Visual Studio
foreach(file ${SOURCES} ${HEADERS})
    get_filename_component(file_path "${file}" PATH)
    string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}" "" file_path_group "${file_path}")
    string(REPLACE "/" "\\" file_path_group "${file_path_group}")
    source_group("${file_path_group}" FILES "${file}")
endforeach()


# Add executable
add_executable(VI-RT ${SOURCES} ${HEADERS})

# Add include directories for the target
target_include_directories(VI-RT PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Camera
    ${CMAKE_CURRENT_SOURCE_DIR}/Image
    ${CMAKE_CURRENT_SOURCE_DIR}/Light
    ${CMAKE_CURRENT_SOURCE_DIR}/Primitive
    ${CMAKE_CURRENT_SOURCE_DIR}/Primitive/BRDF
    ${CMAKE_CURRENT_SOURCE_DIR}/Primitive/Geometry
    ${CMAKE_CURRENT_SOURCE_DIR}/Rays
    ${CMAKE_CURRENT_SOURCE_DIR}/Renderer
    ${CMAKE_CURRENT_SOURCE_DIR}/Scene
    ${CMAKE_CURRENT_SOURCE_DIR}/Shader
    ${CMAKE_CURRENT_SOURCE_DIR}/utils
    ${CMAKE_CURRENT_SOURCE_DIR}/3DSortingStruct
    ${CMAKE_CURRENT_SOURCE_DIR}/Scene/tinyobjloader
)