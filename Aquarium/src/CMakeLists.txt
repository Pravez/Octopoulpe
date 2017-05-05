set(JAVA_SOURCES sample/Aquarium.java
                 sample/Console.java
                sample/Controller.java
                sample/Fish.java)

add_jar(Aquarium ${JAVA_SOURCES})

set(RESOURCE_FILES sample/Images/bb.png
        sample/Images/bg.png
        sample/Images/magicarpe.png
        sample/Images/magicarpe2.png
        sample/Images/nemo.png
        sample/Images/nemo2.png
        sample/Images/poulpy.png
        sample/Images/poulpy2.png
        sample/Images/shiny.png
        sample/Images/shiny2.png)

#foreach(item IN LISTS RESOURCE_FILES)
#    message(STATUS ${item})
#    add_custom_command(
#        OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${item}"
#        COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/${item}" #"${CMAKE_CURRENT_BINARY_DIR}/${item}"
#        DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/${item}"
#    )
#endforeach()

#add_custom_target(data-target ALL DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/Images/*.png")
#set_target_properties(Aquarium PROPERTIES RESOURCE "${RESOURCE_FILES}")
