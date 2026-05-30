include("E:/QtWorkspace/Qt-TextEdit/build/Desktop_Qt_6_11_0_MinGW_64_bit-Debug/.qt/QtDeploySupport.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/TextEidt-plugins.cmake" OPTIONAL)
set(__QT_DEPLOY_I18N_CATALOGS "qtbase")

qt6_deploy_runtime_dependencies(
    EXECUTABLE "E:/QtWorkspace/Qt-TextEdit/build/Desktop_Qt_6_11_0_MinGW_64_bit-Debug/TextEidt.exe"
    GENERATE_QT_CONF
)
