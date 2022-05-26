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

    IncludeDir = {}
    IncludeDir["spdlog"] = "%{wks.location}/"..corename.."/vendor/spdlog/include"
    IncludeDir["ImGui"] = "%{wks.location}/"..corename.."/vendor/imgui"
    IncludeDir["entt"] = "%{wks.location}/"..corename.."/vendor/entt/include"

    include (corename.."/vendor/imgui")

project (corename)
    kind "StaticLib"
    staticruntime "On"
    location (corename)
    
    targetdir ("bin/" ..outputdir.. "/%{prj.name}")
    objdir ("bin-int/" ..outputdir.. "/%{prj.name}")
    
    pchheader "corepch.h"
    pchsource (corename.."/src/corepch.cpp")
    
    defines 
    {
        
    }

    links
    {
        "d3d11",
        "dxgi",
        "d3dcompiler",
        "ImGui"
    }

    files
    {
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/src/**.h"
    }

    includedirs
    {
        "%{prj.name}/src/",
        "%{IncludeDir.spdlog}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.entt}"
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
    staticruntime "On"
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
        "%{IncludeDir.spdlog}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.entt}"
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
