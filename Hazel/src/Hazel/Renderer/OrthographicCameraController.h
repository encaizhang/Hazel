#pragma once

#include "Hazel/Renderer/OrthographicCamera.h"
#include "Hazel/Core/Timestep.h"

#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Events/MouseEvent.h"

namespace Hazel {

	class OrthographicCameraController {

	public:
		OrthographicCameraController(float aspectRatio, bool rotation = false);

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		OrthographicCamera& GetCamera() { return m_Camera; }
		const OrthographicCamera& GetCamera() const{ return m_Camera; }
	private:
		bool onMouseScrolled(MouseScrolledEvent& e);
		bool onWindowResized(WindowResizeEvent& e);


	private:
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;

		bool m_Rotation;
		OrthographicCamera m_Camera;

		float m_CameraRotation = 0.0f;
		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };

		float m_CameraTranslationSpeed = 1.0f;
		float m_CameraRotationSpeed = 180.0f;
	};


}