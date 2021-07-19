#include "Hazel.h"
#include "Sandbox2D.h"
#include <Hazel/Core/EntryPoint.h>


class ExampleLayer : public Hazel::Layer
{
public:
	ExampleLayer()
		: Layer("Example"), m_CameraController(1280.0f / 720.f, false)
	{
		m_VertexArray = Hazel::VertexArray::Create();

		float vertices[3 * 7] = {
			-0.5, -0.5, 0.0, 1.0, 0.0, 0.0, 1.0,
			0.5, -0.5, 0.0, 0.0, 1.0, 0.0, 1.0,
			0.0, 0.5, 0.0, 0.0, 0.0, 1.0, 1.0
		};

		//m_VertexBuffer = std::unique_ptr<VertexBuffer>(VertexBuffer::Create(vertices, sizeof(vertices)));
		m_VertexBuffer = Hazel::VertexBuffer::Create(vertices, sizeof(vertices));

		{
			Hazel::BufferLayout layout = {
				{Hazel::ShaderDataType::Float3, "a_Position"},
				{Hazel::ShaderDataType::Float4, "a_Color"}
			};
			m_VertexBuffer->SetLayout(layout);
			m_VertexArray->AddVertexBuffer(m_VertexBuffer);
		}

		uint32_t indexs[3] = { 0, 1, 2 };
		m_IndexBuffer = Hazel::IndexBuffer::Create(indexs, sizeof(indexs) / sizeof(uint32_t));
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);

		VertexVA = Hazel::VertexArray::Create();
		float squarevertices[5 * 4] = {
			-0.5, -0.5, 0.0, 0.0, 0.0,
			0.5, -0.5, 0.0,	1.0, 0.0,
			0.5, 0.5, 0.0, 1.0, 1.0,
			-0.5, 0.5, 0.0, 0.0, 1.0
		};
		//std::shared_ptr<VertexBuffer> VB;
		VertexVB = Hazel::VertexBuffer::Create(squarevertices, sizeof(squarevertices));
		Hazel::BufferLayout squarelayout = {
				{Hazel::ShaderDataType::Float3, "a_Position"},
				{Hazel::ShaderDataType::Float2, "a_TexCoord"}
		};
		VertexVB->SetLayout(squarelayout);
		VertexVA->AddVertexBuffer(VertexVB);
		uint32_t squareindexs[6] = { 0, 1, 2, 2, 3, 0 };
		VertexIB = Hazel::IndexBuffer::Create(squareindexs, sizeof(squareindexs) / sizeof(uint32_t));
		VertexVA->SetIndexBuffer(VertexIB);

		std::string vertexSrc = R"(
			#version 330 core
			
			layout( location = 0) in vec3 a_Position;
			layout( location = 1) in vec4 a_Color;
			
			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec4 v_Color;
			void main(){
				gl_Position = u_ViewProjection *u_Transform *vec4(a_Position, 1.0);
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

		m_Shader = Hazel::Shader::Create("VertexPosCol", vertexSrc, fragmentSrc);

		std::string bluevertexSrc = R"(
			#version 330 core
			
			layout( location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			void main(){
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}

		)";

		std::string bluefragmentSrc = R"(
			#version 330 core

			layout( location = 0) out vec4 color;
			uniform vec3 u_color;

			void main(){
				color = vec4(u_color, 1.0);
			}

		)";

		m_blueShader = Hazel::Shader::Create("Flat Col", bluevertexSrc, bluefragmentSrc);

		std::string textureShadervertexSrc = R"(
			#version 330 core
			
			layout( location = 0) in vec3 a_Position;
			layout( location = 1) in vec2 a_TexCoord;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;
			
			out vec2 v_TexCoord;
			void main(){
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
				v_TexCoord = a_TexCoord;			
			}

		)";

		std::string textureShaderfragmentSrc = R"(
			#version 330 core

			layout( location = 0) out vec4 color;
			uniform sampler2D u_Texture;
			
			in vec2 v_TexCoord;

			void main(){
				color = texture(u_Texture, v_TexCoord);
			}

		)";

		//m_texShader = std::unique_ptr<Hazel::Shader>(Hazel::Shader::Create(textureShadervertexSrc, textureShaderfragmentSrc));
		auto textureShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");

		m_Texture = Hazel::Texture2D::Create("assets/textures/Checkerboard.png");
		m_ChernoLogoTexture = Hazel::Texture2D::Create("assets/textures/ChernoLogo.png");

		std::dynamic_pointer_cast<Hazel::OpenGLShader>(textureShader)->Bind();
		std::dynamic_pointer_cast<Hazel::OpenGLShader>(textureShader)->UploadUniformInt("u_Texture", 0);
	} 

	void OnUpdate(Hazel::Timestep ts) override
	{
		
		m_CameraController.OnUpdate(ts);

		Hazel::RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
		Hazel::RenderCommand::Clear();

		Hazel::Renderer::BeginScene(m_CameraController.GetCamera());

		static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_blueShader)->Bind();
		std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_blueShader)->UploadUniformFloat3("u_color", m_SquareColor);
		
		for (int y = 0; y < 20; y++) {
			for (int x = 0; x < 20; x++) {
				
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
			
				Hazel::Renderer::Submit(m_blueShader, VertexVA, transform);
			
			}
		
		}
		
		auto textureShader = m_ShaderLibrary.Get("Texture");
		m_Texture->Bind();
		Hazel::Renderer::Submit(textureShader, VertexVA, glm::scale(glm::mat4(1.0), glm::vec3(1.5f)));

		m_ChernoLogoTexture->Bind();
		Hazel::Renderer::Submit(textureShader, VertexVA, glm::scale(glm::mat4(1.0), glm::vec3(1.5f)));
		//Hazel::Renderer::Submit(m_Shader, m_VertexArray);

		Hazel::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override 
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}
	virtual void OnEvent(Hazel::Event& event) override
	{
		m_CameraController.OnEvent(event);
	}

private:
	Hazel::ShaderLibraray m_ShaderLibrary;
	//unsigned int m_VertexArray, m_Index;
	std::shared_ptr<Hazel::Shader> m_Shader;
	std::shared_ptr<Hazel::VertexArray> m_VertexArray;
	std::shared_ptr<Hazel::VertexBuffer> m_VertexBuffer;
	std::shared_ptr<Hazel::IndexBuffer> m_IndexBuffer;

	std::shared_ptr<Hazel::Shader> m_blueShader;
	std::shared_ptr<Hazel::VertexArray> VertexVA;
	std::shared_ptr<Hazel::VertexBuffer> VertexVB;
	std::shared_ptr<Hazel::IndexBuffer> VertexIB;

	Hazel::Ref<Hazel::Texture2D> m_Texture;
	Hazel::Ref<Hazel::Texture2D> m_ChernoLogoTexture;

	Hazel::OrthographicCameraController m_CameraController;
	
	glm::vec3 m_SquareColor;
};

class Sandbox :public Hazel::Application {

public:
	Sandbox() {

		//PushLayer(new ExampleLayer());
		PushLayer(new Sandbox2D());
		PushOverlay(new Hazel::ImGuiLayer());
	}

	~Sandbox() {
	
	
	}

};

Hazel::Application* Hazel::CreateApplication() {
	return new Sandbox;

}