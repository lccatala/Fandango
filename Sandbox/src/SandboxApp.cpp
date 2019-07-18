#include <Fandango.h>

class ExampleLayer : public Fandango::Layer
{
public:
	ExampleLayer()
		: Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f)
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
			-0.75f, -0.75f, 0.0f,
			 0.75f, -0.75f, 0.0f,
			 0.75f,  0.75f, 0.0f,
			-0.75f,  0.75f, 0.0f
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

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjectionMatrix * vec4(a_Position, 1.0);
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

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjectionMatrix * vec4(a_Position, 1);
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

	void OnUpdate() override
	{
		Fandango::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Fandango::RenderCommand::Clear();

		m_Camera.SetPosition({ 0.5f, 0.5f, 0.5f });
		m_Camera.SetRotation(45.0f);

		Fandango::Renderer::BeginScene(m_Camera);

		m_SquareShader->Bind();
		Fandango::Renderer::Submit(m_SquareVA, m_SquareShader);

		m_TriangleShader->Bind();
		Fandango::Renderer::Submit(m_TriangleVA, m_TriangleShader);

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

