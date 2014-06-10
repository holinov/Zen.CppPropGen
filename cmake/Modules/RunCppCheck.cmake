# Запустить проверку CppCheck

MACRO(RUN_CPP_CHECK target sourceDirs excludeDirs)
	IF(CPP_CHECK)
		SET(check_name "${target}_cppcheck")
		SET(include "")

		foreach(incDir ${sourceDirs})
			IF(include)
		  		SET(include "${include} ${incDir}")
		  	else()
		  		SET(include "${incDir}")
		  	endif()
		endforeach()

		foreach(exclDir ${excludeDirs})
		  LIST(APPEND exclude "-i${exclDir}")
		endforeach()

		message("Making CppCheck target ${check_name} on ${include} excluding ${exclude}")
		IF(CPP_CHECK_HTML)
			#Создать HTML отчет о проверки исходинков
			ADD_CUSTOM_TARGET(CPPCHECK ALL cppcheck  --enable=all  -j 4 ${CMAKE_SOURCE_DIR}/src -i${CMAKE_SOURCE_DIR}/src/core/cli -i${CMAKE_SOURCE_DIR}/src/core/json -i${CMAKE_SOURCE_DIR}/src/core/libgit --xml --xml-version=2 2> err.xml)
			ADD_CUSTOM_TARGET(CPPCHECK_HTML ALL cppcheck-htmlreport  --title=${PROJECT_NAME} --file=err.xml --report-dir=${CPP_CHECK_REPORT} --source-dir=${CMAKE_SOURCE_DIR}/src)
			ADD_DEPENDENCIES(CPPCHECK_HTML CPPCHECK)
			ADD_DEPENDENCIES(console CPPCHECK_HTML)
		ELSE()
			#Генерация отчетов в виде понятном компиляторам	
			IF(NOT WIN32)
				SET(CPP_FORMAT "--template=gcc")
			ELSE()
				SET(CPP_FORMAT "--template=vs")
			ENDIF()	
			ADD_CUSTOM_TARGET(${check_name} ALL cppcheck  ${CPP_FORMAT} --enable=all  -j 4 ${sourceDirs} ${exclude} VERBATIM)
			ADD_DEPENDENCIES(${target} CPPCHECK)

			#ADD_CUSTOM_TARGET(CPPCHECK ALL cppcheck  ${CPP_FORMAT} --enable=all  -j 4 ${CMAKE_SOURCE_DIR}/src -i${CMAKE_SOURCE_DIR}/src/core/json -i${CMAKE_SOURCE_DIR}/src/core/libgit)
			#ADD_DEPENDENCIES(console CPPCHECK)
		ENDIF()
	ENDIF()
ENDMACRO()
