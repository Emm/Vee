# the next line sets up include and link directories and defines some variables that we will use.
# you can modify the behavior by setting some variables, e.g.
#   set(QT_USE_OPENGL TRUE)
# -> this will cause cmake to include and link against the OpenGL module

include_directories(/usr/include/tclap)
set(QT_USE_QTWEBKIT TRUE)
set(QT_USE_QTDBUS TRUE)
include(${QT_USE_FILE})
# see http://cmake.org/cmake/help/cmake2.6docs.html#module:FindQt4 for a complete list

# the variable "vee-web_SRCS" contains all .cpp files of this project


set(vee-web_SRCS
    view.cpp
    main.cpp
)

qt4_add_dbus_adaptor(vee-web_SRCS ${CMAKE_SOURCE_DIR}/../interfaces/org.vee.web.View.xml 
        view.h View view_adaptor)

qt4_wrap_cpp(vee-web_SRCS ${vee-web_MOC_HDRS})

# tell cmake to create .moc files for all files in the variable vee-web_SRCS that require such a file.
# note: this assumes that you use #include "header.moc" in your files
qt4_automoc(${vee-web_SRCS})

include_directories( ${CMAKE_BINARY_DIR}/src )
include_directories( ${CMAKE_SOURCE_DIR}/src )

# create an executable file named "vee-web" from the source files in the variable "vee-web_SRCS".
add_executable(vee-web ${vee-web_SRCS} ${vee-web_MOC_HDRS})

# link the "vee-web" target against the Qt libraries. which libraries exactly, is defined by the "include(${QT_USE_FILE})" line above, which sets up this variable.
target_link_libraries(vee-web ${QT_LIBRARIES})
