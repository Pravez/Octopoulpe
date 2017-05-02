include_directories ("${PROJECT_SOURCE_DIR}/Poseidon/src")

set(SOURCE_FILES ../src/server/parser.c
        ../src/server/server.c
        ../src/server/answer.c
        ../src/utility/tools.c
        ../src/utility/fish_type.c
        ../src/view/view.c
        ../src/utility/file.c
        ../src/model/aquarium.c
        ../src/model/aquarium.h
        ../src/utility/vector.c
        ../src/utility/vector.h
        ../src/model/fish.c
        ../src/model/fish.h
        ../src/model/aquarium_view.c
        ../src/model/aquarium_view.h
        ../src/utility/hashmap.c
        ../src/utility/hashmap.h
        ../src/server/world.c
        ../src/server/world.h
        ../src/utility/data.h
        ../src/server/send.c
        ../src/server/send.h
        ../src/server/client.c
        ../src/server/client.h
        ../src/utility/data.h)

add_executable(test_server_send test_server_send.c ${SOURCE_FILES})
target_link_libraries(test_server_send ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(test_server_send readline)
