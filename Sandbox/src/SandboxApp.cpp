#include <Fandango.h>

#include <glm/gtc/matrix_transform.hpp>

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

		float squareVertices[3 * 4] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f
		};

		std::shared_ptr<Fandango::VertexBuffer> squareVB;
		squareVB.reset(Fandango::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{Fandango::ShaderDataType::Float3, "a_Position"}
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

		m_TriangleShader.reset(new Fandango::Shader(vertexSrc, fragmentSrc));

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

		std::string squareFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			in vec3 v_Position;

			void main()
			{
				color = vec4(0.2, 0.3, 0.8, 1.0);
			}
		)";

		m_SquareShader.reset(new Fandango::Shader(squareVertexSrc, squareFragmentSrc));
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

		Fandango::Renderer::Submit(m_TriangleVA, m_TriangleShader);
		for (int i = 0; i < 20; i++)
			for (int j = 0; j < 20; j++)
			{
				glm::vec3 pos(i * 0.11f, j * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				Fandango::Renderer::Submit(m_SquareVA, m_SquareShader, transform);
			}

		Fandango::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override
	{
		
	}

	void OnEvent(Fandango::Event& event) override
	{
	}

private:
	std::shared_ptr<Fandango::Shader> m_TriangleShader;
	std::shared_ptr<Fandango::VertexArray> m_TriangleVA;

	std::shared_ptr<Fandango::Shader> m_SquareShader;
	std::shared_ptr<Fandango::VertexArray> m_SquareVA;

	Fandango::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraSpeed = 5.0f;
	float m_CameraRotationSpeed = 180.0f;
	float m_CameraRotation = 0.0f;
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

