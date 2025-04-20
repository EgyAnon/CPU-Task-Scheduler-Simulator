cmake_minimum_required(VERSION 3.19)
project(Process_Scheduler_Simulator LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

qt_add_executable(Process_Scheduler_Simulator
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    process.hpp runner.hpp scheduler.hpp simulator.cpp simulator.hpp timer.hpp
    process.cpp runner.cpp scheduler.cpp simulator.cpp
    timer.cpp

)

target_link_libraries(Process_Scheduler_Simulator
    PRIVATE
        Qt::Core
        Qt::Widgets
)

include(GNUInstallDirs)

install(TARGETS Process_Scheduler_Simulator
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET Process_Scheduler_Simulator
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
