set(QT_USE_QTDBUS TRUE)
set(QT_USE_QTTEST TRUE)
include(${QT_USE_FILE})
include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}/../../src/vee-frontend
    ${CMAKE_CURRENT_SOURCE_DIR}/../../src/vee-frontend
)

test(vee-frontend test_embed_command)
test(vee-frontend test_blank_view)
test(vee-frontend test_blank_view_builder)
