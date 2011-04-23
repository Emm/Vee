# the next line sets up include and link directories and defines some variables that we will use.
# you can modify the behavior by setting some variables, e.g.
#   set(QT_USE_OPENGL TRUE)
# -> this will cause cmake to include and link against the OpenGL module

INCLUDE_DIRECTORIES(/usr/include/tclap)

set(QT_USE_QTMAIN TRUE)
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

set(vee-frontend_MOC_HDRS
    embedcontainer.h
    embedtabs.h
    mainwindow.h
)

qt4_wrap_cpp(vee-frontend_MOC_SRCS ${vee-frontend_MOC_HDRS})

# tell cmake to create .moc files for all files in the variable vee-frontend_SRCS that require such a file.
# note: this assumes that you use #include "header.moc" in your files
qt4_automoc(${vee-frontend_SRCS})

# create an executable file named "vee" from the source files in the variable "vee-frontend_SRCS".
add_executable(vee ${vee-frontend_SRCS} ${vee-frontend_MOC_SRCS})

# link the "vee-frontend" target against the Qt libraries. which libraries exactly, is defined by the "include(${QT_USE_FILE})" line above, which sets up this variable.
target_link_libraries(vee ${QT_LIBRARIES})
