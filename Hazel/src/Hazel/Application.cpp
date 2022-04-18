#include "hzpch.h"
#include "Application.h"
#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Log.h"
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include "Input.h"

namespace Hazel {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)
	Application* Application::s_Instance = nullptr;

	Application::Application() {
		HZ_CORE_ASSERT(!s_Instance, "s_Instance is not nullptr");
		s_Instance = this;
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
	}

	Application::~Application() {}


	void Application::PushLayer(Layer* layer) {
	
		m_layerStack.PushLayer(layer);
	}
	
	void Application::PushOverlay(Layer* layer) {
	
		m_layerStack.PushOverlay(layer);
	}

	void Application::OnEvent(Event& e) 
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

		HZ_CORE_INFO("{0}", e);

		for (auto it = m_layerStack.end(); it != m_layerStack.begin();)
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Application::run() {
		
		
		while (m_Running)
		{
			glClearColor(0, 1, 0, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			for (Layer* layer : m_layerStack)
				layer->OnUpdate();
			
			//auto [x, y] = Input::GetMousePosition();
			//HZ_CORE_TRACE("{0}, {1}", x, y);

			m_Window->OnUpdate();
		}
	
	}

	bool Application::OnWindowClose(WindowCloseEvent& e) {
		m_Running = false;
		return true;
	}

}