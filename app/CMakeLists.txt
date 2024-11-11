add_executable(
    semu
    test.cc
)
target_link_libraries(
    semu libsemu
)

add_executable(
    linux
    linux.cc
)
target_link_libraries(
    linux libsemu
)
