set(JAVA_SOURCES sample/Aquarium.java
        sample/Communicator.java
        sample/Console.java
        sample/Controller.java
        sample/Fish.java
        sample/Parser.java
        sample/ReceiveHandler.java
        sample/ScenarioWindow.java)

set(JAVA_RESOURCES sample/bb.png sample/bg.png)

add_jar(Aquarium ${JAVA_SOURCES} ${JAVA_RESOURCES})

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

#set(RESOURCE_IMAGES ../resources/Images/*)
#set(RESOURCE_FILES ../resources/affichage.cfg
#        ../resources/MobilityModels.txt
#        ../resources/scenarios.txt
#        ../resources/logs.txt)

#file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Images)

#foreach (item IN LISTS RESOURCE_IMAGES)
#    message(STATUS ${item})
#    add_custom_command(TARGET Aquarium
#            COMMAND ${CMAKE_COMMAND} -E copy_if_different
#            ${CMAKE_CURRENT_SOURCE_DIR}/${item}
#            ${CMAKE_CURRENT_BINARY_DIR}/resources/)
#endforeach ()
#foreach (item IN LISTS RESOURCE_FILES)
#    message(STATUS ${item})
#    add_custom_command(TARGET Aquarium
#            COMMAND ${CMAKE_COMMAND} -E copy_if_different
#            ${CMAKE_CURRENT_SOURCE_DIR}/${item}
#            ${CMAKE_CURRENT_BINARY_DIR})
#endforeach ()

#add_custom_target(data-target ALL DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/Images/*.png")
#set_target_properties(Aquarium PROPERTIES RESOURCE "${RESOURCE_FILES}")
