add_executable(wdb-plus-utest field_test.cpp types_test.cpp record_test.cpp)

target_include_directories(wdb-plus-utest PRIVATE ../src /usr/include)
target_link_libraries(wdb-plus-utest libgtest_main.a gtest pthread wdbplus wgdb)

add_test(NAME wdb-plus-utest-field COMMAND wdb-plus-utest --gtest_filter=UT_Fields.*)
add_test(NAME wdb-plus-utest-types COMMAND wdb-plus-utest --gtest_filter=UT_Types.*)
add_test(NAME wdb-plus-utest-records COMMAND wdb-plus-utest --gtest_filter=Rec_fx.*)