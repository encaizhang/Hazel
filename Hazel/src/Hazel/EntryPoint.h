#pragma once
#include "Application.h"

#ifdef HZ_PLATFORM_WINDOWS

extern Hazel::Application* Hazel::CreateApplication();

void main() {
	printf("hello, welcome");
	auto app = Hazel::CreateApplication();
	app->run();
	delete app;

}

#endif