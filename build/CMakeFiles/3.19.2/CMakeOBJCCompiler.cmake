set(CMAKE_OBJC_COMPILER "/usr/local/bin/x86_64-apple-darwin20-gcc-10.2.0")
set(CMAKE_OBJC_COMPILER_ARG1 "")
set(CMAKE_OBJC_COMPILER_ID "GNU")
set(CMAKE_OBJC_COMPILER_VERSION "10.2.0")
set(CMAKE_OBJC_COMPILER_VERSION_INTERNAL "")
set(CMAKE_OBJC_COMPILER_WRAPPER "")
set(CMAKE_OBJC_STANDARD_COMPUTED_DEFAULT "90")
set(CMAKE_OBJC_COMPILE_FEATURES "")
set(CMAKE_OBJC90_COMPILE_FEATURES "")
set(CMAKE_OBJC99_COMPILE_FEATURES "")
set(CMAKE_OBJC11_COMPILE_FEATURES "")

set(CMAKE_OBJC_PLATFORM_ID "Darwin")
set(CMAKE_OBJC_SIMULATE_ID "")
set(CMAKE_OBJC_COMPILER_FRONTEND_VARIANT "")
set(CMAKE_OBJC_SIMULATE_VERSION "")


set(CMAKE_AR "/Library/Developer/CommandLineTools/usr/bin/ar")
set(CMAKE_OBJC_COMPILER_AR "/usr/local/bin/x86_64-apple-darwin20-gcc-ar-10")
set(CMAKE_RANLIB "/Library/Developer/CommandLineTools/usr/bin/ranlib")
set(CMAKE_OBJC_COMPILER_RANLIB "/usr/local/bin/x86_64-apple-darwin20-gcc-ranlib-10")
set(CMAKE_LINKER "/Library/Developer/CommandLineTools/usr/bin/ld")
set(CMAKE_MT "")
set(CMAKE_COMPILER_IS_GNUOBJC 1)
set(CMAKE_OBJC_COMPILER_LOADED 1)
set(CMAKE_OBJC_COMPILER_WORKS TRUE)
set(CMAKE_OBJC_ABI_COMPILED TRUE)

set(CMAKE_OBJC_COMPILER_ENV_VAR "OBJC")

set(CMAKE_OBJC_COMPILER_ID_RUN 1)
set(CMAKE_OBJC_SOURCE_FILE_EXTENSIONS m)
set(CMAKE_OBJC_IGNORE_EXTENSIONS h;H;o;O)
set(CMAKE_OBJC_LINKER_PREFERENCE 5)

foreach (lang C CXX OBJCXX)
  foreach(extension IN LISTS CMAKE_OBJC_SOURCE_FILE_EXTENSIONS)
    if (CMAKE_${lang}_COMPILER_ID_RUN)
      list(REMOVE_ITEM CMAKE_${lang}_SOURCE_FILE_EXTENSIONS ${extension})
    endif()
  endforeach()
endforeach()

# Save compiler ABI information.
set(CMAKE_OBJC_SIZEOF_DATA_PTR "8")
set(CMAKE_OBJC_COMPILER_ABI "")
set(CMAKE_OBJC_LIBRARY_ARCHITECTURE "")

if(CMAKE_OBJC_SIZEOF_DATA_PTR)
  set(CMAKE_SIZEOF_VOID_P "${CMAKE_OBJC_SIZEOF_DATA_PTR}")
endif()

if(CMAKE_OBJC_COMPILER_ABI)
  set(CMAKE_INTERNAL_PLATFORM_ABI "${CMAKE_OBJC_COMPILER_ABI}")
endif()

if(CMAKE_OBJC_LIBRARY_ARCHITECTURE)
  set(CMAKE_LIBRARY_ARCHITECTURE "")
endif()


set(CMAKE_OBJC_SYSROOT_FLAG "-isysroot")
set(CMAKE_OBJC_OSX_DEPLOYMENT_TARGET_FLAG "-mmacosx-version-min=")

set(CMAKE_OBJC_IMPLICIT_INCLUDE_DIRECTORIES "/usr/local/Cellar/gcc/10.2.0_2/lib/gcc/10/gcc/x86_64-apple-darwin20/10.2.0/include;/usr/local/Cellar/gcc/10.2.0_2/lib/gcc/10/gcc/x86_64-apple-darwin20/10.2.0/include-fixed;/Library/Developer/CommandLineTools/SDKs/MacOSX11.0.sdk/usr/include;/Library/Developer/CommandLineTools/SDKs/MacOSX11.0.sdk/System/Library/Frameworks")
set(CMAKE_OBJC_IMPLICIT_LINK_LIBRARIES "gcc_ext.10.5;gcc")
set(CMAKE_OBJC_IMPLICIT_LINK_DIRECTORIES "/usr/local/Cellar/gcc/10.2.0_2/lib/gcc/10/gcc/x86_64-apple-darwin20/10.2.0;/usr/local/Cellar/gcc/10.2.0_2/lib/gcc/10;/Library/Developer/CommandLineTools/SDKs/MacOSX11.0.sdk/usr/lib")
set(CMAKE_OBJC_IMPLICIT_LINK_FRAMEWORK_DIRECTORIES "/Library/Developer/CommandLineTools/SDKs/MacOSX11.0.sdk/System/Library/Frameworks")
