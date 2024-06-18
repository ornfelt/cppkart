cmake_minimum_required(VERSION 3.22)
project(cppkart)

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)
set(CMAKE_CXX_EXTENSIONS ON)

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /MT")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /MTd")

# Specify the path to Bullet
set(BULLET_ROOT "C:/local/bin/bullet3-3.25")
set(BULLET_INCLUDE_DIR "${BULLET_ROOT}/src")
#set(BULLET_LIB_DIR "${BULLET_ROOT}/build/lib/Release")
set(BULLET_LIB_DIR "${BULLET_ROOT}/build/lib/Debug")

# Manually specify Bullet libraries
find_library(BULLET_DYNAMICS_LIBRARY NAMES BulletDynamics PATHS ${BULLET_LIB_DIR})
find_library(BULLET_COLLISION_LIBRARY NAMES BulletCollision PATHS ${BULLET_LIB_DIR})
find_library(BULLET_MATH_LIBRARY NAMES LinearMath PATHS ${BULLET_LIB_DIR})
find_library(BULLET_SOFTBODY_LIBRARY NAMES BulletSoftBody PATHS ${BULLET_LIB_DIR})

# Verify that all libraries were found
if(NOT BULLET_DYNAMICS_LIBRARY OR NOT BULLET_COLLISION_LIBRARY OR NOT BULLET_MATH_LIBRARY OR NOT BULLET_SOFTBODY_LIBRARY)
  message(FATAL_ERROR "Could NOT find all Bullet libraries")
endif()

# Add the vcpkg toolchain file
# One could also run cmake with -DCMAKE_TOOLCHAIN_FILE=C:/local/bin/vcpkg/scripts/buildsystems/vcpkg.cmake
if(DEFINED ENV{VCPKG_ROOT})
	MESSAGE("vcpkg root found")
    #set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
	# Include instead since CMAKE_TOOLCHAIN_FILE might be defined...
	include("$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
elseif(EXISTS "${CMAKE_SOURCE_DIR}/vcpkg.json")
    set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/vcpkg.json/scripts/buildsystems/vcpkg.cmake")
endif()

if(DEFINED ENV{SDL2_MIXER_ROOT})
    set(SDL2_MIXER_INCLUDE_DIR "$ENV{SDL2_MIXER_ROOT}/include")
    set(SDL2_MIXER_LIBRARY "$ENV{SDL2_MIXER_ROOT}/lib/x64/SDL2_mixer.lib")
    set(SDL2_MIXER_DLL "$ENV{SDL2_MIXER_ROOT}/lib/x64/SDL2_mixer.dll")
else()
    message(FATAL_ERROR "SDL2_MIXER_ROOT environment variable is not set")
endif()

if(DEFINED ENV{SDL2_DIR})
    set(SDL2_DLL "$ENV{SDL2_DIR}/lib/x64/SDL2.dll")
else()
    message(FATAL_ERROR "SDL2_DIR environment variable is not set")
endif()

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Bullet REQUIRED)
find_package(assimp REQUIRED)

include_directories(${BULLET_INCLUDE_DIR})
#include_directories(${ASSIMP_INCLUDE_DIRS})

# External Source Libraries

include_directories(src/glad/include src ${BULLET_INCLUDE_DIRS} src/imgui src/imgui/backends ${SDL2_MIXER_INCLUDE_DIR})

# --- Allows CMake to know all Imgui source files & backend impls I'm using
file(GLOB IMGUI_SOURCES 
     src/imgui/*.cpp 
     src/imgui/backends/imgui_impl_sdl2.cpp 
     src/imgui/backends/imgui_impl_opengl3.cpp)

add_executable(cppkart
     ${IMGUI_SOURCES}
     src/glad/src/glad.c
     src/stb/stb_image.h
     src/stb/stb.cpp

#    ######## Project Source Code: ########

      src/main.cpp 

     ##### Game:

      src/game/GameWindow.cpp
      src/game/GameWindow.hpp

      src/game/JXGame.cpp
      src/game/JXGame.hpp

     src/game/state/GameInput.cpp
     src/game/state/GameInput.hpp

     src/game/debug/GameDebugHelper.cpp
     src/game/debug/GameDebugHelper.hpp

     ##### Core:

          # Rendering:
          src/core/gl/EBO.cpp 
          src/core/gl/VAO.cpp 
          src/core/gl/VBO.cpp 
          src/core/gl/shaderClass.cpp
          src/core/gl/Camera.cpp

          src/core/gl/Geometry.cpp
          src/core/gl/Geometry.hpp

          src/core/gl/Texture.h
          src/core/gl/Texture.cpp

          src/core/gl/BulletDebugDrawer.cpp

          # Loaders:

          src/core/loaders/ObjModel.cpp
          src/core/loaders/ObjModel.hpp

          src/core/loaders/ChunkedMapLoader.cpp
          src/core/loaders/ChunkedMapLoader.hpp

          src/core/ID.hpp
          src/core/ID.cpp

      ##### Engine:

          # Objects:
          src/engine/objects/SimulationObject.cpp
          src/engine/objects/SimulationObject.hpp

          src/engine/objects/VehicleObject.cpp
          src/engine/objects/VehicleObject.hpp

          # Physics:
          src/engine/physics/PhysicsWorldSingleton.cpp
          src/engine/physics/PhysicsWorldSingleton.h

          src/engine/physics/VehiclePhysics.cpp
          src/engine/physics/VehiclePhysics.hpp

          src/engine/physics/StaticTriangleMeshPhysics.cpp
          src/engine/physics/StaticTriangleMeshPhysics.hpp

          # Physics Managers (Controllers):
          src/engine/physics/managers/PhysicsChunkManager.cpp 
          src/engine/physics/managers/PhysicsChunkManager.hpp

          # Rendering:
          src/engine/rendering/RenderRsrcManager.hpp
          src/engine/rendering/RenderRsrcManager.cpp

          src/engine/rendering/GameRenderer.cpp          
          src/engine/rendering/GameRenderer.hpp

          src/engine/rendering/ObjectRenderer.cpp
          src/engine/rendering/ObjectRenderer.hpp

          src/engine/rendering/GLRenderer.cpp
          src/engine/rendering/GLRenderer.hpp

          # State:
          src/engine/state/GameInputState.hpp
     
     src/engine/SimulationWorld.cpp
     src/engine/SimulationWorld.hpp
)

# Ensure all targets use the /MT runtime library
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
foreach(target ${PROJECT_NAME})
    set_target_properties(${target} PROPERTIES
        MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endforeach()

# Same as Properties -> linker -> system -> Windows
if (WIN32)
    target_link_options(cppkart PRIVATE /SUBSYSTEM:WINDOWS /ENTRY:WinMainCRTStartup)
endif()

target_compile_options(cppkart PRIVATE -fsanitize=address)
target_link_libraries(cppkart PRIVATE -fsanitize=address)

#target_link_libraries(cppkart PRIVATE SDL2_mixer SDL2::SDL2 OpenGL::GL ${BULLET_LIBRARIES} assimp::assimp)
target_link_libraries(cppkart PRIVATE SDL2::SDL2 ${SDL2_MIXER_LIBRARY} OpenGL::GL ${BULLET_DYNAMICS_LIBRARY} ${BULLET_COLLISION_LIBRARY} ${BULLET_MATH_LIBRARY} ${BULLET_SOFTBODY_LIBRARY} assimp::assimp)

# Copy the SDL2_mixer DLL to the build directory
add_custom_command(TARGET cppkart POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${SDL2_MIXER_DLL}"
    $<TARGET_FILE_DIR:cppkart>)

# Copy the SDL2 DLL to the build directory
add_custom_command(TARGET cppkart POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${SDL2_DLL}"
    $<TARGET_FILE_DIR:cppkart>)

# Copy the resources directory to the target directory
add_custom_command(TARGET cppkart POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/src/ressources"
    #"$<TARGET_FILE_DIR:cppkart>/../src/resources")
	"$<TARGET_FILE_DIR:cppkart>/resources")
