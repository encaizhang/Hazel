#pragma once

#include "Hazel/Window.h"
#include "Hazel/Log.h"
#include "Hazel/Renderer/GraphicsContext.h"
#include <GLFW/glfw3.h>

namespace Hazel {
	
	class WindowsWindow : public Window {
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		inline unsigned int GetWidth() const override { return m_Data.Width; };
		inline unsigned int GetHeight() const override { return m_Data.Height; };

		//Window attributes
		inline void SetEventCallback(const EventCallbackFn& callback) override 
		{
			m_Data.EventCallback = callback;
		}

		void SetVSync(bool enabled);
		bool IsVSync() const;

		inline virtual void* GetNativeWindow() const { return m_window; };
	private:
		virtual void Init(const WindowProps& props);
		virtual void shutdown();
	private:
		GLFWwindow* m_window;
		GraphicsContext* m_context;

		struct WindowData 
		{
			std::string Title;

			unsigned int Width, Height;

			bool VSync;
			EventCallbackFn EventCallback;

		};

		WindowData m_Data;
	};

}