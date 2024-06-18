cmake_minimum_required(VERSION 3.22)
project(cppkart)

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)
set(CMAKE_CXX_EXTENSIONS ON)

if (WIN32)
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
	set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /MT")
	set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /MTd")

	# Specify path to Bullet
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
else()
	# Linux
	if(DEFINED ENV{VCPKG_ROOT})
		set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
	endif()
endif()

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
if(UNIX AND NOT APPLE)
	find_package(Bullet CONFIG REQUIRED)
else()
	find_package(Bullet REQUIRED)
endif()
find_package(assimp REQUIRED)

if (WIN32)
	include_directories(${BULLET_INCLUDE_DIR})
	#include_directories(${ASSIMP_INCLUDE_DIRS})
	
	# External Source Libraries
	include_directories(src/glad/include src ${BULLET_INCLUDE_DIRS} src/imgui src/imgui/backends ${SDL2_MIXER_INCLUDE_DIR})
	
else()
	# External Source Libraries
	include_directories(src/glad/include src ${BULLET_INCLUDE_DIRS} src/imgui src/imgui/backends)
endif()

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

     # src/game/state/GameInput.cpp
     # src/game/state/GameInput.hpp

     src/game/debug/GameDebugHelper.cpp
     src/game/debug/GameDebugHelper.hpp
     
     #!  ------------------------------ NEW FILES ------------------------------

     #---! Engine Rewrite Files !---:

     src/jx_engine/logs/Logger.hpp
     src/jx_engine/logs/Logger.cpp

     src/jx_engine/core/Scene.hpp
     src/jx_engine/core/Scene.cpp

     src/jx_engine/core/SceneManager.hpp
     src/jx_engine/core/SceneManager.cpp

     src/jx_engine/entity/ECManager.hpp
     src/jx_engine/entity/ECManager.cpp

     src/jx_engine/entity/Entity.hpp
     src/jx_engine/entity/Entity.cpp

     # Components:

     src/jx_engine/component/EComponent.hpp
     src/jx_engine/component/EComponent.cpp

     src/jx_engine/component/RenderComponent.hpp
     src/jx_engine/component/RenderComponent.cpp

     src/jx_engine/component/VehicleRenderComponent.hpp
     src/jx_engine/component/VehicleRenderComponent.cpp

     src/jx_engine/component/PlayerVehicleComponent.hpp
     src/jx_engine/component/PlayerVehicleComponent.cpp

     src/jx_engine/component/TerrainChunksComponent.hpp
     src/jx_engine/component/TerrainChunksComponent.cpp

     # I/O: 

     src/jx_engine/io/GameInput.hpp
     src/jx_engine/io/GameInput.cpp

     # Import / Loaders:

     src/jx_engine/import/ObjModel.hpp
     src/jx_engine/import/ObjModel.cpp

     src/jx_engine/import/PhysChunkedMapLoader.hpp
     src/jx_engine/import/PhysChunkedMapLoader.cpp

     # Rendering:

     src/jx_engine/render/GameGLRenderer.hpp
     src/jx_engine/render/GameGLRenderer.cpp

     src/jx_engine/render/RenderRsrcManager.hpp
     src/jx_engine/render/RenderRsrcManager.cpp

     src/jx_engine/render/gl/Geometry.cpp
     src/jx_engine/render/gl/Geometry.hpp

     src/jx_engine/render/gl/Texture.h
     src/jx_engine/render/gl/Texture.cpp

     src/jx_engine/render/gl/misc/ID.hpp
     src/jx_engine/render/gl/misc/ID.cpp

     src/jx_engine/render/gl/shaderClass.h
     src/jx_engine/render/gl/shaderClass.cpp

     src/jx_engine/render/gl/EBO.h
     src/jx_engine/render/gl/EBO.cpp

     src/jx_engine/render/gl/VBO.h
     src/jx_engine/render/gl/VBO.cpp

     src/jx_engine/render/gl/VAO.h
     src/jx_engine/render/gl/VAO.cpp

     src/jx_engine/render/gl/Camera.h
     src/jx_engine/render/gl/Camera.cpp

     # Physics:
     src/jx_engine/physics/PhysicsWorldSingleton.h
     src/jx_engine/physics/PhysicsWorldSingleton.cpp

     src/jx_engine/physics/StaticTriangleMeshPhysics.hpp
     src/jx_engine/physics/StaticTriangleMeshPhysics.cpp

     src/jx_engine/physics/BulletDebugDrawer.cpp

     src/jx_engine/physics/manager/PhysicsChunkManager.hpp
     src/jx_engine/physics/manager/PhysicsChunkManager.cpp

     src/jx_engine/physics/VehiclePhysics.hpp
     src/jx_engine/physics/VehiclePhysics.cpp

     #---! Rewrite Game Files !---:

     src/game/GameWindow.cpp
     src/game/GameWindow.hpp

     src/game/DemoGame.hpp
     src/game/DemoGame.cpp

     src/game/scenes/GameScene.hpp
     src/game/scenes/GameScene.cpp
)

if (WIN32)
	# Ensure all targets use the /MT runtime library
	set_property(GLOBAL PROPERTY USE_FOLDERS ON)
	foreach(target ${PROJECT_NAME})
		set_target_properties(${target} PROPERTIES
			MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
	endforeach()

	# Same as Properties -> linker -> system -> Windows
    target_link_options(cppkart PRIVATE /SUBSYSTEM:WINDOWS /ENTRY:WinMainCRTStartup)
endif()

# Enable AddressSanitizer
set(SANITIZE_FLAGS "-fsanitize=address -fno-omit-frame-pointer")

target_compile_options(cppkart PRIVATE -fsanitize=address)
target_link_libraries(cppkart PRIVATE -fsanitize=address)

if(UNIX AND NOT APPLE)
	# I'm assuming SDL2_mixer is already installed, FIX THIS!
	target_link_libraries(cppkart PRIVATE SDL2_mixer SDL2::SDL2 OpenGL::GL ${BULLET_LIBRARIES} assimp::assimp)
else()
	target_link_libraries(cppkart PRIVATE SDL2::SDL2 ${SDL2_MIXER_LIBRARY} OpenGL::GL ${BULLET_DYNAMICS_LIBRARY} ${BULLET_COLLISION_LIBRARY} ${BULLET_MATH_LIBRARY} ${BULLET_SOFTBODY_LIBRARY} assimp::assimp)
endif()

if (WIN32)
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
endif()

# Copy the resources directory to the target directory
add_custom_command(TARGET cppkart POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/src/ressources"
    #"$<TARGET_FILE_DIR:cppkart>/../src/resources")
	"$<TARGET_FILE_DIR:cppkart>/resources")
