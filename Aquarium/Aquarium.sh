#!/bin/sh

DIR=Aquarium_production

usage(){
    echo "Usage : -h or --help to print this help menu"
    echo "Output directory is : $DIR/"
    echo "Use --create option to create and compile sources in output directory"
    echo "Use --run to run the application"
    echo "Use --clean to remove output directory and its content"
    echo "/!\ DO NOT /!\ move output directory, or if you move it, move the folder resources/ relatively to it"
}

create(){
    echo "Creating production directory, or cleaning ..."
    mkdir -p  $DIR/
    rm -rf $DIR/*

    echo "Compiling java sources ..."
    javac -d $DIR/ -cp $DIR/ src/sample/*.java

    echo "Copying images ..."
    cp -r src/sample/Images $DIR/sample/

    echo "Done !"
}

run(){
    java -cp $DIR/ sample.Aquarium
}

clean(){
    rm -rf $DIR
}

if [ $# -ge 1 ]
then
    while [[ $# -ge 1 ]]
    do
    key="$1"


    case $key in
        --create)
        create
        ;;
        --run)
        run
        ;;
        -h|--help)
        usage
        ;;
        --clean)
        clean
        ;;
        *)
                echo "Unknown option"
        ;;
    esac
    shift # past argument or value
    done
else
    echo "Please give at least one option"
    usage
fi
