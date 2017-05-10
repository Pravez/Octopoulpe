set(JAVA_SOURCES sample/Aquarium.java
        sample/Communicator.java
        sample/Console.java
        sample/Fish.java
        sample/Parser.java
        sample/ReceiveHandler.java
        sample/ScenarioWindow.java)

set(JAVA_RESOURCES sample/Images/bb.png
                   sample/Images/bg.png
                   sample/Images/PoissonClown.png
                   sample/Images/PoissonClown2.png
                   sample/Images/PoissonDore.png
                   sample/Images/PoissonDore2.png
                   sample/Images/PoissonNain.png
                   sample/Images/PoissonNain2.png
                   sample/Images/PoissonRouge.png
                   sample/Images/PoissonRouge2.png)

add_jar(Aquarium ${JAVA_SOURCES} ${JAVA_RESOURCES})

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR})