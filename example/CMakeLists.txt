
add_executable(example main.cpp)

target_link_libraries(example wdbplus wgdb)
target_include_directories(example PRIVATE ../src)
