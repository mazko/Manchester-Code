@set GTEST_HOME=gtest-1.6.0
@set GMOCK_HOME=gmock-1.6.0

cl /EHsc /I%GTEST_HOME% /I%GTEST_HOME%/include -I%GMOCK_HOME% -I%GMOCK_HOME%/include main.cpp src/manchester/Man_Encode.cpp src/manchester/Man_Decode.cpp %GTEST_HOME%/src/gtest-all.cc %GMOCK_HOME%/src/gmock-all.cc

main.exe

@del *.obj main.exe
