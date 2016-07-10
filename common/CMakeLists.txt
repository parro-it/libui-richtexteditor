list(APPEND _SOURCES
	common/richtexteditor.c
)
set(_SOURCES ${_SOURCES} PARENT_SCOPE)

list(APPEND _INCLUDEDIRS
	common
	node_modules/libui-npm
)

set(_INCLUDEDIRS ${_INCLUDEDIRS} PARENT_SCOPE)
