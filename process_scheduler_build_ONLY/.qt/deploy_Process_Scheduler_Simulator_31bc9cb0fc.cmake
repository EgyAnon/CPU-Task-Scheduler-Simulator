include("D:/College/Current Courses/OS/process_scheduler_build_ONLY/.qt/QtDeploySupport.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/Process_Scheduler_Simulator-plugins.cmake" OPTIONAL)
set(__QT_DEPLOY_I18N_CATALOGS "qtbase")

qt6_deploy_runtime_dependencies(
    EXECUTABLE D:/College/Current Courses/OS/process_scheduler_build_ONLY/Process_Scheduler_Simulator.exe
    GENERATE_QT_CONF
)
