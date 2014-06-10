cmake_minimum_required(VERSION 2.8)
project(generator)

file(GLOB SourceFiles "*.cpp" "*.h")
add_executable(cpppropgen ${SourceFiles})

#Передать наверх для прповерки
SET(TARGETS_TO_CHECK "cpppropgen ${TARGETS_TO_CHECK}" PARENT_SCOPE)
SET(generator_exc "" PARENT_SCOPE)
SET(generator_inc ${SourceFiles} PARENT_SCOPE)

INSTALL(TARGETS cpppropgen DESTINATION bin)

