# 3 june 2016

if(WIN32)
	set(_TEST_RESOURCES_RC resources.rc)
endif()

_add_exec(tester
	main.c

	${_TEST_RESOURCES_RC}
)
target_include_directories(tester
	PRIVATE test
)
set_target_properties(tester PROPERTIES
	OUTPUT_NAME test
	WIN32_EXECUTABLE FALSE
)
