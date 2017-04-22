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
                    utility/hashmap.h)

add_executable(Server_default ${SOURCE_FILES})
target_link_libraries(Server_default ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(Server_default readline)
