macro(_add_example _name)
	_add_exec(${_name} ${ARGN})
	# because Microsoft's toolchain is dumb
	if(MSVC)
		set_property(TARGET ${_name} APPEND_STRING PROPERTY
			LINK_FLAGS " /ENTRY:mainCRTStartup")
	endif()
endmacro()

_add_example(rte
	rte/main.c
)

list(APPEND _INCLUDEDIRS
	examples/rte
	node_modules/libui-npm
)

set(_INCLUDEDIRS ${_INCLUDEDIRS} PARENT_SCOPE)

add_custom_target(examples DEPENDS rte)
