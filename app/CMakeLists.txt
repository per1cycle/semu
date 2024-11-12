add_executable(
    test-rv32ui-p-add
    test-rv32ui-p-add.cc
)
target_link_libraries(
    test-rv32ui-p-add libsemu
)

add_executable(
    linux
    linux.cc
)
target_link_libraries(
    linux libsemu
)

if(BUILD_BACKEND)
    add_subdirectory(backend)
endif(BUILD_BACKEND)
