#include "fndgpch.h"
#include "DebugUILayer.h"

#include "imgui.h"
#include "examples/imgui_impl_glfw.h"
#include "examples/imgui_impl_opengl3.h"

#include "Fandango/Core/Application.h"

// Temporary
#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Fandango
{
	DebugUILayer::DebugUILayer()
		: Layer("DebugUILayer")
	{

	}

	DebugUILayer::~DebugUILayer()
	{

	}

	// Set up ImgGui
	void DebugUILayer::OnAttach()
	{
		FNDG_PROFILE_FUNCTION();

		// Set up Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO &io = ImGui::GetIO(); (void)io;

		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable keyboard controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable gamepad controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;   // Enable multi-viewport (platform windows)

		ImGui::StyleColorsDark();

		// When viewports are enabled we tweak the window rounding and background
		// so platform windows can look identical to regular ones
		ImGuiStyle &style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		Application &app = Application::Get();

		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

		// Set up platform/renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 410");
	}

	void DebugUILayer::OnDetach()
	{
		FNDG_PROFILE_FUNCTION();

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void DebugUILayer::Begin()
	{
		FNDG_PROFILE_FUNCTION();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void DebugUILayer::End()
	{
		FNDG_PROFILE_FUNCTION();

		ImGuiIO &io = ImGui::GetIO();
		Application &app = Application::Get();
		io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}

	void DebugUILayer::OnImGuiRender()
	{
		
	}
}