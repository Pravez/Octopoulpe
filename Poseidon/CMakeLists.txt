
set(SOURCE_FILES main.c
                    server/server.c
                    server/parser.c
                    utility/tools.c
                    utility/fish_type.c
                    model/aquarium.c
                    view/view.c)

add_executable(Server_default ${SOURCE_FILES})