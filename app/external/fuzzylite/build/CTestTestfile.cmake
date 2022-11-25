# CMake generated Testfile for 
# Source directory: D:/git/project/HapticGamesTouchX/app/external/fuzzylite
# Build directory: D:/git/project/HapticGamesTouchX/app/external/fuzzylite/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
if(CTEST_CONFIGURATION_TYPE MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
  add_test(RunTests "D:/git/project/HapticGamesTouchX/app/external/fuzzylite/build/bin/Debug/fuzzylite-tests-debug.exe")
  set_tests_properties(RunTests PROPERTIES  _BACKTRACE_TRIPLES "D:/git/project/HapticGamesTouchX/app/external/fuzzylite/CMakeLists.txt;218;add_test;D:/git/project/HapticGamesTouchX/app/external/fuzzylite/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
  add_test(RunTests "D:/git/project/HapticGamesTouchX/app/external/fuzzylite/build/bin/Release/fuzzylite-tests.exe")
  set_tests_properties(RunTests PROPERTIES  _BACKTRACE_TRIPLES "D:/git/project/HapticGamesTouchX/app/external/fuzzylite/CMakeLists.txt;218;add_test;D:/git/project/HapticGamesTouchX/app/external/fuzzylite/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
  add_test(RunTests "D:/git/project/HapticGamesTouchX/app/external/fuzzylite/build/bin/MinSizeRel/fuzzylite-tests.exe")
  set_tests_properties(RunTests PROPERTIES  _BACKTRACE_TRIPLES "D:/git/project/HapticGamesTouchX/app/external/fuzzylite/CMakeLists.txt;218;add_test;D:/git/project/HapticGamesTouchX/app/external/fuzzylite/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
  add_test(RunTests "D:/git/project/HapticGamesTouchX/app/external/fuzzylite/build/bin/RelWithDebInfo/fuzzylite-tests.exe")
  set_tests_properties(RunTests PROPERTIES  _BACKTRACE_TRIPLES "D:/git/project/HapticGamesTouchX/app/external/fuzzylite/CMakeLists.txt;218;add_test;D:/git/project/HapticGamesTouchX/app/external/fuzzylite/CMakeLists.txt;0;")
else()
  add_test(RunTests NOT_AVAILABLE)
endif()
