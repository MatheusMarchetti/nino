project "DirectXTex"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    systemversion "latest"
    staticruntime "On"
    buildoptions "/MTd"
    
    targetdir ("bin/" ..outputdir.. "/%{prj.name}")
    objdir ("bin-int/" ..outputdir.. "/%{prj.name}")

    files
    {
        "DirectXTex.h",
        "DirectXTexP.h",
        "DDS.h",
        "DirectXTexDDS.cpp",
        "DirectXTexHDR.cpp",
        "DirectXTexImage.cpp",
        "DirectXTexTGA.cpp",
        "DirectXTexWIC.cpp"
    }