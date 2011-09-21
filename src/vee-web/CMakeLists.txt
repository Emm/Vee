# the next line sets up include and link directories and defines some variables that we will use.
# you can modify the behavior by setting some variables, e.g.
#   set(QT_USE_OPENGL TRUE)
# -> this will cause cmake to include and link against the OpenGL module

include_directories(/usr/include/tclap)
set(QT_USE_QTWEBKIT TRUE)
set(QT_USE_QTDBUS TRUE)
include(${QT_USE_FILE})

include_directories( ${CMAKE_CURRENT_BINARY_DIR} )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )

# see http://cmake.org/cmake/help/cmake2.6docs.html#module:FindQt4 for a complete list

# the variable "vee-web_SRCS" contains all .cpp files of this project


set(vee-web_SRCS
    vee_web_view_adaptor_impl.cpp
    vee_web_view.cpp
    command_line_parser.cpp
    widget_builder.cpp
    dbus_manager.cpp
)

set(vee-web_MOC_HDRS
    vee_web_view.h
    vee_web_view_adaptor_impl.h
    )

qt4_add_dbus_adaptor(vee-web_SRCS ${VEE_WEB_DBUS_XML} vee_web_view.h VeeWebView vee_web_view_adaptor)

qt4_wrap_cpp(vee-web_SRCS ${vee-web_MOC_HDRS})

# tell cmake to create .moc files for all files in the variable vee-web_SRCS that require such a file.
# note: this assumes that you use #include "header.moc" in your files
qt4_automoc(${vee-web_SRCS})

# create an executable file named "vee-web" from the source files in the variable "vee-web_SRCS".
add_library(libvee-web ${vee-web_SRCS} ${vee-web_MOC_HDRS})
set_target_properties(libvee-web PROPERTIES OUTPUT_NAME vee-web)
add_executable(vee-web main.cpp)

# link the "vee-web" target against the Qt libraries. which libraries exactly, is defined by the "include(${QT_USE_FILE})" line above, which sets up this variable.
target_link_libraries(libvee-web ${QT_LIBRARIES})
target_link_libraries(vee-web libvee-web)
