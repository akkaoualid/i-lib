find_package(GTest REQUIRED)

function(add_ilib_test name)
    add_executable(${name} ${name}.cpp)
    target_compile_features(${name} PUBLIC cxx_std_20)
    target_link_libraries(${name} PUBLIC GTest::GTest)
    target_include_directories(${name} PUBLIC ${CMAKE_SOURCE_DIR}/include)
    add_warns(${name})
endfunction()

add_ilib_test(containers)
add_ilib_test(utility)
