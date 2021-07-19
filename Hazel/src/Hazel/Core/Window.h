#pragma once

#include "hzpch.h"
#include "Hazel/Core/core.h"
#include "Hazel/Events/Event.h"

namespace Hazel {

	struct WindowProps {
		std::string title;
		unsigned int width;
		unsigned int height;

		WindowProps(const std::string& title = "Hazel Engine",
			const unsigned int& width = 640,
			const unsigned int& height = 360)
			:title(title), width(width), height(height) 
		{
		}

	};


	class HAZEL_API_ Window {
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		~Window() {};

		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		//Window attributes

		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;

		static Window* Create(const WindowProps& props = WindowProps());

	};


}