#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "berror.h"
#include "bint.h"

float vertices[] = {
	 0.0,  0.5, 0.0,
	-0.5, -0.5, 0.0,
	 0.5, -0.5, 0.0
};

const char* vertex_shader_source = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char* fragment_shader_source = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

namespace bgl
{
	typedef GLuint bshader;
	typedef GLuint bprogram;

	namespace bshader_type
	{
		enum : u8
		{
			vertex_bit          = 1 << 0,
			fragment_bit        = 1 << 1,
			geometry_bit        = 1 << 2,
			tess_control_bit    = 1 << 3,
			tess_evaluation_bit = 1 << 4,
			compute_bit         = 1 << 5,

			vertex              = 0,
			fragment            = 1,
			geometry            = 2,
			tess_control        = 3,
			tess_evaluation     = 4,
			compute             = 5,

			count               = 6,
		};
	}

	struct bshader_glsl_info
	{
		bshader id;
		const char* file_path;
		const char* glsl_buffer;
	};

	struct bshader_spirv_info
	{
		bshader id;
		const char* file_path;
		char* spirv_buffer;
	};

	struct bshader_glsl_shaders_info
	{
		bshader_glsl_info vertex;
		bshader_glsl_info fragment;
		bshader_glsl_info geometry;
		bshader_glsl_info tess_control;
		bshader_glsl_info tess_evaluation;
		bshader_glsl_info compute;
	};

	int bshader_compile_glsl_shaders(bshader_glsl_shaders_info* compile_info)
	{
		int success = 0;

		auto shader_array = (bshader_glsl_info*)(&compile_info);
		for (u16 s = 0; s < bshader_type::count; s++)
		{
			auto& shader_info = shader_array[s];

			if (shader_info.id != 0)
				continue;

			/*
			* opengl has something called glisshader to check if a shader id
			* has been previously created with glcreateshader and not deleted
			* it also has stuff to get and set the source code of a shader
			* the only thing it doesn't have is a way to associate a file path
			* with a shader, but I can do that myself tomorrow
			*/
			if (shader_info.glsl_buffer == nullptr)
			{
				DEBUG_ONLY({
					if (shader_info.file_path == NULL) { BERRNO = BEMEMNULL | BERROR_SEVERITY_WARNING; success = -1; continue; }
					});

				GLuint opengl_shader_type;
				switch (s)
				{
				case bshader_type::vertex: opengl_shader_type = GL_VERTEX_SHADER;  break;
				case bshader_type::fragment: opengl_shader_type = GL_FRAGMENT_SHADER; break;
				case bshader_type::geometry: opengl_shader_type = GL_GEOMETRY_SHADER; break;
				case bshader_type::tess_control: opengl_shader_type = GL_TESS_CONTROL_SHADER; break;
				case bshader_type::tess_evaluation: opengl_shader_type = GL_TESS_EVALUATION_SHADER; break;
				case bshader_type::compute: opengl_shader_type = GL_COMPUTE_SHADER; break;
				}

				shader_info.id = glCreateShader(opengl_shader_type);
				if (shader_info.id == 0)
				{
					BERRNO = BEPARTFAIL | BERROR_SEVERITY_HIGH; return -1;
				}
			}
			else
			{
			}
		}

		return success;
	}
}

int main()
{
#pragma region window
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 5);

	GLFWwindow* window = glfwCreateWindow(1280, 720, "circles", NULL, NULL);
	glfwMakeContextCurrent(window);

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
#pragma endregion

#pragma region shader
	GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
	glCompileShader(vertex_shader);

	GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
	glCompileShader(fragment_shader);

	GLuint shader_program = glCreateProgram();
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);
	glLinkProgram(shader_program);

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
#pragma endregion

#pragma region object
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
#pragma endregion

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	while (!glfwWindowShouldClose(window)) {

		glClear(GL_COLOR_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		glUseProgram(shader_program);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		int flags =
			ImGuiWindowFlags_NoTitleBar
			| ImGuiWindowFlags_NoResize
			| ImGuiWindowFlags_NoMove
			| ImGuiWindowFlags_NoBackground;
		ImGui::SetNextWindowPos({ 0, 0 });
		ImGui::SetNextWindowSize({ 1280, 720 });
		ImGui::Begin("##window", nullptr, flags);
		ImGui::Text("Hello");
		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glDeleteBuffers(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteProgram(shader_program);
	glfwDestroyWindow(window);
	return 0;
}
