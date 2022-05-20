include "FolderStructure.lua"

workspace (solutionname)
    language "C++"
    cppdialect "C++17"
    architecture "x64"
    systemversion "latest"
    characterset "Unicode"
    startproject (appname)

    configurations
    {
        "Debug",
        "Profiling",
        "Release"
    }

    outputdir = "%{cfg.buildcfg}"

project (corename)
    kind "SharedLib"
    staticruntime "Off"
    location (corename)
    
    targetdir ("bin/" ..outputdir.. "/%{prj.name}")
    objdir ("bin-int/" ..outputdir.. "/%{prj.name}")
    
    pchheader "corepch.h"
    pchsource (corename.."/src/corepch.cpp")
    
    defines 
    {
        "CORE_BUILD_DLL"
    }

    links
    {
        "d3d12",
        "dxgi",
        "d3dcompiler"
    }

    files
    {
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/src/**.h"
    }

    includedirs
    {
        "%{prj.name}/src/",
        "%{prj.name}/vendor/spdlog/include",
        "%{prj.name}/vendor/d3d12/include"
    }

    postbuildcommands
    {
        ("{COPY} %{cfg.buildtarget.relpath} \"../bin/" ..outputdir.. "/"..appname.."/\"")
    }

    filter "configurations:Debug"
        defines "CORE_DEBUG"
        runtime "Debug"
        symbols "On"

    filter "configurations:Profiling"
        defines "CORE_PROFILING"
        runtime "Release"
        optimize "On"

    filter "configurations:Release"
        defines "CORE_RELEASE"
        runtime "Release"
        optimize "On"

project (appname)
    kind "ConsoleApp"
    staticruntime "Off"
    location (appname)

    targetdir ("bin/" ..outputdir.. "/%{prj.name}")
    objdir ("bin-int/" ..outputdir.. "/%{prj.name}")

    links
    {
        corename
    }

    files
    {
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/src/**.h"
    }

    includedirs
    {
        "%{prj.name}/src/",
        corename.."/src/",
        corename.."/vendor/spdlog/include",
        corename.."/vendor/d3d12/include"
    }

    filter "configurations:Debug"
    defines "CORE_DEBUG"
    runtime "Debug"
    symbols "On"

filter "configurations:Profiling"
    defines "CORE_PROFILING"
    runtime "Release"
    optimize "On"

filter "configurations:Release"
    kind "WindowedApp"
    defines "CORE_RELEASE"
    runtime "Release"
    optimize "On"
