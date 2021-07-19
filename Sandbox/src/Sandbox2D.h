#pragma once

#include "Hazel.h"

class Sandbox2D: public Hazel::Layer {
public:
	Sandbox2D();
	virtual ~Sandbox2D() {};
	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnEvent(Hazel::Event& event) override;
	virtual void OnImGuiRender() override;
	void OnUpdate(Hazel::Timestep ts) override;

private:
	Hazel::OrthographicCameraController m_CameraController;
	std::shared_ptr<Hazel::Shader> m_FlatColorShader;

	Hazel::Ref<Hazel::Texture2D> m_CheckboardTexture;

	struct ProfileResult
	{
		const char* Name;
		float Time;
	};

	std::vector<ProfileResult> m_ProfileResults;

	std::shared_ptr<Hazel::VertexArray> m_SquareVA;
	glm::vec4 m_SquareColor = {0.0, 1.0, 0.0, 1.0};
};