#include "Hazel.h"

class ExampleLayer : public Hazel::Layer
{
public:
	ExampleLayer()
		: Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f)
	{
		m_VertexArray.reset(Hazel::VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5, -0.5, 0.0, 1.0, 0.0, 0.0, 1.0,
			0.5, -0.5, 0.0, 0.0, 1.0, 0.0, 1.0,
			0.0, 0.5, 0.0, 0.0, 0.0, 1.0, 1.0
		};

		//m_VertexBuffer = std::unique_ptr<VertexBuffer>(VertexBuffer::Create(vertices, sizeof(vertices)));
		m_VertexBuffer.reset(Hazel::VertexBuffer::Create(vertices, sizeof(vertices)));

		{
			Hazel::BufferLayout layout = {
				{Hazel::ShaderDataType::Float3, "a_Position"},
				{Hazel::ShaderDataType::Float4, "a_Color"}
			};
			m_VertexBuffer->SetLayout(layout);
			m_VertexArray->AddVertexBuffer(m_VertexBuffer);
		}

		uint32_t indexs[3] = { 0, 1, 2 };
		m_IndexBuffer.reset(Hazel::IndexBuffer::Create(indexs, sizeof(indexs) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);

		VertexVA.reset(Hazel::VertexArray::Create());
		float squarevertices[3 * 4] = {
			-0.5, -0.5, 0.0,
			0.5, -0.5, 0.0,
			0.5, 0.5, 0.0,
			-0.5, 0.5, 0.0
		};
		//std::shared_ptr<VertexBuffer> VB;
		VertexVB.reset(Hazel::VertexBuffer::Create(squarevertices, sizeof(squarevertices)));
		Hazel::BufferLayout squarelayout = {
				{Hazel::ShaderDataType::Float3, "a_Position"}
		};
		VertexVB->SetLayout(squarelayout);
		VertexVA->AddVertexBuffer(VertexVB);
		uint32_t squareindexs[6] = { 0, 1, 2, 2, 3, 0 };
		VertexIB.reset(Hazel::IndexBuffer::Create(squareindexs, sizeof(squareindexs) / sizeof(uint32_t)));
		VertexVA->SetIndexBuffer(VertexIB);

		std::string vertexSrc = R"(
			#version 330 core
			
			layout( location = 0) in vec3 a_Position;
			layout( location = 1) in vec4 a_Color;
			
			uniform mat4 u_ViewProjection;

			out vec4 v_Color;
			void main(){
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
				v_Color = a_Color;
			}

		)";

		std::string fragmentSrc = R"(
			#version 330 core

			layout( location = 0) out vec4 color;
			in vec4 v_Color;

			void main(){
				color = v_Color;
			}

		)";

		m_Shader = std::unique_ptr<Hazel::Shader>(new Hazel::Shader(vertexSrc, fragmentSrc));

		std::string bluevertexSrc = R"(
			#version 330 core
			
			layout( location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;

			void main(){
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
			}

		)";

		std::string bluefragmentSrc = R"(
			#version 330 core

			layout( location = 0) out vec4 color;

			void main(){
				color = vec4(0.0, 1.0, 0.0, 1.0);
			}

		)";

		m_blueShader = std::unique_ptr<Hazel::Shader>(new Hazel::Shader(bluevertexSrc, bluefragmentSrc));

	}

	void OnUpdate(Hazel::Timestep ts) override
	{
		HZ_TRACE("Delta time:{0}s ({1}ms)", ts.GetSeconds(), ts.GetMilliseconds());

		if (Hazel::Input::IsKeyPressed(HZ_KEY_LEFT)) {
			m_CameraPosition.x += m_CameraMoveSpeed * ts;
		}else if (Hazel::Input::IsKeyPressed(HZ_KEY_RIGHT)) {
			m_CameraPosition.x -= m_CameraMoveSpeed * ts;
		}

		if (Hazel::Input::IsKeyPressed(HZ_KEY_DOWN)) {
			m_CameraPosition.y += m_CameraMoveSpeed * ts;
		}else if (Hazel::Input::IsKeyPressed(HZ_KEY_UP)) {
			m_CameraPosition.y -= m_CameraMoveSpeed * ts;
		}

		if (Hazel::Input::IsKeyPressed(HZ_KEY_A)) {
			m_CameraRotation -= m_CameraRotationSpeed * ts;
		}else if (Hazel::Input::IsKeyPressed(HZ_KEY_D)) {
			m_CameraRotation += m_CameraRotationSpeed * ts;
		}


		Hazel::RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
		Hazel::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);
		Hazel::Renderer::BeginScene(m_Camera);

		Hazel::Renderer::Submit(m_blueShader, VertexVA);
		Hazel::Renderer::Submit(m_Shader, m_VertexArray);

		Hazel::Renderer::EndScene();
	}


private:
	//unsigned int m_VertexArray, m_Index;
	std::shared_ptr<Hazel::Shader> m_Shader;
	std::shared_ptr<Hazel::VertexArray> m_VertexArray;
	std::shared_ptr<Hazel::VertexBuffer> m_VertexBuffer;
	std::shared_ptr<Hazel::IndexBuffer> m_IndexBuffer;

	std::shared_ptr<Hazel::Shader> m_blueShader;
	std::shared_ptr<Hazel::VertexArray> VertexVA;
	std::shared_ptr<Hazel::VertexBuffer> VertexVB;
	std::shared_ptr<Hazel::IndexBuffer> VertexIB;

	Hazel::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition = {0.0f, 0.0f, 0.0f};
	float m_CameraMoveSpeed = 1.0f;

	float m_CameraRotation = 0.0f;
	float m_CameraRotationSpeed = 30.0f;
};

class Sandbox :public Hazel::Application {

public:
	Sandbox() {

		PushLayer(new ExampleLayer());
		PushOverlay(new Hazel::ImGuiLayer());
	}

	~Sandbox() {
	
	
	}

};

Hazel::Application* Hazel::CreateApplication() {
	return new Sandbox;

}