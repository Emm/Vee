set(QT_USE_QTWEBKIT TRUE)
set(QT_USE_QTDBUS TRUE)
set(QT_USE_QTTEST TRUE)
include(${QT_USE_FILE})
include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}/../../src/vee-web
    ${CMAKE_CURRENT_SOURCE_DIR}/../../src/vee-web
)

test(vee-web test_web_view_proxy_url)
test(vee-web test_web_view_proxy_file)
test(vee-web test_web_view_proxy_adaptor)
test(vee-web test_command_line_parser)
