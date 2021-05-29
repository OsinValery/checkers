# Install script for directory: /Users/valerij/Desktop/checkers/raylib-cpp

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES
    "/Users/valerij/Desktop/checkers/raylib-cpp/AudioDevice.hpp"
    "/Users/valerij/Desktop/checkers/raylib-cpp/AudioStream.hpp"
    "/Users/valerij/Desktop/checkers/raylib-cpp/BoundingBox.hpp"
    "/Users/valerij/Desktop/checkers/raylib-cpp/Camera2D.hpp"
    "/Users/valerij/Desktop/checkers/raylib-cpp/Camera3D.hpp"
    "/Users/valerij/Desktop/checkers/raylib-cpp/Color.hpp"
    "/Users/valerij/Desktop/checkers/raylib-cpp/Font.hpp"
    "/Users/valerij/Desktop/checkers/raylib-cpp/Functions.hpp"
    "/Users/valerij/Desktop/checkers/raylib-cpp/Gamepad.hpp"
    "/Users/valerij/Desktop/checkers/raylib-cpp/Image.hpp"
    "/Users/valerij/Desktop/checkers/raylib-cpp/Material.hpp"
    "/Users/valerij/Desktop/checkers/raylib-cpp/Matrix.hpp"
    "/Users/valerij/Desktop/checkers/raylib-cpp/Mesh.hpp"
    "/Users/valerij/Desktop/checkers/raylib-cpp/Model.hpp"
    "/Users/valerij/Desktop/checkers/raylib-cpp/ModelAnimation.hpp"
    "/Users/valerij/Desktop/checkers/raylib-cpp/Mouse.hpp"
    "/Users/valerij/Desktop/checkers/raylib-cpp/Music.hpp"
    "/Users/valerij/Desktop/checkers/raylib-cpp/physac.hpp"
    "/Users/valerij/Desktop/checkers/raylib-cpp/Physics.hpp"
    "/Users/valerij/Desktop/checkers/raylib-cpp/Ray.hpp"
    "/Users/valerij/Desktop/checkers/raylib-cpp/RayHitInfo.hpp"
    "/Users/valerij/Desktop/checkers/raylib-cpp/raylib-cpp-utils.hpp"
    "/Users/valerij/Desktop/checkers/raylib-cpp/raylib-cpp.hpp"
    "/Users/valerij/Desktop/checkers/raylib-cpp/raylib.hpp"
    "/Users/valerij/Desktop/checkers/raylib-cpp/raymath.hpp"
    "/Users/valerij/Desktop/checkers/raylib-cpp/Rectangle.hpp"
    "/Users/valerij/Desktop/checkers/raylib-cpp/RenderTexture.hpp"
    "/Users/valerij/Desktop/checkers/raylib-cpp/Shader.hpp"
    "/Users/valerij/Desktop/checkers/raylib-cpp/Sound.hpp"
    "/Users/valerij/Desktop/checkers/raylib-cpp/Text.hpp"
    "/Users/valerij/Desktop/checkers/raylib-cpp/Texture.hpp"
    "/Users/valerij/Desktop/checkers/raylib-cpp/Vector2.hpp"
    "/Users/valerij/Desktop/checkers/raylib-cpp/Vector3.hpp"
    "/Users/valerij/Desktop/checkers/raylib-cpp/Vector4.hpp"
    "/Users/valerij/Desktop/checkers/raylib-cpp/VrSimulator.hpp"
    "/Users/valerij/Desktop/checkers/raylib-cpp/Wave.hpp"
    "/Users/valerij/Desktop/checkers/raylib-cpp/Window.hpp"
    )
endif()

