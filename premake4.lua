solution "WorldView"
	configurations { "debug", "release" }

project "worldview"
	kind "ConsoleApp"
	language "C++"
	files { "src/**" }
	links { "noise", "sfml-graphics", "sfml-window", "sfml-system", "worldgen" }
	libdirs { "." }
	includedirs { "." }
	objdir "build/obj"
	buildoptions { "-std=c++11" }

	configuration "debug"
		flags { "Symbols", "ExtraWarnings" }

	configuration "release"
		flags { "Optimize" }

