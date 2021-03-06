#!/bin/sh

SCRIPT=$(readlink -f $0)
SCRIPTPATH=`dirname $SCRIPT`
EXECUTABLE_NAME=""

usage(){
    echo "Help : (run -h to get this menu)"
    echo "Run with -p option to ask to this script to prepare everything (create build directory, cmake ..)"
    echo "Run with -c option to ask to compile sources"
    echo "Run with -r [executable name] to ask to run an executable"
    echo "Executables are : \" Server \" for the Server in C, and \" Aquarium \" for the View in Java"
    echo "If you want to clean : you can use \"--clean\" option to execute a make clean, or if you want to remove even the build directory, use \"--clean-everything\" "
}

prepare(){
    echo "Creating build directory ..."
    mkdir -p $SCRIPTPATH/build
    echo "Moving to build/ ..."
    cd $SCRIPTPATH/build
    echo "Executing cmake ..."
    cmake ../
}

compile(){
    if [ -d $SCRIPTPATH/build ]
    then
        echo "Moving to build directory"
        cd $SCRIPTPATH/build
        if [ -f Makefile ]
        then
            echo "Compiling ..."
            make all
        else
            echo "No makefile, are you sure you runned \"-p\" option first ?"
        fi
    else
        echo "No build directory, are you sure you runned \"-p\" option first ?"
    fi
}

execute(){
    runned=false

    { if [ -d $SCRIPTPATH/build ]
    then
        echo "Moving to build directory"
        cd $SCRIPTPATH/build
        if [ "$EXECUTABLE_NAME" == "Server" ] && [ -f $SCRIPTPATH/build/Poseidon/src/Server ]
        then
            echo "Running Server ..."
            cd $SCRIPTPATH/build/Poseidon/src
            ./Server
            runned=true
        fi
        if [ "$EXECUTABLE_NAME" == "Aquarium" ] && [ -f $SCRIPTPATH/build/Aquarium/src/Aquarium.jar ]
        then
            echo "Running aquarium ..."
            cd $SCRIPTPATH/build/Aquarium/src
            java -cp Aquarium.jar sample.Aquarium
            runned=true
        fi

        if [ ${runned} == false ]
        then
            echo "Please be sure to give a correct executable name"
        fi
    else
        echo "No build directory, are you sure you runned \"-p\" option first ?"
    fi }
}

test(){
    runned=false

    { if [ -d $SCRIPTPATH/build ]
    then
        echo "Moving to build directory"
        cd $SCRIPTPATH/build
        if [ -f $SCRIPTPATH/build/Poseidon/tst/Test_intern_protocol ]
        then
            cd $SCRIPTPATH/build/Poseidon/tst
            ./Test_intern_protocol
            runned=true
        fi
        if [ -f $SCRIPTPATH/build/Poseidon/tst/Test_parser ]
        then
            cd $SCRIPTPATH/build/Poseidon/tst
            ./Test_parser
            runned=true
        fi

        if [ ${runned} == false ]
        then
            echo "Please be sure you compiled test executables"
        fi
    else
        echo "No build directory, are you sure you runned \"-p\" option first ?"
    fi }
}

clean(){
	if [ -d $SCRIPTPATH/build ]
    	then
        	echo "Moving to build directory"
        	cd $SCRIPTPATH/build
        	if [ -f Makefile ]
        	then
            		echo "Cleaning ..."
            		make clean
        	else
            		echo "No makefile, are you sure you runned \"-p\" option 
first ?"
        	fi
    	else
        	echo "No build directory, are you sure you runned \"-p\" option first ?"
    	fi
}

if [ $# -ge 1 ]
then
    while [[ $# -ge 1 ]]
    do
    key="$1"


    case $key in
        -p|--prepare)
        prepare
        ;;
        -c|--compile)
        compile
        ;;
        -t|--test)
        test
        ;;
        -r|--run)
        EXECUTABLE_NAME="$2"
        execute
        shift
        ;;
        -h|--help)
        usage
        ;;
	--clean)
	clean
	;;
	--clean-everything)
	rm -rf build/
	shift
	;;
        *)
                echo "Unknown option"
        ;;
    esac
    shift
    done
else
    echo "Please give at least one option"
    usage
fi
