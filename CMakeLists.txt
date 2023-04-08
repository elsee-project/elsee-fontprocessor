cmake_minimum_required(VERSION 3.7.2)
project(FontProcessor)
set(FONTPROCINC include)
set(HEADERS
${FONTPROCINC}/FontEngineInit.h
${FONTPROCINC}/FontMachineRoutines.h
${FONTPROCINC}/ProgramSetup.h
${FONTPROCINC}/ScanConverter.h
${FONTPROCINC}/FontData/cmap.h
${FONTPROCINC}/FontData/FontTableSetup.h
${FONTPROCINC}/FontData/TrueFontTypes.h)

add_library(Instructions STATIC Instructions/Instructions.c
Instructions/InstructionGroups/FunctionGroupZero.c
Instructions/InstructionGroups/FunctionGroupOne.c
Instructions/InstructionGroups/FunctionGroupTwo.c
Instructions/InstructionGroups/FunctionGroupThree.c
Instructions/InstructionGroups/FunctionGroupFour.c
Instructions/InstructionGroups/FunctionGroupFive.c
Instructions/InstructionGroups/FunctionGroupSix.c
Instructions/InstructionGroups/FunctionGroupSeven.c
Instructions/InstructionGroups/FunctionGroupEight.c
Instructions/InstructionGroups/FunctionGroupCD.c
Instructions/InstructionGroups/FunctionGroupEF.c
${HEADERS})
target_include_directories(Instructions PRIVATE ${FONTPROCINC})
add_library(FontProcessor STATIC fontprocesssor.c ${HEADERS})
target_include_directories(FontProcessor PRIVATE ${FONTPROCINC})
target_link_libraries(FontProcessor Instructions)
target_link_libraries(Instructions FontProcessor)
