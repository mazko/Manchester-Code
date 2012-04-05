#!/bin/bash

cd "`dirname "${0}"`"

#When a source file is compiled with -g, the compiler attaches DWARF3 debugging information which describes the location of all stack and global arrays in the file

#g++ -g -I$GTEST_HOME -I$GTEST_HOME/include -I$GMOCK_HOME -I$GMOCK_HOME/include -pthread main.cpp src/manchester/Man_Encode.cpp src/manchester/Man_Decode.cpp $GTEST_HOME/src/gtest-all.cc $GMOCK_HOME/src/gmock-all.cc

GTEST_HOME=gtest-1.6.0
GMOCK_HOME=gmock-1.6.0

NOEXISTS=1;
test -e "libgmock.a";
NOEXISTS=$?;
if [ $NOEXISTS -ne 0 ]
then 
	echo "compiling 'gtest-all.o'"
	g++ -g -I$GTEST_HOME -I$GTEST_HOME/include -c $GTEST_HOME/src/gtest-all.cc

	echo "compiling 'libgmock.a'"
	g++ -g -I$GTEST_HOME -I$GTEST_HOME/include -I$GMOCK_HOME -I$GMOCK_HOME/include -c $GMOCK_HOME/src/gmock-all.cc
  	ar -rv libgmock.a gtest-all.o gmock-all.o

	rm gtest-all.o gmock-all.o
fi

#compile tests

g++ -g -I$GTEST_HOME -I$GTEST_HOME/include -I$GMOCK_HOME -I$GMOCK_HOME/include -pthread main.cpp src/manchester/Man_Encode.cpp src/manchester/Man_Decode.cpp libgmock.a

#run tests

./a.out

rm -f a.out
