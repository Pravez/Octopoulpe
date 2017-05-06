add_executable(Test_intern_protocol test_intern_protocol.c test.h)
target_link_libraries(Test_intern_protocol SERVER_LIB)
target_link_libraries(Test_intern_protocol ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(Test_intern_protocol readline)

add_executable(Test_parser test_parser.c test.h)
target_link_libraries(Test_parser SERVER_LIB)
target_link_libraries(Test_parser ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(Test_parser readline)

add_custom_command(TARGET Test_parser
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/controller.cfg
        $<TARGET_FILE_DIR:Test_parser>)
