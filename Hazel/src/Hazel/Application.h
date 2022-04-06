#pragma once
#include "core.h"
#include <stdio.h>

namespace Hazel {

	class HAZEL_API_ Application
	{
	public:
		Application() {

		}

		virtual ~Application(){
		
		}

		void run();
	};

	// To be defined in client
	Application* CreateApplication();
}


