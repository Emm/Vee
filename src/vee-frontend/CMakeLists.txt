# the next line sets up include and link directories and defines some variables that we will use.
# you can modify the behavior by setting some variables, e.g.
#   set(QT_USE_OPENGL TRUE)
# -> this will cause cmake to include and link against the OpenGL module

include_directories(/usr/include/tclap)
include_directories( ${CMAKE_CURRENT_BINARY_DIR} )

set(QT_USE_QTMAIN TRUE)
set(QT_USE_QTDBUS TRUE)
set(QT_USE_QTALGORITHMS TRUE)
include(${QT_USE_FILE})
# see http://cmake.org/cmake/help/cmake2.6docs.html#module:FindQt4 for a complete list

# the variable "vee-frontend_SRCS" contains all .cpp files of this project


set(vee-frontend_SRCS
    embed_command.cpp
    view_tab.cpp
    view_tabs.cpp
    main_window.cpp
    view_resolver.cpp
    view.cpp
    remote_view.cpp
    blank_view.cpp
    web_view.cpp
    remote_view_builder.cpp
    blank_view_builder.cpp
    view_resolver_factory.cpp
    main.cpp
)

#qt4_add_dbus_interface(vee-frontend_SRCS ${VEE_WEB_DBUS_XML} web_view)

set(vee-frontend_MOC_HDRS
    view_tab.h
    view_tabs.h
    view_resolver.h
    view.h
    remote_view.h
    local_view.h
    blank_view.h
    web_view.h
    view_builder.h
    remote_view_builder.h
    blank_view_builder.h
    view_resolver_factory.h
    main_window.h
)

qt4_wrap_cpp(vee-frontend_MOC_SRCS ${vee-frontend_MOC_HDRS})

# tell cmake to create .moc files for all files in the variable vee-frontend_SRCS that require such a file.
# note: this assumes that you use #include "header.moc" in your files
qt4_automoc(${vee-frontend_SRCS})


# create an executable file named "vee" from the source files in the variable "vee-frontend_SRCS".
add_executable(vee ${vee-frontend_SRCS} ${vee-frontend_MOC_SRCS})

# link the "vee-frontend" target against the Qt libraries. which libraries exactly, is defined by the "include(${QT_USE_FILE})" line above, which sets up this variable.
target_link_libraries(vee ${QT_LIBRARIES})
