cmake_minimum_required(VERSION 4.1)

include(FetchContent)
find_package(Threads REQUIRED)

set(FETCHCONTENT_BASE_DIR ${CMAKE_SOURCE_DIR}/libs/_deps)
FetchContent_Declare(
        QXlsx
        GIT_REPOSITORY https://github.com/QtExcel/QXlsx.git
        GIT_TAG        v1.5.0
        SOURCE_SUBDIR  QXlsx
)

FetchContent_MakeAvailable(QXlsx)