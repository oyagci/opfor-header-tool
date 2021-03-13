workspace "opfor-header-tool"
    architecture "x64"

    configurations {
        "Debug",
        "Release",
    }

    flags {
        "MultiProcessorCompile"
    }

    outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

    project "op4ht"
        location "op4ht"
        kind "ConsoleApp"
        language "C++"
        cppdialect "C++17"

        targetdir ("build/bin/" .. outputdir .. "%{prj.name}")
        objdir ("build/bin-int/" .. outputdir .. "%{prj.name}")

        files {
            "%{prj.name}/src/**.hpp",
            "%{prj.name}/src/**.h",
            "%{prj.name}/src/**.cpp",
        }

        includedirs {
            "%{prj.name}/src",
        }

        links {
        }

        filter "configurations:Debug"
            defines "OP4_DEBUG"
            symbols "On"

        filter "configurations:Release"
            defines "OP4_RELEASE"
            optimize "On"

        filter "system:windows"
            staticruntime "On"
            systemversion "latest"