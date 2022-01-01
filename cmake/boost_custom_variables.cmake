message(STATUS "boost_custom_variables.cmake")

if(OS_WINDOWS)
  if (GEN_NINJA)
    # When using the Ninja generator clear the CMake defaults to avoid excessive
    # console warnings (see issue #2120).
    set(CMAKE_CXX_FLAGS "")
    set(CMAKE_CXX_FLAGS_DEBUG "")
    set(CMAKE_CXX_FLAGS_RELEASE "")
  endif()

  # Consumers who run into LNK4099 warnings can pass /Z7 instead (see issue #385).
  set(BOOST_DEBUG_INFO_FLAG "/Zi" CACHE STRING "Optional flag specifying specific /Z flag to use")

  # Consumers using different runtime types may want to pass different flags
  set(BOOST_RUNTIME_LIBRARY_FLAG "/MT" CACHE STRING "Optional flag specifying which runtime to use")
  if (BOOST_RUNTIME_LIBRARY_FLAG)
    list(APPEND BOOST_COMPILER_FLAGS_DEBUG ${BOOST_RUNTIME_LIBRARY_FLAG}d)
    list(APPEND BOOST_COMPILER_FLAGS_RELEASE ${BOOST_RUNTIME_LIBRARY_FLAG})
  endif()

  # Platform-specific compiler/linker flags.
  set(BOOST_LIBTYPE STATIC)
  list(APPEND BOOST_COMPILER_FLAGS
    /MP           # Multiprocess compilation
    /Gy           # Enable function-level linking
    /GR-          # Disable run-time type information
    /W4           # Warning level 4
    /WX-          # Treat warnings as errors
    /wd4100       # Ignore "unreferenced formal parameter" warning
    /wd4127       # Ignore "conditional expression is constant" warning
    /wd4244       # Ignore "conversion possible loss of data" warning
    /wd4324       # Ignore "structure was padded due to alignment specifier" warning
    /wd4481       # Ignore "nonstandard extension used: override" warning
    /wd4512       # Ignore "assignment operator could not be generated" warning
    /wd4701       # Ignore "potentially uninitialized local variable" warning
    /wd4702       # Ignore "unreachable code" warning
    /wd4996       # Ignore "function or variable may be unsafe" warning
    ${BOOST_DEBUG_INFO_FLAG}
    )
  list(APPEND BOOST_COMPILER_FLAGS_DEBUG
    /RTC1         # Disable optimizations
    /Od           # Enable basic run-time checks
    )
  list(APPEND BOOST_COMPILER_FLAGS_RELEASE
    /O2           # Optimize for maximum speed
    /Ob2          # Inline any suitable function
    /GF           # Enable string pooling
    )
  list(APPEND BOOST_LINKER_FLAGS_DEBUG
    /DEBUG        # Generate debug information
    )
  list(APPEND BOOST_EXE_LINKER_FLAGS
    /MANIFEST:NO        # No default manifest (see ADD_WINDOWS_MANIFEST macro usage)
    /LARGEADDRESSAWARE  # Allow 32-bit processes to access 3GB of RAM
    )
  list(APPEND BOOST_COMPILER_DEFINES
    WIN32 _WIN32 _WINDOWS             # Windows platform
    UNICODE _UNICODE                  # Unicode build
    WINVER=0x0601 _WIN32_WINNT=0x601  # Targeting Windows 7
    NOMINMAX                          # Use the standard's templated min/max
    WIN32_LEAN_AND_MEAN               # Exclude less common API declarations
    _HAS_EXCEPTIONS=0                 # Disable exceptions
    )
  list(APPEND BOOST_COMPILER_DEFINES_RELEASE
    NDEBUG _NDEBUG                    # Not a debug build
    )

  # Standard libraries.
  set(BOOST_STANDARD_LIBS
    # comctl32.lib
    # gdi32.lib
    # rpcrt4.lib
    # shlwapi.lib
    # ws2_32.lib
    )

  # BOOST directory paths.
  set(BOOST_RESOURCE_DIR        "${_BOOST_ROOT}/Resources")
  set(BOOST_BINARY_DIR          "${_BOOST_ROOT}/$<CONFIGURATION>")
  set(BOOST_BINARY_DIR_DEBUG    "${_BOOST_ROOT}/Debug")
  set(BOOST_BINARY_DIR_RELEASE  "${_BOOST_ROOT}/Release")

endif()