// LearnGLFW.cpp : Defines the entry point for the application.
//

#include "LearnOpenGL.h"

// Global State
ImGuiContext* g_ImGuiContext = nullptr;
float g_scaleFactor = 1.25f;
bool g_quitProgram = false;

// Shader sources
std::string vertexShaderSource(
	"#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0"
);
std::string fragmentShaderSource(
	"#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
	"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	"}\0"
);

void MakeUI();
void CustomizeImGui(ImGuiIO& io, ImGuiStyle& style);
GLuint PrepareBuffer();
GLuint CreateShaderProgram(std::string& vertexShaderSource, std::string& fragmentShaderSource);

int main() {
	if (glfwInit() == GLFW_FALSE) {
		std::cout << "Failed to Initialized GLFW" << std::endl;
		return -1;
	}
	std::cout << "GLFW: " << glfwGetVersionString() << std::endl;

	// Imgui init
	g_ImGuiContext = ImGui::CreateContext();
	

	// Window hints for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	

	GLFWwindow* window = glfwCreateWindow(1000, 600, "LearnOpenGL", nullptr, nullptr);

	// Tell GLFW to use OpenGL context of specified window to be current context
	glfwMakeContextCurrent(window);

	// Tell GLAD to initalize
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	} 

	GLuint vertexArray = PrepareBuffer();
	GLuint shaderProgram = CreateShaderProgram(vertexShaderSource, fragmentShaderSource);

	// Tell ImGui to use 
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	// ImGui stlying customization
	ImGuiIO& io = ImGui::GetIO();
	ImGuiStyle& style = ImGui::GetStyle();
	ImFont* mainFont = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Arial.ttf", 18.0f);
	IM_ASSERT(mainFont != NULL);
	CustomizeImGui(io, style);
	io.FontGlobalScale = g_scaleFactor;

	glfwSwapInterval(1);
	while (!g_quitProgram && !glfwWindowShouldClose(window)) {
		glfwPollEvents();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::PushFont(mainFont);
		MakeUI();
		ImGui::ShowDemoWindow();
		ImGui::PopFont();

		ImGui::EndFrame();
		ImGui::Render();
		ImDrawData* drawData = ImGui::GetDrawData();

		// Clear frame buffer of previous draws
		glClearColor(0.0f, 0.0f, 0.0f, 0.4f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw Triangle using vertex array we created
		glUseProgram(shaderProgram);
		glBindVertexArray(vertexArray);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		
		ImGui_ImplOpenGL3_RenderDrawData(drawData);
		glfwSwapBuffers(window);
	}
	
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext(g_ImGuiContext);
	glfwTerminate();
	return 0;
}

void MakeUI()
{
	using namespace ImGui;
	if (BeginMainMenuBar()) {
		if (BeginMenu("File")) {
			if (MenuItem("Quit")) {
				g_quitProgram = true;
			}
			EndMenu();
		}

		ImGuiWindowFlags flag = ImGuiWindowFlags_NoCollapse;
		Begin("Settings", NULL, flag);
		End();
		EndMainMenuBar();
	}
}

void CustomizeImGui(ImGuiIO& io, ImGuiStyle& style)
{
	style.WindowRounding = 3.5f;
	style.ScaleAllSizes(g_scaleFactor);
}

GLuint PrepareBuffer()
{
	float trianglePositions[] = {
		// Vertex pos			    // Vertex color
		-0.5f, -0.5f,  0.0f,		1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f,  0.0f,		0.0f, 1.0f, 0.0f,
		 0.0f,  0.5f,  0.0f,		0.0f, 0.0f, 1.0f,
	};
	
	GLuint vertexBuffer;
	GLuint vertexArray;

	glGenBuffers(1, &vertexBuffer); // Create buffer object for our triangle
	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer); // Bind that to OpenGL array buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(trianglePositions), &trianglePositions, GL_STATIC_DRAW); // Copy buffer data to GPU

	GLuint vertexStride = 6 * sizeof(float);

	// Vertex position attribute
	//		attribute index will be 0
	//		number of component of attribute will be 3 (since it is 3D position vector)
	//		value already normalized so we pass GL_FALSE
	//		since a vertex contain 6 unit of floats (3 floats of position and 3 floats of rgb color), the stride will be 6 of 4 bytes float
	//		offset in bytes within a vertex to the attribute will be 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexStride, (const void*)0);

	// Vertex position attribute
	//		attribute index will be 1
	//		number of component of attribute will be 3 (since it is RGB values)
	//		value already normalized so we pass GL_FALSE
	//		since a vertex contain 5 unit of floats (3 floats of position and 3 floats of rgb color), the stride will be 6 of 4 bytes float
	//		offset in bytes within a vertex to the attribute will be 3 of 4 bytes float
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexStride, (const void*) (3 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	
	return vertexArray;
}

GLuint CreateShaderProgram(std::string& vertexShaderSource, std::string& fragmentShaderSource)
{
	// Create Shader Object
	uint32_t vertexShader = glCreateShader(GL_VERTEX_SHADER);
	uint32_t fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	// Bind shader source to its object
	const char* vertexShaderSourceConstStr = vertexShaderSource.c_str();
	glShaderSource(vertexShader, 1, &vertexShaderSourceConstStr, NULL);
	const char* fragmentShaderSourceConstStr = fragmentShaderSource.c_str();
	glShaderSource(fragmentShader, 1, &fragmentShaderSourceConstStr, NULL);
	
	int success;
	char infoLog[512];

	// Compile the shader
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	
	// Create shader program and link with vertex and fragment shader
	uint32_t shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// Delete compiled shader object
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}
