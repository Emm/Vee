cmake_minimum_required(VERSION 2.4.0)
project(vee)

find_package(Qt4 REQUIRED)
set(WEB_VIEW_XML ${CMAKE_SOURCE_DIR}/interfaces/org.vee.WebView.xml)

enable_testing()
add_subdirectory(./src/vee-frontend)
add_subdirectory(./src/vee-web)
add_subdirectory(./tests)
