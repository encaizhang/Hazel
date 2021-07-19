#include "hzpch.h"
#include "Shader.h"
#include "Hazel/Core/Log.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "Renderer.h"
#include <glad/glad.h>

namespace Hazel {
	Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (Renderer::GetAPI())
		{

		case RendererAPI::API::None: HZ_CORE_ASSERT(false, "RendererAPI: Nonde is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL: return std::make_shared<OpenGLShader>(name, vertexSrc, fragmentSrc);

		}

		HZ_CORE_ASSERT(false, "Unknown RendererAPI!");

		return nullptr;
	
	}

	Ref<Shader> Shader::Create(const std::string& filepath)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLShader>(filepath);
		}

		HZ_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	void ShaderLibraray::Add(const std::string& name, const Ref<Shader>& shader)
	{
		HZ_CORE_ASSERT(!Exists(name), "Shader already exists");
		m_Shaders[name] = shader;
	}


	void ShaderLibraray::Add(const Ref<Shader>& shader)
	{
		auto& name = shader->GetName();
		Add(name, shader);
	}


	Ref<Shader> ShaderLibraray::Load(const std::string& filepath)
	{
		auto shader = Shader::Create(filepath);
		Add(shader);
		return shader;
	}

	Ref<Shader> ShaderLibraray::Load(const std::string& name, const std::string& filepath)
	{
		auto shader = Shader::Create(filepath);
		Add(shader);
		return shader;
	}

	Ref<Shader> ShaderLibraray::Get(const std::string& name)
	{

		HZ_CORE_ASSERT(Exists(name), "Shader not found");
		return m_Shaders[name];
		
	}

	bool ShaderLibraray::Exists(const std::string& name) {
	
		return m_Shaders.find(name) != m_Shaders.end();
	}
}
