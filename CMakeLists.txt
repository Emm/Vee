cmake_minimum_required(VERSION 2.4.0)
project(vee)

find_package(Qt4 REQUIRED)
set(VEE_WEB_DBUS_XML ${CMAKE_SOURCE_DIR}/interfaces/org.vee.web.View.xml)

add_subdirectory(./src/vee-frontend)
add_subdirectory(./src/vee-web)
