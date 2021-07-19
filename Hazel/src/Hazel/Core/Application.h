#pragma once
#include "core.h"
#include "Log.h"
#include "Hazel/Events/Event.h"
#include "Hazel/Events/ApplicationEvent.h"
#include "Window.h"
#include "Hazel/Core/LayerStack.h"

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
		bool OnWindowResize(WindowResizeEvent& e);

	private:
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;

		bool m_Running = true;
		bool m_Minized = false;
		LayerStack m_layerStack;
		unsigned int length;
		float m_LastFrametime;
		
	private:
		static Application* s_Instance;
	};

	// To be defined in client
	Application* CreateApplication();
}


