#pragma once

extern Fandango::Application* Fandango::CreateApplication();

int main(int argc, char** argv)
{
	Fandango::Log::Init();
	auto app = Fandango::CreateApplication();
	app->Run();
	delete app;
}