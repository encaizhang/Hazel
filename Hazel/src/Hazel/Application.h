#pragma once
#include "core.h"
#include "Log.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"
#include "Window.h"
#include "Hazel/LayerStack.h"

#include "Hazel/ImGui/ImGuiLayer.h"
#include "Hazel/Core/Timestep.h"

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

	private:
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;

		bool m_Running = true;
		LayerStack m_layerStack;
		unsigned int length;
		float m_LastFrametime;
		
	private:
		static Application* s_Instance;
	};

	// To be defined in client
	Application* CreateApplication();
}


