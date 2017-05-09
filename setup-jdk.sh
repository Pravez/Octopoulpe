#!/bin/sh

echo "Creating jdk directory ..."
mkdir -p jdk
cd jdk

if [ ! -d oracle-jdk8/bin ]
then
	if [ ! -f jdk-8u131-linux-x64.tar.gz ]
	then
		echo "Downloading Oracle jdk ..."
		sleep 1
		wget --no-check-certificate --no-cookies --header "Cookie: oraclelicense=accept-securebackup-cookie" http://download.oracle.com/otn-pub/java/jdk/8u131-b11/d54c1d3a095b4ff2b6607d096fa80163/jdk-8u131-linux-x64.tar.gz
	fi

	echo "Unpacking jdk ..."
	sleep 1
	mkdir -p oracle-jdk8 && tar xzfv jdk-8u131-linux-x64.tar.gz -C oracle-jdk8 --strip-components 1
fi

echo "Please execute these two lines to change your JAVA_HOME"
echo export JAVA_HOME=`pwd`/oracle-jdk8/
echo export PATH=\$JAVA_HOME/bin:\$PATH
