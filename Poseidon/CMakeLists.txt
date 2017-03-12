
set(SOURCE_FILES main.c
                    server/parser.c
                    server/server.c
                    utility/tools.c
                    utility/fish_type.c
                    model/aquarium.c
                    view/view.c
                    utility/file.c)

add_executable(Server_default ${SOURCE_FILES})