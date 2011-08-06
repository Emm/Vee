# the next line sets up include and link directories and defines some variables that we will use.
# you can modify the behavior by setting some variables, e.g.
#   set(QT_USE_OPENGL TRUE)
# -> this will cause cmake to include and link against the OpenGL module

include_directories(/usr/include/tclap)

set(QT_USE_QTMAIN TRUE)
set(QT_USE_QTDBUS TRUE)
include(${QT_USE_FILE})
# see http://cmake.org/cmake/help/cmake2.6docs.html#module:FindQt4 for a complete list

# the variable "vee-frontend_SRCS" contains all .cpp files of this project


set(vee-frontend_SRCS
    embedcommand.cpp
    embedcontainer.cpp
    embedtabs.cpp
    mainwindow.cpp
    main.cpp
)

qt4_add_dbus_interface(vee-frontend_SRCS
    ${CMAKE_SOURCE_DIR}/../interfaces/org.vee.web.View.xml 
        vee_web_view_interface)

set(vee-frontend_MOC_HDRS
    embedcontainer.h
    embedtabs.h
    mainwindow.h
)

qt4_wrap_cpp(vee-frontend_MOC_SRCS ${vee-frontend_MOC_HDRS})

# tell cmake to create .moc files for all files in the variable vee-frontend_SRCS that require such a file.
# note: this assumes that you use #include "header.moc" in your files
qt4_automoc(${vee-frontend_SRCS})

include_directories( ${CMAKE_BINARY_DIR}/src )
include_directories( ${CMAKE_SOURCE_DIR} )

# create an executable file named "vee" from the source files in the variable "vee-frontend_SRCS".
add_executable(vee ${vee-frontend_SRCS} ${vee-frontend_MOC_SRCS})

# link the "vee-frontend" target against the Qt libraries. which libraries exactly, is defined by the "include(${QT_USE_FILE})" line above, which sets up this variable.
target_link_libraries(vee ${QT_LIBRARIES})
