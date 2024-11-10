add_executable(
    semu
    main.cc
)

target_link_libraries(
    semu libsemu
)