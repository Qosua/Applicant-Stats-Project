cmake_minimum_required(VERSION 4.1)
project(ApplicantStatsProject)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if (DEFINED ENV{QT_ROOT})
    set(QT_ROOT_PATH $ENV{QT_ROOT})
endif ()
set(CMAKE_PREFIX_PATH "${QT_ROOT_PATH}/mingw_64")

set(Qt6_DIR "${CMAKE_PREFIX_PATH}/lib/cmake/Qt6")

add_subdirectory(libs)
include(FetchContent)


find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        Charts
        REQUIRED)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(ApplicantStatsProject
        ${SOURCES}
        resources.qrc
)
target_include_directories(ApplicantStatsProject
        PRIVATE ${CMAKE_SOURCE_DIR}
        PRIVATE ${CMAKE_SOURCE_DIR}/src
)
target_link_libraries(ApplicantStatsProject PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt6::Charts
        $<$<PLATFORM_ID:Windows>:dwmapi>
        QXlsx::QXlsx
)

if (WIN32)
    get_target_property(QT_QMAKE_EXECUTABLE Qt6::qmake IMPORTED_LOCATION)
    get_filename_component(QT_BIN_DIR "${QT_QMAKE_EXECUTABLE}" DIRECTORY)
    find_program(WINDEPLOYQT windeployqt HINTS "${QT_BIN_DIR}")

    foreach (MINGW_DLL libgcc_s_seh-1.dll libstdc++-6.dll libwinpthread-1.dll)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${QT_BIN_DIR}/${MINGW_DLL}"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/"
                COMMENT "Copying ${MINGW_DLL}..."
        )
    endforeach ()

    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND "${WINDEPLOYQT}"
            --qmldir "${CMAKE_SOURCE_DIR}"
            --no-translations
            "$<TARGET_FILE:${PROJECT_NAME}>"
            COMMENT "Running windeployqt..."
    )
endif ()