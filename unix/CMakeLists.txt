# 3 june 2016

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK REQUIRED gtk+-3.0)

set(_LIBUINAME ui_richtexteditor PARENT_SCOPE)
if(NOT BUILD_SHARED_LIBS)
	set(_LIBUINAME libui-temporary PARENT_SCOPE)
endif()

macro(_handle_static)
	set_target_properties(${_LIBUINAME} PROPERTIES
		ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")

	set(_aname $<TARGET_FILE:${_LIBUINAME}>)
	set(_oname ui_richtexteditor-combined.o)
	add_custom_command(
		OUTPUT ${_oname}
		COMMAND
			ld -r --whole-archive ${_aname} -o ${_oname}
		COMMAND
			objcopy --localize-hidden ${_oname}
		COMMENT "Removing hidden symbols")

	add_library(ui_richtexteditor STATIC ${_oname})

	set_target_properties(libui PROPERTIES
		LINKER_LANGUAGE C)
	set(_aname)
	set(_oname)
endmacro()

# TODO the other variables don't work?
set(_LIBUI_CFLAGS
	${GTK_CFLAGS}
PARENT_SCOPE)

set(_LIBUI_LIBS
	${GTK_LDFLAGS} m dl ui
PARENT_SCOPE)

