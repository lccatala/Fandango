#pragma once

extern Fandango::Application* Fandango::CreateApplication();

int main(int argc, char** argv)
{
	Fandango::Log::Init();

	FNDG_PROFILE_BEGIN_SESSION("Startup", "FandangoProfile-Startup.json");
	auto app = Fandango::CreateApplication();
	FNDG_PROFILE_END_SESSION();

	FNDG_PROFILE_BEGIN_SESSION("Runtine", "FandangoProfile-Runtime.json");
	app->Run();
	FNDG_PROFILE_END_SESSION();

	FNDG_PROFILE_BEGIN_SESSION("Shutdown", "FandangoProfile-Shutdown.json");
	delete app;
	FNDG_PROFILE_END_SESSION();
}