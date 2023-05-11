cmake_minimum_required(VERSION 3.21)
project(FinalProject C)

set(CMAKE_C_STANDARD 90)
set(GCC_COVERAGE_COMPILE_FLAGS "-Wall -ansi -pedantic" )
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )

add_executable(FinalProject GlobalDefinitions.h main.c Assembler.c Assembler.h  MacroLayout/MacroLayout.h MacroLayout/MacroLayout.c MacroLayout/MacroList.h MacroLayout/MacroList.c DataStructs/List/List.c DataStructs/List/List.h DataStructs/List/ListTester.c  DataStructs/List/ListTester.h DataStructs/Dictionary/Dictionary.c DataStructs/Dictionary/Dictionary.h LinkerLevelFilesWriter/LinkerLevelFilesWriter.c LinkerLevelFilesWriter/LinkerLevelFilesWriter.h LinkerLevelFilesWriter/BuildMemoryViewFromAmFile.c LinkerLevelFilesWriter/BuildMemoryViewFromAmFile.h DataStructs/operation/operation.c DataStructs/operation/operation.h DataStructs/command/commands.c DataStructs/command/commands.h DecodingOperations/decodingOperations.h DecodingOperations/decodingOperations.c Error/error.c Error/error.h LinkerLevelFilesWriter/MemoryViewBuilderTester.c LinkerLevelFilesWriter/MemoryViewBuilderTester.h LinkerLevelFilesWriter/outputTester.c LinkerLevelFilesWriter/outputTester.h)
