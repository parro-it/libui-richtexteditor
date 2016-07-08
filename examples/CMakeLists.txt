# 3 june 2016

macro(_add_example _name)
	_add_exec(${_name} ${ARGN})
	# because Microsoft's toolchain is dumb
	if(MSVC)
		set_property(TARGET ${_name} APPEND_STRING PROPERTY
			LINK_FLAGS " /ENTRY:mainCRTStartup")
	endif()
endmacro()

_add_example(controlgallery
	main.c
	${_EXAMPLE_RESOURCES_RC}
)


