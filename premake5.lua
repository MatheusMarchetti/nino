include "FolderStructure.lua"

workspace (solutionname)
    language "C++"
    cppdialect "C++17"
    architecture "x64"
    systemversion "latest"
    characterset "MBCS"
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
    IncludeDir["DirectXTK"] = "%{wks.location}/"..corename.."/vendor/DirectXTK/Inc"

    include (corename.."/vendor/imgui")

project (corename)
    kind "StaticLib"
    staticruntime "On"
    location (corename)
    buildoptions "/MTd"
    
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
        "dxguid",
        "ImGui",
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
        "%{IncludeDir.entt}",
        "%{IncludeDir.DirectXTK}"
    }

    postbuildcommands
    {

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
    buildoptions "/MTd"

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
        "%{IncludeDir.entt}",
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
