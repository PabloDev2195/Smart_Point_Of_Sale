cmake_minimum_required(VERSION 3.19)
project(Smart_Point_Of_Sale LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Sql)

set(CMAKE_AUTOUIC ON)

qt_standard_project_setup()

qt_add_executable(Smart_Point_Of_Sale
    WIN32 MACOSX_BUNDLE
    src/main.cpp
    src/spos_mainwindow.cpp
    src/spos_mainwindow.h
    src/database/database_manager.cpp
    src/database/database_manager.h
    src/models/Product.h
    src/repositories/productRepository.cpp
    src/repositories/productRepository.h
    src/spos_mainwindow.ui
)

target_link_libraries(Smart_Point_Of_Sale
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt::Sql
)

install(TARGETS Smart_Point_Of_Sale
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET Smart_Point_Of_Sale
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)

install(SCRIPT ${deploy_script})
