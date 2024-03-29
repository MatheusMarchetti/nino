project "DirectXTex"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    systemversion "latest"
    staticruntime "On"
    
    targetdir ("bin/" ..outputdir.. "/%{prj.name}")
    objdir ("bin-int/" ..outputdir.. "/%{prj.name}")

    files
    {
        "DirectXTex.h",
        "DirectXTexP.h",
        "DDS.h",
        "BC.h",
        "DirectXTexDDS.cpp",
        "DirectXTexHDR.cpp",
        "DirectXTexImage.cpp",
        "DirectXTexTGA.cpp",
        "DirectXTexWIC.cpp",
        "DirectXTexUtil.cpp",
        "DirectXTexMipmaps.cpp",
        "DirectXTexConvert.cpp",
        "DirectXTexCompress.cpp",
        "DirectXTexD3D11.cpp",
        "BC.cpp",
        "BC4BC5.cpp",
        "BC6HBC7.cpp"
    }

    filter "configurations:Debug"
    runtime "Debug"
    symbols "On"
    buildoptions "/MTd"

filter "configurations:Profiling"
    runtime "Release"
    optimize "On"
    buildoptions "/MT"

filter "configurations:Release"
    runtime "Release"
    optimize "On"
    buildoptions "/MT"