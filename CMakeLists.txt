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
        Qml
        Quick
        Graphs
        REQUIRED)

add_executable(ApplicantStatsProject

        src/xlsx.h
        src/data-processing/cache-manager.h
        src/data-processing/magic-hat.h
        #src/data-processing/data-to-stats-converter.h
        src/support-system.h
        src/qml-helper.h
        src/models/tree-view-model.h
        src/applicants-faculty-data/table-parser-bachelor.h
        src/applicants-faculty-data/namespaces.h
        src/applicants-faculty-data/faculty-direction.h
        src/applicants-faculty-data/applicant.h
        src/applicants-faculty-data/priority-info.h
        src/models/tables-list-model.h
        src/models/stats-page-model.h

        src/main.cpp
        src/data-processing/cache-manager.cpp
        src/data-processing/magic-hat.cpp
        #src/data-processing/data-to-stats-converter.cpp
        src/models/tree-view-model.cpp
        src/applicants-faculty-data/table-parser-bachelor.cpp
        src/applicants-faculty-data/faculty-direction.cpp
        src/applicants-faculty-data/applicant.cpp
        src/applicants-faculty-data/priority-info.cpp
        src/models/tables-list-model.cpp
        src/models/stats-page-model.cpp

        resources.qrc
)

qt_add_qml_module(ApplicantStatsProject
        URI ApplicantStatsProjectModule
        VERSION 1.0
        NO_PLUGIN
        QML_FILES
        src/gui_qml/Main.qml
        src/gui_qml/StatsPage.qml
        src/gui_qml/components/PageButton.qml
        src/gui_qml/TablesList.qml
        src/gui_qml/visual-effects/LeftTopGradient.qml
        src/gui_qml/PagesMenu.qml
        src/gui_qml/StatusBar.qml
        src/gui_qml/FacultyTreeView.qml
        src/gui_qml/FacultyStatsPage.qml
        src/gui_qml/DirectionStatsPage.qml
        src/gui_qml/EmptyStatsPlaceholder.qml
        src/gui_qml/components/ScoreDist.qml
)

target_link_libraries(ApplicantStatsProject PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt6::Qml
        Qt6::Quick
        Qt6::Graphs
        QXlsx::QXlsx
        $<$<PLATFORM_ID:Windows>:dwmapi>
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