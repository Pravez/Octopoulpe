set(SOURCE_FILES
        server/parser.c
        server/server.c
        server/communication/answer.c
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
        utility/hashmap.h server/world/world.c server/world/world.h utility/data.h server/communication/send.c server/communication/send.h server/communication/client.c server/communication/client.h)

add_library(SERVER_LIB ${SOURCE_FILES})
target_link_libraries(SERVER_LIB readline)

add_executable(Server main.c)
target_link_libraries(Server SERVER_LIB)
target_link_libraries(Server ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(Server readline)

add_custom_command(TARGET Server
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/../controller.cfg
        $<TARGET_FILE_DIR:Server>)
