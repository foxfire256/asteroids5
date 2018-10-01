cmake_minimum_required(VERSION 3.0.0)
# cmake version 3.0.0 is needed for block comments, 2.8.11 was the previous
# min version

project(asteroids5)

# http://www.cmake.org/Wiki/CMake_Useful_Variables

set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories("../common-cpp")

# this assumes you are running cmake from a build directory in the source directory
# cmake .. -G "Visual Studio 15 2017 Win64"
if(MSVC)
	ADD_DEFINITIONS(-D_SCL_SECURE_NO_WARNINGS)
	ADD_DEFINITIONS(-D_CRT_SECURE_NO_DEPRECATE)
	ADD_DEFINITIONS(-D_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS)
	ADD_DEFINITIONS(-DBOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE)
	#ADD_DEFINITIONS(-D_USE_MATH_DEFINES)
	# WARNING: the following do not work with boost program_options
	#ADD_DEFINITIONS(-DBOOST_NO_RTTI)
	#ADD_DEFINITIONS(-DBOOST_NO_TYPEID)
	
	# /Oi enable intrinsic functions
	# /GR- disable RTTI, can't do with boost program_options
	# /std:c++17 or /std:c++14 are options
	SET(CMAKE_C_FLAGS "/MP ${CMAKE_C_FLAGS}")
	SET(CMAKE_CXX_FLAGS "/MP /std:c++latest /Oi ${CMAKE_CXX_FLAGS}")
	
	find_package(OpenGL REQUIRED)
	
	set(VC_PREFIX "C:\\vclibs\\vclibs")
	
	set(LIBS ${LIBS} kernel32.lib)
	set(SDL_LIBS
		optimized "C:\\vclibs\\vclibs\\lib\\SDL2main.lib"
		debug "C:\\vclibs\\vclibs\\lib\\SDL2maind.lib")
	set(SDL_LIBS ${SDL_LIBS}
		optimized "C:\\vclibs\\vclibs\\lib\\SDL2.lib"
		debug "C:\\vclibs\\vclibs\\lib\\SDL2d.lib")
	set(SDL_LIBS ${SDL_LIBS}
		optimized "C:\\vclibs\\vclibs\\lib\\SDL2_ttf.lib"
		debug "C:\\vclibs\\vclibs\\lib\\SDL2_ttfd.lib")
	set(SDL_LIBS ${SDL_LIBS}
		optimized "C:\\vclibs\\vclibs\\lib\\SDL2_image.lib"
		debug "C:\\vclibs\\vclibs\\lib\\SDL2_imaged.lib")
	
	set(BOOST_LIBS ${BOOST_LIBS}
		optimized "C:\\vclibs\\vclibs\\lib\\libboost_program_options-vc141-mt-x64-1_66.lib"
		debug "C:\\vclibs\\vclibs\\lib\\libboost_program_options-vc141-mt-gd-x64-1_66.lib")
	set(BOOST_LIBS ${BOOST_LIBS}
		optimized "C:\\vclibs\\vclibs\\lib\\libboost_system-vc141-mt-x64-1_66.lib"
		debug "C:\\vclibs\\vclibs\\lib\\libboost_system-vc141-mt-gd-x64-1_66.lib")
	set(BOOST_LIBS ${BOOST_LIBS}
		optimized "C:\\vclibs\\vclibs\\lib\\libboost_filesystem-vc141-mt-x64-1_66.lib"
		debug "C:\\vclibs\\vclibs\\lib\\libboost_filesystem-vc141-mt-gd-x64-1_66.lib")
	
	set(LIBS ${LIBS} ${BOOST_LIBS})
	
	include_directories("C:\\vclibs\\vclibs\\include")
endif(MSVC)

if(MINGW OR MSYS OR UNIX OR APPLE)
	if(APPLE)
		set(CMAKE_PREFIX_PATH "/usr/local")
		set(CMAKE_INCLUDE_PATH "/usr/local/include")
		set(CMAKE_LIBRARY_PATH "/usr/local/lib")
	elseif(UNIX AND NOT APPLE)
		set(CMAKE_PREFIX_PATH "/usr")
		set(CMAKE_INCLUDE_PATH "/usr/include")
		set(CMAKE_LIBRARY_PATH "/usr/lib")
	elseif(MINGW OR MSYS)
		set(CMAKE_PREFIX_PATH "c:/msys64/mingw64")
		set(CMAKE_INCLUDE_PATH "c:/msys64/mingw64/include")
		set(CMAKE_LIBRARY_PATH "c:/msys64/mingw64/lib")
	endif(APPLE)
	
	if(APPLE)
		set(LIBS ${LIBS} -L/usr/local/lib)
	elseif(UNIX AND NOT APPLE)
		set(LIBS ${LIBS})
	elseif(MINGW OR MSYS)
		set(LIBS ${LIBS} -lmingw32)
	endif(APPLE)
	
	find_package(Boost COMPONENTS program_options system filesystem REQUIRED)
	include_directories(${Boost_INCLUDE_DIRS})
	set(BOOST_LIBS ${Boost_LIBRARIES})
	
	set(SDL2_LIBS -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image)
	set(SOUND_LIBS -lSDL2_mixer -lvorbis -logg -lz)
	
	if(APPLE)
		include_directories("/usr/local/opt/qt/include")
		include_directories("/usr/local/include")
		include_directories("/usr/local/include/eigen3")
	elseif(UNIX AND NOT APPLE)
		include_directories("/usr/include")
		include_directories("/usr/include/eigen3")
	elseif(MINGW OR MSYS)
		include_directories("c:/msys64/mingw64/include/eigen3")
		# -mwindows makes the console box go away
		#set(SDL2_LIBS ${SDL2_LIBS} -mwindows)
	endif(APPLE)
	
	# better optimizations
	# -O3 still IEEE754 compliant
	# -Ofast not IEEE754 compliant, faster than -O3
	# -ffast-math not needed with -Ofast
	# -ftree-vectorize not sure how well this does
	# -mavx
	# -mavx2
	set(CMAKE_C_FLAGS_RELEASE "-Ofast")
	set(CMAKE_CXX_FLAGS_RELEASE "-Ofast")
	
	# force some better debugging
	# this should be gcc
	# WARNING: warnings are disabled
	if(NOT APPLE)
		set(CMAKE_C_FLAGS_DEBUG "-ggdb3 -Og")
		set(CMAKE_CXX_FLAGS_DEBUG "-ggdb3 -Og -w")
	endif(NOT APPLE)
	# and this should be clang
	# WARNING: warnings are disabled on clang
	if(APPLE)
		set(CMAKE_C_FLAGS_DEBUG "-O0 -g3")
		set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g3 -w")
	endif(APPLE)

	# enable C11/C++17 for gcc and clang
	set(EXTRA_CXX_FLAGS "-std=c++17 -Wno-deprecated-declarations -pipe")
	set(EXTRA_C_FLAGS "-std=c17 -Wno-deprecated-declarations -pipe")
	
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${EXTRA_C_FLAGS}")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${EXTRA_CXX_FLAGS}")
	
	# pipe keeps compilation in memory
	set(CMAKE_LD_FLAGS "-pipe")
endif(MINGW OR MSYS OR UNIX OR APPLE)

set(MAIN_SOURCE
	main.cpp
	gfx/gfx.hpp
	gfx/gfx.cpp
	main_observer.hpp
	main_observer.cpp
	sdl_message_handler.hpp
	sdl_message_handler.cpp
)

set(COMMON_EVENTS_SOURCE
	../common-cpp/events/console_writer.hpp
	../common-cpp/events/console_writer.cpp
	../common-cpp/events/message_base.hpp
	../common-cpp/events/message_info.hpp
	../common-cpp/events/message_error.hpp
	../common-cpp/events/manager_interface.hpp
	../common-cpp/events/manager.hpp
	../common-cpp/events/manager.cpp
	../common-cpp/events/message_framerate.hpp
	../common-cpp/events/message_keydown.hpp
	../common-cpp/events/message_keyup.hpp
	../common-cpp/events/message_mousebutton.hpp
	../common-cpp/events/message_mousemotion.hpp
	../common-cpp/events/message_mousewheel.hpp
	../common-cpp/events/message_resize.hpp
	../common-cpp/events/observer.hpp
	../common-cpp/events/observer.cpp
	../common-cpp/events/subject.hpp
	../common-cpp/events/subject.cpp
)

set(COMMON_FOX_SOURCE
	../common-cpp/fox/counter.hpp
	../common-cpp/fox/counter.cpp
)

set(COMMON_FOX_GFX_SOURCE
	../common-cpp/fox/gfx/font_factory.cpp
	../common-cpp/fox/gfx/font_factory.hpp
	../common-cpp/fox/gfx/font_config.hpp
	../common-cpp/fox/gfx/font.hpp
	../common-cpp/fox/gfx/font.cpp
	../common-cpp/fox/gfx/font_texture.hpp
	../common-cpp/fox/gfx/font_texture.cpp
	../common-cpp/fox/gfx/font_variant.hpp
	../common-cpp/fox/gfx/texture_factory.cpp
	../common-cpp/fox/gfx/texture_factory.hpp
	../common-cpp/fox/gfx/texture_config.hpp
	../common-cpp/fox/gfx/texture.cpp
	../common-cpp/fox/gfx/texture.hpp
)

set(GCORE_SOURCE
	gcore/world.hpp
	gcore/world.cpp
	gcore/space_object.cpp
	gcore/space_object.hpp
	gcore/bullet.cpp
	gcore/bullet.hpp
	gcore/asteroid.cpp
	gcore/asteroid.hpp
	gcore/ship.cpp
	gcore/ship.hpp
	)

add_executable(${PROJECT_NAME} ${MAIN_SOURCE}
	${COMMON_EVENTS_SOURCE}
	${COMMON_FOX_SOURCE}
	${COMMON_FOX_GFX_SOURCE}
	${GCORE_SOURCE}
	)
target_link_libraries(${PROJECT_NAME} ${LIBS} ${SDL2_LIBS} ${BOOST_LIBS})

MESSAGE( STATUS "MINGW: " ${MINGW} )
MESSAGE( STATUS "MSYS: " ${MSYS} )
MESSAGE( STATUS "MSVC: " ${MSVC} )
MESSAGE( STATUS "APPLE: " ${APPLE} )
MESSAGE( STATUS "INCLUDE_DIRECTORIES: " ${INCLUDE_DIRECTORIES} )
MESSAGE( STATUS "CMAKE_C_FLAGS: " ${CMAKE_C_FLAGS} )
MESSAGE( STATUS "CMAKE_C_FLAGS_DEBUG: " ${CMAKE_C_FLAGS_DEBUG} )
MESSAGE( STATUS "CMAKE_C_FLAGS_RELEASE: " ${CMAKE_C_FLAGS_RELEASE} )
MESSAGE( STATUS "CMAKE_CXX_FLAGS: " ${CMAKE_CXX_FLAGS} )
MESSAGE( STATUS "CMAKE_CXX_FLAGS_DEBUG: " ${CMAKE_CXX_FLAGS_DEBUG} )
MESSAGE( STATUS "CMAKE_CXX_FLAGS_RELEASE: " ${CMAKE_CXX_FLAGS_RELEASE} )

