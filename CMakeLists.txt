cmake_minimum_required(VERSION 3.19)
project(LogAnalysisSystem LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

qt_add_executable(LogAnalysisSystem
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h


    logindialog.h
    logindialog.cpp
    resources/resources.qrc
    animatedbutton.h
    animatedbutton.cpp
    animatedlineedit.h
    animatedlineedit.cpp
    titlebar.h
    titlebar.cpp
    animatedstackedwidget.h
    animatedstackedwidget.cpp
    navbutton.h
    navbutton.cpp
    sidebar.h
    sidebar.cpp
    circularprogress.h
    circularprogress.cpp
    userstore.h
    userstore.cpp
)

target_link_libraries(LogAnalysisSystem
    PRIVATE
        Qt::Core
        Qt::Widgets
)

install(TARGETS LogAnalysisSystem
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET LogAnalysisSystem
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
