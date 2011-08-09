set(QT_USE_QTWEBKIT TRUE)
set(QT_USE_QTDBUS TRUE)
set(QT_USE_QTTEST TRUE)
include(${QT_USE_FILE})
include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../../src/vee-web
)

set(vee-web-tests_SRCS
     test_view_url.cpp
    )

qt4_wrap_cpp(vee-web-tests_MOC_SRCS ${vee-web-tests_SRCS} OPTIONS -f)
add_executable(vee-web-tests ${vee-web-tests_MOC_SRCS})
target_link_libraries(vee-web-tests ${QT_LIBRARIES} libvee-web)
add_test(NAME vee-web-tests COMMAND vee-web-tests)
