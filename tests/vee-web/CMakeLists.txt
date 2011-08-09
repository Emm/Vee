set(QT_USE_QTWEBKIT TRUE)
set(QT_USE_QTDBUS TRUE)
set(QT_USE_QTTEST TRUE)
include(${QT_USE_FILE})
include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../../src/vee-web
)



test(vee-web test_view_url)
