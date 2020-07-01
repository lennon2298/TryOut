workspace "TryOut"

    architecture "x64"
    filter "system:windows"
        startproject "TryOut"

    configurations 
    {
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "TryOut/vendor/glfw/include"
IncludeDir["Glad"] = "TryOut/vendor/glad/include"
IncludeDir["glm"] = "TryOut/vendor/glm"
IncludeDir["stb_image"] = "TryOut/vendor/stb/include"

group "Dependencies"
    include "TryOut/vendor/glfw"
    include "TryOut/vendor/glad"
group ""

project "TryOut"
    location "TryOut"
    kind "ConsoleApp"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl",
    }

    includedirs
    {
        "%{prj.name}/src",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}"
    }

    links
    {
        "GLFW",
        "Glad",
        "opengl32.lib"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines
        {
            "GE_BUILD_DLL",
            "GE_PLATFORM_WINDOWS"
        }

        -- postbuildcommands
        -- {
        --     ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
        -- }

    filter "configurations:Debug"
        defines "GE_DEBUG"
        symbols "On"
      
    filter "configurations:Release"
        defines "GE_RELEASE"
        optimize "On"
      
    filter "configurations:Dist"
        defines "GE_DIST"
        optimize "On"

