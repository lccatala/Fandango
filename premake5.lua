workspace "Fandango"
	architecture "x64"
	startproject "Fandango-Editor"

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
IncludeDir["entt"] = "Fandango/vendor/entt/include"
IncludeDir["Glad"]  = "Fandango/vendor/Glad/include"
IncludeDir["GLFW"]  = "Fandango/vendor/GLFW/include"
IncludeDir["glm"] = "Fandango/vendor/glm"
IncludeDir["ImGui"] = "Fandango/vendor/imgui"
IncludeDir["stb_image"] = "Fandango/vendor/stb_image"
IncludeDir["yaml_cpp"] = "Fandango/vendor/yaml-cpp/include"
IncludeDir["ImGuizmo"] = "Fandango/vendor/ImGuizmo"

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
		"%{prj.name}/vendor/glm/glm.**.inl",

		"%{prj.name}/vendor/ImGuizmo/ImGuizmo.h",
		"%{prj.name}/vendor/ImGuizmo/ImGuizmo.cpp"
	}
	
	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.yaml_cpp}",
		"%{IncludeDir.ImGuizmo}"
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib",
		"yaml-cpp"
	}

	-- Avoid having to include the PCH in ImGuizmo files
	filter "files:Fandango/vendor/ImGuizmo/**.cpp"
		flags { "NoPCH" }

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
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}"
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


project "Fandango-Editor"
	location "Fandango-Editor"
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
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.ImGuizmo}",
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

group "Dependencies"
	include "Fandango/vendor/GLFW"
	include "Fandango/vendor/Glad"
	include "Fandango/vendor/imgui"
	include "Fandango/vendor/yaml-cpp"

group ""