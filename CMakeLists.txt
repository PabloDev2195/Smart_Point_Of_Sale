cmake_minimum_required(VERSION 3.19)
project(Smart_Point_Of_Sale LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Sql Charts)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

qt_standard_project_setup()

qt_add_executable(Smart_Point_Of_Sale
    WIN32 MACOSX_BUNDLE
    app.rc

    src/main.cpp

    resources/resources.qrc
    src/spos_mainwindow.cpp
    src/spos_mainwindow.h
    src/spos_mainwindow.ui

    src/dialog/dialog_addproduct.h
    src/dialog/dialog_addproduct.cpp
    src/dialog/dialog_addproduct.ui
    src/dialog/dialog_modifyproduct.h
    src/dialog/dialog_modifyproduct.cpp
    src/dialog/dialog_modifyproduct.ui
    src/dialog/dialog_deleteproduct.h
    src/dialog/dialog_deleteproduct.cpp
    src/dialog/dialog_deleteproduct.ui
    src/dialog/dialog_findproduct.h
    src/dialog/dialog_findproduct.cpp
    src/dialog/dialog_findproduct.ui
    src/dialog/dialog_payment.h
    src/dialog/dialog_payment.cpp
    src/dialog/dialog_payment.ui
    src/dialog/dialog_ticket.h
    src/dialog/dialog_ticket.cpp
    src/dialog/dialog_ticket.ui
    src/dialog/dialog_salechart.h
    src/dialog/dialog_salechart.cpp
    src/dialog/dialog_salechart.ui

    src/models/Product.h

    src/database/database_manager.cpp
    src/database/database_manager.h

    src/product_database/product_mgr.h
    src/product_database/product_mgr.cpp

    src/current_sale/current_sale.h
    src/current_sale/current_sale.cpp

    src/sale_database/sale_database.h
    src/sale_database/sale_database.cpp

    src/sales_mgr/sales_mgr.h
    src/sales_mgr/sales_mgr.cpp

    src/statistics/sales_statistics.h
    src/statistics/sales_statistics.cpp
)

target_link_libraries(Smart_Point_Of_Sale
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt::Sql
        Qt::Charts
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
