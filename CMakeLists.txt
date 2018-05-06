cmake_minimum_required(VERSION 3.0.0)
# cmake version 3.0.0 is needed for block comments, 2.8.11 was the previous
# min version

project(asteroids5)

# http://www.cmake.org/Wiki/CMake_Useful_Variables

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# this assumes you are running cmake from a build directory in the source directory
# cmake .. -G "Visual Studio 15 2017 Win64"
if(MSVC)
	ADD_DEFINITIONS(-D_SCL_SECURE_NO_WARNINGS)
	ADD_DEFINITIONS(-D_CRT_SECURE_NO_DEPRECATE)
	ADD_DEFINITIONS(-D_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS)
	ADD_DEFINITIONS(-DBOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE)
	#ADD_DEFINITIONS(-D_USE_MATH_DEFINES)
	
	# /Oi enable intrinsic functions
	# /GR- disable RTTI
	SET(CMAKE_C_FLAGS "/MP ${CMAKE_C_FLAGS}")
	SET(CMAKE_CXX_FLAGS "/MP /std:c++latest /Oi /GR- ${CMAKE_CXX_FLAGS}")
	
	find_package(OpenGL REQUIRED)
	
	set(VC_PREFIX "C:\\vclibs\\vclibs")
	
	set(LIBS ${LIBS} kernel32.lib)
	set(SDL_LIBS
		optimized "C:\\vclibs\\vclibs\\lib\\SDL2main.lib"
		debug "C:\\vclibs\\vclibs\\lib\\SDL2maind.lib")
	set(SDL_LIBS ${SDL_LIBS}
		optimized "C:\\vclibs\\vclibs\\lib\\SDL2.lib"
		debug "C:\\vclibs\\vclibs\\lib\\SDL2d.lib")
	set(GFX_LIBS
		optimized "C:\\vclibs\\vclibs\\lib\\SDL2_ttf.lib"
		debug "C:\\vclibs\\vclibs\\lib\\SDL2_ttfd.lib")
	set(GFX_LIBS ${GFX_LIBS}
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

if(MINGW OR MSYS)
	set(CMAKE_PREFIX_PATH "c:/msys64/mingw64")
	set(CMAKE_INCLUDE_PATH "c:/msys64/mingw64/include")
	set(CMAKE_LIBRARY_PATH "c:/msys64/mingw64/lib")
	
	find_package(Boost COMPONENTS program_options system filesystem REQUIRED)
	include_directories(${Boost_INCLUDE_DIRS})

	set(BOOST_LIBS ${Boost_LIBRARIES})
	
	INCLUDE(FindPkgConfig)
	PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
	include_directories(${SDL2_INCLUDE_DIR})
	set(SDL_LIBS
		${SDL2_LIBRARIES}
		)
	set(GFX_LIBS -lSDL2_ttf -lSDL2_image)
	
	include_directories("c:/msys64/mingw64/include/eigen3")
endif(MINGW OR MSYS)

if(APPLE)
	set(CMAKE_PREFIX_PATH "/usr/local")
	set(CMAKE_INCLUDE_PATH "/usr/local/include")
	set(CMAKE_LIBRARY_PATH "/usr/local/lib")
	
	find_package(Boost COMPONENTS program_options system filesystem REQUIRED)
	include_directories(${Boost_INCLUDE_DIRS})

	set(BOOST_LIBS ${Boost_LIBRARIES})
	set(LIBS
		-L/usr/local/lib
		${LIBS}
	)
	
	INCLUDE(FindPkgConfig)
	PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
	include_directories(${SDL2_INCLUDE_DIR})
	set(SDL_LIBS
		${SDL2_LIBRARIES}
		)
	set(GFX_LIBS -lSDL2_ttf -lSDL2_image)
	
	include_directories("/usr/local/include")
	include_directories("/usr/local/include/eigen3")
endif(APPLE)

# this probably only works on openSUSE Tumbleweed
if(UNIX AND NOT APPLE)
	set(CMAKE_PREFIX_PATH "/usr")
	set(CMAKE_INCLUDE_PATH "/usr/include")
	set(CMAKE_LIBRARY_PATH "/usr/lib")

	find_package(Boost COMPONENTS program_options system filesystem REQUIRED)
	include_directories(${Boost_INCLUDE_DIRS})
	
	INCLUDE(FindPkgConfig)
	PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
	include_directories(${SDL2_INCLUDE_DIR})
	set(SDL_LIBS
		${SDL2_LIBRARIES}
		)
	set(GFX_LIBS -lSDL2_ttf -lSDL2_image)

	set(BOOST_LIBS ${Boost_LIBRARIES})
	set(LIBS ${LIBS} ${Boost_LIBRARIES})
 
	include_directories("/usr/include")
	include_directories("/usr/include/eigen3")

endif(UNIX AND NOT APPLE)

include_directories("../common-cpp")

if(NOT MSVC)
	# --------------------------------------------------------------------------
	# WARNING: all warnings are disabled for GCC
	# --------------------------------------------------------------------------
	
	# better optimizations
	# -O3 still IEEE754 compliant
	# -Ofast not IEEE754 compliant, faster than -O3
	# -ffast-math not needed with -Ofast
	# -ftree-vectorize not sure how well this does
	# -mavx
	# -mavx2
	set(CMAKE_C_FLAGS_RELEASE "-O3")
	set(CMAKE_CXX_FLAGS_RELEASE "-O3 -w")
	
	# force some better debugging
	# this should be gcc
	if(NOT APPLE)
		set(CMAKE_C_FLAGS_DEBUG "-ggdb3 -Og")
		set(CMAKE_CXX_FLAGS_DEBUG "-ggdb3 -Og -w")
	endif(NOT APPLE)
	# and this should be clang
	if(APPLE)
		set(CMAKE_C_FLAGS_DEBUG "-O0 -g3")
		set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g3")
	endif(APPLE)

	# enable C11/C++14 for gcc and clang
	set(EXTRA_CXX_FLAGS "-std=c++14 -Wno-deprecated-declarations -pipe")
	set(EXTRA_C_FLAGS "-std=c11 -Wno-deprecated-declarations -pipe")
	
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${EXTRA_C_FLAGS}")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${EXTRA_CXX_FLAGS}")
	
	# pipe keeps compilation in memory
	set(CMAKE_LD_FLAGS "-pipe")
endif(NOT MSVC)

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

# static libraries don't work so well on Apple for some reason
if(APPLE)
	set(LIBRARY_TYPE SHARED)
else(APPLE)
	set(LIBRARY_TYPE STATIC)
endif(APPLE)

add_library(common_events ${LIBRARY_TYPE} ${COMMON_EVENTS_SOURCE})
add_library(common_fox ${LIBRARY_TYPE} ${COMMON_FOX_SOURCE})
target_link_libraries(common_fox ${LIBS})
add_library(common_fox_gfx ${LIBRARY_TYPE} ${COMMON_FOX_GFX_SOURCE})
target_link_libraries(common_fox_gfx ${LIBS} ${SDL_LIBS} ${GFX_LIBS})
#add_library(sound ${LIBRARY_TYPE} ${SOUND_SOURCE})
#target_link_libraries(sound ${LIBS} ${SDL_LIBS} ${SOUND_LIBS} common_events)
add_library(gcore ${LIBRARY_TYPE} ${GCORE_SOURCE})
target_link_libraries(gcore ${LIBS} ${BOOST_LIBS} common_events common_fox)
add_executable(${PROJECT_NAME} ${MAIN_SOURCE})
target_link_libraries(${PROJECT_NAME} ${LIBS} ${SDL_LIBS} common_fox common_fox_gfx common_events gcore)

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

