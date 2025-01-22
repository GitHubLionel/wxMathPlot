cmake_minimum_required(VERSION 3.25)
project(wxMathPlot)
set(LIB_VERSION_MAJOR "1")
set(LIB_VERSION_MINOR "1")
set(LIB_VERSION_PATCH "0")
if (MSVC)
	add_compile_options(/W4)
else()
	add_compile_options(-Wall -Wextra)
endif()
find_package(wxWidgets REQUIRED core base adv aui)
# wxMathPlot lib
set(wxmathplot_src mathplot/mathplot.cpp MathPlotConfig/MathPlotConfig.cpp)
add_library(wxmathplot SHARED ${wxmathplot_src})
add_library(wxmathplot_static ${wxmathplot_src})
target_link_libraries(wxmathplot ${wxWidgets_LIBRARIES})
include(${wxWidgets_USE_FILE})
include_directories(${CMAKE_SOURCE_DIR}/MathPlotConfig)
include_directories(${CMAKE_SOURCE_DIR}/mathplot)
INSTALL(TARGETS wxmathplot
	RUNTIME DESTINATION bin COMPONENT Applications
	LIBRARY DESTINATION lib COMPONENT Libraries
)
install(FILES mathplot/mathplot.h DESTINATION include/wxmathplot COMPONENT Development)
install(FILES MathPlotConfig/MathPlotConfig.h DESTINATION include/wxmathplot COMPONENT Development)

# Demos
add_executable(MathPlotDemo 
	MathPlotDemo/MathPlotDemoApp.cpp MathPlotDemo/MathPlotDemoMain.cpp
	)
target_link_libraries(MathPlotDemo wxmathplot_static ${wxWidgets_LIBRARIES})

add_executable(MathPlotConfigDemo 
	MathPlotConfig/MathPlotConfigDemo.cpp
	)
target_link_libraries(MathPlotConfigDemo wxmathplot_static ${wxWidgets_LIBRARIES})

if(WIN32 AND MSVC)
	set_target_properties(MathPlotDemo PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")
	set_target_properties(MathPlotConfigDemo PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")
endif(WIN32 AND MSVC)

set(MY_PATH "PATH=%PATH%" C:/wxWidgetsDev/wx-3.2.6/lib/vc14x_x64_dll)
set_target_properties(MathPlotDemo PROPERTIES VS_DEBUGGER_ENVIRONMENT "${MY_PATH}")
set_target_properties(MathPlotConfigDemo PROPERTIES VS_DEBUGGER_ENVIRONMENT "${MY_PATH}")

#CPack

set(CPACK_PACKAGE_DESCRIPTION "wxMathPlot")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "wxMathPlot")
set(CPACK_PACKAGE_VENDOR "wxMathPlot")
set(CPACK_PACKAGE_CONTACT "https://github.com/GitHubLionel/wxMathPlot")
set(CPACK_PACKAGE_VERSION_MAJOR "${LIB_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${LIB_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${LIB_VERSION_PATCH}")
set(CPACK_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}_${LIB_VERSION_MAJOR}.${LIB_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}_${CMAKE_SYSTEM_PROCESSOR}")
set(CPACK_SOURCE_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}_${LIB_VERSION_MAJOR}.${LIB_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")

set(CPACK_COMPONENTS_ALL Libraries Development Applications)
include(CPack)
