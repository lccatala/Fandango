workspace "Fandango"
	architecture "x64"
	startproject "Sandbox"

configurations
{
	"Debug",
	"Release",
	"Dist"
}

flags
{
	"MultiProcessorCompile"
}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- External libraries
IncludeDir = {}
IncludeDir["GLFW"]  = "Fandango/vendor/GLFW/include"
IncludeDir["Glad"]  = "Fandango/vendor/Glad/include"
IncludeDir["ImGui"] = "Fandango/vendor/imgui"
IncludeDir["glm"] = "Fandango/vendor/glm"
IncludeDir["stb_image"] = "Fandango/vendor/stb_image"

group "Dependencies"
	include "Fandango/vendor/GLFW"
	include "Fandango/vendor/Glad"
	include "Fandango/vendor/imgui"

group ""

project "Fandango"
  location "Fandango"
  kind "StaticLib"
	language "C++"
	cppdialect "C++17"
  staticruntime "on"

  targetdir ("bin/" .. outputdir .. "/%{prj.name}")
  objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

  pchheader "fndgpch.h"
  pchsource "Fandango/src/fndgpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/glm/glm.**.hpp",
		"%{prj.name}/vendor/glm/glm.**.inl"
	}
	
	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

  includedirs
  {
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}"
  }

  links
  {
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
  }

  filter "system:windows"
		systemversion "latest"

	defines
	{
	  "FNDG_BUILD_DLL",
	  "GLFW_INCLUDE_NONE"
	}

  filter "system:macos"
		systemversion "latest"

	defines
	{
	  "FNDG_PLATFORM_MAC",
	  "FNDG_BUILD_DLL",
	  "GLFW_INCLUDE_NONE"
	}

	filter "configurations:Debug"
		defines "FNDG_DEBUG"
		runtime "Debug"
		symbols "on"
		
	filter "configurations:Release"
		defines "FNDG_RELEASE"
		runtime "Release"
		optimize "on"
	
	filter "configurations:Dist"
		defines "FNDG_DIST"
		runtime "Release"
		optimize "on"

project "Sandbox"
  location "Sandbox"
  kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
  staticruntime "on"

  targetdir ("bin/" .. outputdir .. "/%{prj.name}")
  objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

  files
  {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
  }

  includedirs
  {
		"Fandango/vendor/spdlog/include",
		"Fandango/src",
		"Fandango/vendor",
		"%{IncludeDir.glm}"
  }

  links 
  { 
		"Fandango"
  }

  filter "system:windows"
		systemversion "latest"

		defines
		{
			"FNDG_PLATFORM_WINDOWS"
		}

  filter "system:macos"
		systemversion "latest"

		defines
		{
			"FNDG_PLATFORM_MAC"
		}

  filter "configurations:Debug"
		defines "FNDG_DEBUG"
		runtime "Debug"
		symbols "on"

  filter "configurations:Release"
		defines "FNDG_RELEASE"
		runtime "Release"
		optimize "on"
		
		filter "configurations:Dist"
		defines "FNDG_DIST"
		runtime "Release"
		optimize "on"