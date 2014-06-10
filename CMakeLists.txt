cmake_minimum_required(VERSION 2.8)
project(CppPropertyGenerator)

#Параметры сборки
OPTION( CPP_CHECK			"Run CppCheck"			 				OFF)
OPTION( CPP_CHECK_HTML		"Get HTML CppCheck reports"			 	OFF)
SET( CPP_CHECK_REPORT "code_report" CACHE STRING "CppCheck report dir") 

#будет заполненно вложенными проектами
SET(TARGETS_TO_CHECK "")
INCLUDE(cmake/Modules/RunCppCheck.cmake)

ADD_SUBDIRECTORY(src)


IF(CPP_CHECK)
	string(REPLACE " " ";" TARGETS_TO_CHECK_LIST ${TARGETS_TO_CHECK})
	foreach(chkTarget ${TARGETS_TO_CHECK_LIST})
		SET(chkInc  ${${chkTarget}_inc})
		SET(chkExcl ${${chkTarget}_exc})
		RUN_CPP_CHECK(${chkTarget} "${chkInc}" "${chkExcl}")
	endforeach()
ENDIF()