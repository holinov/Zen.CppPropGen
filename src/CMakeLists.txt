cmake_minimum_required(VERSION 2.8)
project(generator)

file(GLOB SourceFiles "*.cpp" "*.h" "*.hpp")
add_executable(cpppropgen ${SourceFiles})

#Передать наверх для прповерки
SET(TARGETS_TO_CHECK "cpppropgen ${TARGETS_TO_CHECK}" PARENT_SCOPE)
SET(cpppropgen_exc "" PARENT_SCOPE)
SET(cpppropgen_inc ${SourceFiles} PARENT_SCOPE)

INSTALL(TARGETS cpppropgen DESTINATION bin)

