option(GUI_ENABLE "Enable GUI" ON)
if(GUI_ENABLE)
  set(GUI ON)
  set(CMAKE_AUTOMOC ON)
  set(CMAKE_AUTORCC ON)
  set(CMAKE_AUTOUIC ON)
  find_package(Qt5 REQUIRED COMPONENTS Widgets)
endif()

find_package(PkgConfig REQUIRED)
set(BUILD_SHARED_LIBS OFF)

pkg_check_modules(ICU REQUIRED icu-uc icu-io)

find_package(Threads REQUIRED)
set(THREADS_PREFER_PTHREAD_FLAG ON)

pkg_check_modules(JSON_C REQUIRED json-c)

configure_file(config.h.in config.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(siganim
    main.cpp
    SiganimCore.cpp
    SiganimDefaults.cpp
    SiganimFileHandler.cpp
    ../base64.c/base64.c
    ../tinyxml/tinyxml2.cpp
)

set_property(TARGET siganim PROPERTY CXX_STANDARD 17)
set(CXX_STANDARD_REQUIRED ON)

add_subdirectory(font)
add_subdirectory(render)
add_subdirectory(sign)

if(GUI)
    add_subdirectory(ui)
    target_link_libraries(siganim Qt5::Widgets)
    target_link_libraries(siganim Threads::Threads)
endif(GUI)

option(STATIC_LINKING "Use static linking" OFF)
if(STATIC_LINKING)
    # If linking statically, then we need to add icudt.
    target_link_libraries(siganim sicuuc sicuio sicudt)
else(STATIC_LINKING)
    target_link_libraries(siganim icuuc icuio)
endif(STATIC_LINKING)

target_link_libraries(siganim json-c)

