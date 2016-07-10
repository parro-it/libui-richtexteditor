cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
set(CMAKE_OSX_DEPLOYMENT_TARGET "10.8")
set(MSVC_INCREMENTAL_DEFAULT ON)
if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE DEBUG CACHE STRING "" FORCE)
endif()

project(libui_richtexteditor)
option(BUILD_SHARED_LIBS "Whether to build libui_richtexteditor as a shared library or a static library" ON)

message(STATUS "CMAKE_SYSTEM_NAME          = ${CMAKE_SYSTEM_NAME}"         )
message(STATUS "CMAKE_CXX_COMPILER_ID      = ${CMAKE_CXX_COMPILER_ID}"     )
message(STATUS "CMAKE_CXX_COMPILER_VERSION = ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "CMAKE_COMPILER_IS_GNUCXX   = ${CMAKE_COMPILER_IS_GNUCXX}"  )

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/out")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/out")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/out")
set(CMAKE_PDB_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/out")

if(APPLE)

	set(_OSNAME darwin)
	set(_HASVERSION TRUE)
	set(_VERSION "A")

	set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
	set(CMAKE_INSTALL_RPATH "@executable_path/")
	set(CMAKE_MACOSX_RPATH TRUE)

elseif(WIN32)

	set(_OSNAME windows)
	set(CMAKE_C_STANDARD_LIBRARIES CACHE STRING "" FORCE)
	set(CMAKE_CXX_STANDARD_LIBRARIES CACHE STRING "" FORCE)

else()

	set(_OSNAME unix)
	set(_HASVERSION TRUE)
	set(_VERSION "0")
	set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
	set(CMAKE_INSTALL_RPATH "\$ORIGIN")

endif()

if(MSVC)
	set(_COMMON_CFLAGS
		/W4 /wd4100
		/bigobj /nologo
		/RTC1 /RTCs /RTCu
		/EHsc
	)

	set(_COMMON_LDFLAGS
		/LARGEADDRESSAWARE
		/NOLOGO
		/INCREMENTAL:NO
		/MANIFEST:NO
	)

else()
	set(_COMMON_CFLAGS
		-Wall -Wextra -pedantic
		-Wno-unused-parameter
		-Wno-switch
		-fvisibility=hidden
	)

	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --std=c99")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11")

	set(_COMMON_LDFLAGS
		-fvisibility=hidden

	)

	SET(CMAKE_SKIP_BUILD_RPATH  FALSE)
	SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)

	if(WIN32)
		list(APPEND _COMMON_LDFLAGS
			-static
			-static-libgcc
			-static-libstdc++
		)
	endif()

endif()

macro(_target_link_options_private _target)
	foreach(_opt IN LISTS ${ARGN})
		set_property(TARGET ${_target} APPEND_STRING PROPERTY
			LINK_FLAGS " ${_opt}")
	endforeach()
endmacro()

link_directories(node_modules/libui-npm)

add_subdirectory("common")
add_subdirectory("${_OSNAME}")

add_library(${_LIBUINAME} ${_SOURCES})

target_include_directories(${_LIBUINAME}
	PUBLIC .
	PRIVATE ${_INCLUDEDIRS})

target_compile_definitions(${_LIBUINAME}
	PRIVATE ${_LIBUI_DEFS})

target_compile_definitions(${_LIBUINAME}
	PRIVATE libui_EXPORTS)

target_compile_options(${_LIBUINAME}
	PUBLIC ${_COMMON_CFLAGS}
	PRIVATE ${_LIBUI_CFLAGS})

if(BUILD_SHARED_LIBS)
	target_link_libraries(${_LIBUINAME}
		PRIVATE ${_LIBUI_LIBS})
endif()

if(BUILD_SHARED_LIBS)
	_target_link_options_private(${_LIBUINAME}
		_COMMON_LDFLAGS
		_LIBUI_LDFLAGS)
endif()

if(NOT BUILD_SHARED_LIBS)
	_handle_static()
	target_compile_definitions(${_LIBUINAME}
		PUBLIC _UI_STATIC)
endif()

if(NOT MSVC)
	if(BUILD_SHARED_LIBS)
		if(NOT (${CMAKE_SYSTEM_NAME} STREQUAL FreeBSD))
			# on OS X we don't need to do this; Apple's linker warns about undefined symbols in -shared builds!
			if(NOT APPLE)
				target_link_libraries(ui_richtexteditor
					PRIVATE -Wl,--no-undefined -Wl,--no-allow-shlib-undefined
				)
			endif()
		endif()
	endif()
endif()

if(BUILD_SHARED_LIBS)
	if(_HASVERSION)
		set_target_properties(${_LIBUINAME} PROPERTIES
			SOVERSION "${_VERSION}")
	endif()
endif()

macro(_add_exec _name)
	add_executable(${_name}
		WIN32 EXCLUDE_FROM_ALL
		${ARGN})


	target_link_libraries(${_name}
		ui ui_richtexteditor ${_LIBUI_STATIC_RES})

	_target_link_options_private(${_name}
		_COMMON_LDFLAGS)

	if(BUILD_SHARED_LIBS)
		set_property(TARGET ${_name} PROPERTY
			POSITION_INDEPENDENT_CODE True)
	endif()

	if(NOT BUILD_SHARED_LIBS)
		target_link_libraries(${_name}
			${_LIBUI_LIBS})
	endif()

	if(NOT WIN32)
		target_include_directories(${_name}
			PUBLIC .)
		target_compile_options(${_name}
			PUBLIC ${_COMMON_CFLAGS})
	endif()
endmacro()

add_subdirectory("test")
add_subdirectory("examples")
