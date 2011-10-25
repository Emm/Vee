# Global test macro
# Accepts the name of the current module being tested (which should be a
# library) and the name of the test file, minus the cpp extension
macro(test module test_file)
    set(${module}-${test_file}_SRCS ${test_file}.cpp)
    qt4_wrap_cpp(${module}-${test_file}_MOC_SRCS ${${module}-${test_file}_SRCS} OPTIONS -f)
    add_executable(${module}-${test_file} ${${module}-${test_file}_MOC_SRCS})
    target_link_libraries(${module}-${test_file} ${QT_LIBRARIES} lib${module})
    add_test(NAME ${module}-${test_file} COMMAND ${module}-${test_file})
endmacro(test)

add_subdirectory(./vee-web)
add_subdirectory(./vee-frontend)
