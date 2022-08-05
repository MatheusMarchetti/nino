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

    flags
    {
        "MultiProcessorCompile"
    }

    outputdir = "%{cfg.buildcfg}"

    IncludeDir = {}
    IncludeDir["spdlog"] = "%{wks.location}/"..corename.."/vendor/spdlog/include"
    IncludeDir["ImGui"] = "%{wks.location}/"..corename.."/vendor/imgui"
    IncludeDir["entt"] = "%{wks.location}/"..corename.."/vendor/entt/include"
    IncludeDir["DirectXTK"] = "%{wks.location}/"..corename.."/vendor/DirectXTK/Inc"
    IncludeDir["DirectXTex"] = "%{wks.location}/"..corename.."/vendor/DirectXTex"
    IncludeDir["inih"] = "%{wks.location}/"..corename.."/vendor/inih"
    IncludeDir["assimp"] = "%{wks.location}/"..corename.."/vendor/assimp/include"

    include (corename.."/vendor/imgui")
    include (corename.."/vendor/DirectXTK")
    include (corename.."/vendor/DirectXTex")
    include (corename.."/vendor/assimp")

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
        "NINO_API_D3D11"
    }

    links
    {
        "d3d11",
        "dxgi",
        "d3dcompiler",
        "dxguid",
        "ImGui",
        "DirectXTK",
        "DirectXTex",
        "assimp"
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
        "%{IncludeDir.DirectXTK}",
        "%{IncludeDir.DirectXTex}",
        "%{IncludeDir.inih}",
        "%{IncludeDir.assimp}"
    }

    postbuildcommands
    {

    }

    filter "configurations:Debug"
        defines "CORE_DEBUG"
        runtime "Debug"
        symbols "On"
        buildoptions "/MTd"

    filter "configurations:Profiling"
        defines "CORE_PROFILING"
        runtime "Release"
        optimize "On"
        buildoptions "/MT"

    filter "configurations:Release"
        defines "CORE_RELEASE"
        runtime "Release"
        optimize "On"
        buildoptions "/MT"

project (appname)
    kind "ConsoleApp"
    staticruntime "On"
    location (appname)
    buildoptions "/MT"

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
        "%{IncludeDir.DirectXTK}",
        "%{IncludeDir.DirectXTex}"
    }

    filter "configurations:Debug"
    defines "CORE_DEBUG"
    runtime "Debug"
    symbols "On"
    buildoptions "/MTd"

filter "configurations:Profiling"
    defines "CORE_PROFILING"
    runtime "Release"
    optimize "On"
    buildoptions "/MT"

filter "configurations:Release"
    kind "WindowedApp"
    defines "CORE_RELEASE"
    runtime "Release"
    optimize "On"
    buildoptions "/MT"