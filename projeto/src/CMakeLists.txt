cmake_minimum_required(VERSION 3.10)
project(VI-RT)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set optimization flags
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -fopenmp -lglfw3 -lgdi  -lopengl32 -lGLU -lGL -lXrandr -lXxf86vm -lXi -lXinerama -lX11 -lrt -ldl")

# Add header files
set(HEADERS
    utils/RGB.hpp
    utils/vector.hpp
    utils/customValues.hpp
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
    Window/Window.hpp
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
    utils/customValues.cpp
    Window/Window.cpp
)
# Group source and header files by folder for Visual Studio
foreach(file ${SOURCES} ${HEADERS})
    get_filename_component(file_path "${file}" PATH)
    string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}" "" file_path_group "${file_path}")
    string(REPLACE "/" "\\" file_path_group "${file_path_group}")
    source_group("${file_path_group}" FILES "${file}")
endforeach()

find_package(OpenGL REQUIRED)
if(NOT OPENGL_FOUND)
    message(ERROR " OPENGL not found!")
endif(NOT OPENGL_FOUND)

#include_directories(${OpenGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} ${DevIL_INCLUDE_DIRS} ${GLFW_INCLUDE_DIRS})
#link_directories(${OpenGL_LIBRARY_DIRS})
#add_definitions(${OpenGL_DEFINITIONS})
# Add executable




message(STATUS "Toolkits_DIR set to: " ${TOOLKITS_FOLDER})
set(TOOLKITS_FOLDER "" CACHE PATH "Path to Toolkits folder")



if (NOT EXISTS "${TOOLKITS_FOLDER}/glfw/GLFW/glfw3.h" OR NOT EXISTS "${TOOLKITS_FOLDER}/glfw/glfw3.lib")
	message(ERROR ": GLFW not found")
endif (NOT EXISTS "${TOOLKITS_FOLDER}/glfw/GLFW/glfw3.h" OR NOT EXISTS "${TOOLKITS_FOLDER}/glfw/glfw3.lib")	

include_directories(${TOOLKITS_FOLDER}/glfw)

link_directories(${TOOLKITS_FOLDER}/glfw)

add_definitions(${GLFW_DEFINITIONS})


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
    ${CMAKE_CURRENT_SOURCE_DIR}/Window
    
)


target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} 
                                      ${TOOLKITS_FOLDER}/glfw/glfw3.lib)

