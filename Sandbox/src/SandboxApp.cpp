#include <Fandango.h>

#include "imgui/imgui.h"

class ExampleLayer : public Fandango::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{
	}

	void OnUpdate() override
	{

	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Test");
		ImGui::Text("Hello World");
		ImGui::End();
	}

	void OnEvent(Fandango::Event& event) override
	{

	}

};

class Sandbox : public Fandango::Application
{
public:
	Sandbox()
	{
	}

	~Sandbox()
	{

	}
};


Fandango::Application* Fandango::CreateApplication()
{
	return new Sandbox();
}

