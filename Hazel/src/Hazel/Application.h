#pragma once
#include "core.h"
#include "Log.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"
#include "Window.h"
#include "Hazel/LayerStack.h"

namespace Hazel {

	class HAZEL_API_ Application
	{
	public:
		Application();

		virtual ~Application();

		void run();

		void OnEvent(Event& e);
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }

	private:
		bool OnWindowClose(WindowCloseEvent& e);
		std::unique_ptr<Window> m_Window;

		bool m_Running = true;
		LayerStack m_layerStack;

	private:
		static Application* s_Instance;
	};

	// To be defined in client
	Application* CreateApplication();
}


