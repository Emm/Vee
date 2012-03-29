set(QT_USE_QTDBUS TRUE)
set(QT_USE_QTTEST TRUE)
include(${QT_USE_FILE})
include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}/../../src/vee-frontend
    ${CMAKE_CURRENT_SOURCE_DIR}/../../src/vee-frontend
)

set(test_vee-frontend_SRCS
    dummy_process.cpp
    dummy_process_builder.cpp
    dummy_remote_web_view.cpp
    dummy_remote_web_view_adaptor.cpp
    dummy_view_builder.cpp
    dummy_view.cpp
    )

set(test_vee-frontend_MOC_HDRS
    dummy_process.h
    dummy_process_builder.h
    dummy_remote_web_view.h
    dummy_remote_web_view_adaptor.h
    dummy_view_builder.h
    dummy_view.h
    )

qt4_wrap_cpp(test_vee-frontend_MOC_SRCS ${test_vee-frontend_MOC_HDRS})

qt4_automoc(${test_vee-frontend_SRCS})
add_library(libvee-tests ${test_vee-frontend_SRCS} ${test_vee-frontend_MOC_SRCS})
target_link_libraries(libvee-tests ${QT_LIBRARIES} libvee-frontend)

test(vee-frontend test_embed_command)
test(vee-frontend test_command_line_parser)
test(vee-frontend test_blank_view)
test(vee-frontend test_blank_view_builder)
test(vee-frontend test_web_view)
target_link_libraries(vee-frontend-test_web_view libvee-tests)
test(vee-frontend test_web_view_builder)
target_link_libraries(vee-frontend-test_web_view_builder libvee-tests)
test(vee-frontend test_vim)
test(vee-frontend test_view_resolver)
target_link_libraries(vee-frontend-test_view_resolver libvee-tests)
test(vee-frontend test_view_tab)
target_link_libraries(vee-frontend-test_view_tab libvee-tests)
test(vee-frontend test_history_entry)
test(vee-frontend test_tab_history)
