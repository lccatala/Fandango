#include <Fandango.h>

#include "Fandango/Platform/OpenGL/OpenGLShader.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui/imgui.h>

class ExampleLayer : public Fandango::Layer
{
public:
	ExampleLayer()
		: Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f)
	{
		m_TriangleVA.reset(Fandango::VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};

		std::shared_ptr<Fandango::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Fandango::VertexBuffer::Create(vertices, sizeof(vertices)));

		Fandango::BufferLayout layout = {
			{Fandango::ShaderDataType::Float3, "a_Position"},
			{Fandango::ShaderDataType::Float4, "a_Color"}
		};

		vertexBuffer->SetLayout(layout);
		m_TriangleVA->AddVertexBuffer(vertexBuffer);

		uint32_t triangleIndices[3] = { 0, 1, 2 };
		std::shared_ptr<Fandango::IndexBuffer> triangleIB;
		triangleIB.reset(Fandango::IndexBuffer::Create(triangleIndices, sizeof(triangleIndices) / sizeof(uint32_t)));
		m_TriangleVA->SetIndexBuffer(triangleIB);

		m_SquareVA.reset(Fandango::VertexArray::Create());

		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		std::shared_ptr<Fandango::VertexBuffer> squareVB;
		squareVB.reset(Fandango::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{Fandango::ShaderDataType::Float3, "a_Position"},
			{Fandango::ShaderDataType::Float2, "a_TexCoord"}
			});
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<Fandango::IndexBuffer> squareIB;
		squareIB.reset(Fandango::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(squareIB);

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjectionMatrix;
			uniform mat4 u_Transform;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjectionMatrix * u_Transform * vec4(a_Position, 1.0);
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}
		)";

		m_TriangleShader.reset(Fandango::Shader::Create(vertexSrc, fragmentSrc));

		std::string squareVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

			out vec3 v_Position;
			uniform mat4 u_ViewProjectionMatrix;
			uniform mat4 u_Transform;

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjectionMatrix * u_Transform * vec4(a_Position, 1);
			}
		)";

		std::string flatColorShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			in vec3 v_Position;
			uniform vec3 u_Color;

			void main()
			{
				color = vec4(u_Color, 1.0);
			}
		)";

		m_FlatColorShader.reset(Fandango::Shader::Create(squareVertexSrc, flatColorShaderFragmentSrc));

		std::string textureShaderVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec2 a_TexCoord;

			uniform mat4 u_ViewProjectionMatrix;
			uniform mat4 u_Transform;

			out vec2 v_TexCoord;

			void main()
			{
				v_TexCoord = a_TexCoord;
				gl_Position = u_ViewProjectionMatrix * u_Transform * vec4(a_Position, 1);
			}
		)";

		std::string textureShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec2 v_TexCoord;
			
			uniform sampler2D u_Texture;
			
			void main()
			{
				color = texture(u_Texture, v_TexCoord);
			}
		)";

		m_TextureShader.reset(Fandango::Shader::Create("assets/shaders/Texture.glsl"));

		m_Texture = Fandango::Texture2D::Create("assets/textures/Checkerboard.png");
		m_LogoTexture= Fandango::Texture2D::Create("assets/textures/ChernoLogo.png");

		std::dynamic_pointer_cast<Fandango::OpenGLShader>(m_TextureShader)->Bind();
		std::dynamic_pointer_cast<Fandango::OpenGLShader>(m_TextureShader)->UploadUniform("u_Texture", 0);
	}

	void OnUpdate(Fandango::TimeStep ts) override
	{
		if (Fandango::Input::IsKeyPressed(FNDG_KEY_A))
			m_CameraPosition.x -= m_CameraSpeed * ts;
		else if (Fandango::Input::IsKeyPressed(FNDG_KEY_D))
			m_CameraPosition.x += m_CameraSpeed * ts;

		if (Fandango::Input::IsKeyPressed(FNDG_KEY_S))
			m_CameraPosition.y -= m_CameraSpeed * ts;
		else if (Fandango::Input::IsKeyPressed(FNDG_KEY_W))
			m_CameraPosition.y += m_CameraSpeed * ts;

		if (Fandango::Input::IsKeyPressed(FNDG_KEY_Q))
			m_CameraRotation -= m_CameraRotationSpeed * ts;
		else if (Fandango::Input::IsKeyPressed(FNDG_KEY_E))
			m_CameraRotation += m_CameraRotationSpeed * ts;

		Fandango::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Fandango::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		Fandango::Renderer::BeginScene(m_Camera);

		static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		std::dynamic_pointer_cast<Fandango::OpenGLShader>(m_FlatColorShader)->Bind();
		std::dynamic_pointer_cast<Fandango::OpenGLShader>(m_FlatColorShader)->UploadUniform("u_Color", m_SquareColor);

		for (int i = 0; i < 20; i++)
		{
			for (int j = 0; j < 20; j++)
			{
				glm::vec3 pos(i * 0.11f, j * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;

				Fandango::Renderer::Submit(m_SquareVA, m_FlatColorShader, transform);
			}
		}

		m_Texture->Bind();
		Fandango::Renderer::Submit(m_SquareVA, m_TextureShader, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
		m_LogoTexture->Bind();
		Fandango::Renderer::Submit(m_SquareVA, m_TextureShader, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		Fandango::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}

	void OnEvent(Fandango::Event& event) override
	{
	}

private:
	Fandango::Ref<Fandango::Shader> m_TriangleShader;
	Fandango::Ref<Fandango::VertexArray> m_TriangleVA;

	Fandango::Ref<Fandango::Shader> m_FlatColorShader, m_TextureShader;
	Fandango::Ref<Fandango::VertexArray> m_SquareVA;

	Fandango::Ref<Fandango::Texture2D> m_Texture, m_LogoTexture;

	Fandango::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraSpeed = 5.0f;
	float m_CameraRotationSpeed = 180.0f;
	float m_CameraRotation = 0.0f;

	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };
};

class Sandbox : public Fandango::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}

	~Sandbox()
	{

	}
};


Fandango::Application* Fandango::CreateApplication()
{
	return new Sandbox();
}

