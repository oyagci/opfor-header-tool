workspace "opfor-header-tool"

    IncludeDir = {}
    IncludeDir["fmt"] = "op4ht/vendor/fmt/include"
    IncludeDir["cxxopts"] = "op4ht/vendor/cxxopts/include"

    architecture "x64"

    configurations {
        "Debug",
        "Release",
    }

    flags {
        "MultiProcessorCompile"
    }

    outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

    include "op4ht/vendor/fmt"

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
            "%{IncludeDir.fmt}",
            "%{IncludeDir.cxxopts}",
        }

        links {
            "fmt"
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