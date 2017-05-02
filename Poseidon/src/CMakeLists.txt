set(SOURCE_FILES main.c
        server/parser.c
        server/server.c
        server/answer.c
        utility/tools.c
        utility/fish_type.c
        view/view.c
        utility/file.c
        model/aquarium.c
        model/aquarium.h
        utility/vector.c
        utility/vector.h
        model/fish.c
        model/fish.h
        model/aquarium_view.c
        model/aquarium_view.h
        utility/hashmap.c
        utility/hashmap.h server/world.c server/world.h utility/data.h server/send.c server/send.h server/client.c server/client.h)

add_executable(Server ${SOURCE_FILES})
target_link_libraries(Server ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(Server readline)

add_custom_command(TARGET Server
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/../controller.cfg
        $<TARGET_FILE_DIR:Server>)
