#pragma once
#include "Application.h"

#ifdef HZ_PLATFORM_WINDOWS

extern Hazel::Application* Hazel::CreateApplication();

int main(int argc, char** argv) {
	Hazel::Log::Init();
	HZ_CORE_ERROR("Hello!");

	auto app = Hazel::CreateApplication();
	app->run();
	delete app;

}

#endif