#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "Core.h"
#include "Renderer.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "VertexArray.h"
#include "ShaderProgram.h"
#include "Texture2D.h"


int main() {

	if (!glfwInit()) {
		std::cout << "GLFW could not be initialized... Aborting!!";
		exit(-1);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3.0);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3.0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	GLFWwindow* window = glfwCreateWindow(1000, 600, "LearnOpenGL", nullptr, nullptr);
	if (!window) {
		std::cout << "Failed to create a window... Aborting!!";
		glfwTerminate();
		exit(-1);
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD... Aborting!!";
		glfwTerminate();
		exit(-1);
	}

	glViewport(0, 0, 1000, 600);

	{
		//create shader programs
		ShaderProgram cubeProgram;
		cubeProgram.AddShader("./source/shaders/cube-shader.vert", GL_VERTEX_SHADER);
		cubeProgram.AddShader("./source/shaders/cube-shader.frag", GL_FRAGMENT_SHADER);
		cubeProgram.LinkProgram();
		unsigned int programId = cubeProgram.GetProgramId();

		ShaderProgram lightProgram;
		lightProgram.AddShader("./source/shaders/light-shader.vert", GL_VERTEX_SHADER);
		lightProgram.AddShader("./source/shaders/light-shader.frag", GL_FRAGMENT_SHADER);
		lightProgram.LinkProgram();
		unsigned int lightProgramId = lightProgram.GetProgramId();

		//////////////////////////////////// object settings //////////////////////////////////////

		glUseProgram(programId);

		VertexArray cubeVertexArray;
		VertexBuffer cubeVertexBuffer(CUBE_VERTEX_DATA, sizeof(CUBE_VERTEX_DATA));
		VertexBufferLayout cubeVertexLayout(8 * sizeof(float));
		cubeVertexLayout.Push<float>(0, 3, GL_FLOAT, 0);					// position vertex attribute
		cubeVertexLayout.Push<float>(1, 3, GL_FLOAT, sizeof(float) * 5);    // normal vertex attribute
		cubeVertexLayout.Push<float>(2, 2, GL_FLOAT, sizeof(float) * 3);    // texture coordinates
		cubeVertexArray.AddAttributes(cubeVertexBuffer, cubeVertexLayout);

		Texture2D diffuseMap(GL_TEXTURE0, "./assets/container.png");
		diffuseMap.GeneratePngTexture();

		Texture2D specularMap(GL_TEXTURE1, "./assets/container-specular.png");
		specularMap.GeneratePngTexture();

		Texture2D emissionMap(GL_TEXTURE2, "./assets/matrix.jpg");
		emissionMap.GenerateJpgTexture();

		cubeVertexBuffer.Unbind();
		cubeVertexArray.Unbind();
		glUseProgram(0);

		////////////////////////////////////////////////////////////////////////////////////////////

		/////////////////////////////////// light settings /////////////////////////////////////////

		glUseProgram(lightProgramId);
		VertexArray lightVertexArray;
		VertexBufferLayout lightVertexLayout(8 * sizeof(float));
		lightVertexLayout.Push<float>(0, 3, GL_FLOAT, 0);	// position vertex attribute
		cubeVertexArray.AddAttributes(cubeVertexBuffer, lightVertexLayout);
		lightVertexArray.Unbind();
		glUseProgram(0);

		/////////////////////////////////////////////////////////////////////////////////////////////

		glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1000.0f / 600.0f, 0.1f, 100.0f);

		glm::mat4 view = glm::mat4(1.0f);
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

		glm::mat4 model;
		glm::mat3 normalMatrix;

		glEnable(GL_DEPTH_TEST);

		while (!glfwWindowShouldClose(window)) {
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			//////////////////////////////////////////////////////////////////////// OBJECT /////////////////////////////////////////////////////////////////////
			glUseProgram(programId);
			cubeVertexArray.Bind();

			glUniformMatrix4fv(glGetUniformLocation(programId, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
			glUniformMatrix4fv(glGetUniformLocation(programId, "view"), 1, GL_FALSE, glm::value_ptr(view));

			model = glm::mat4(1.0f);
			model = glm::rotate(model, (float)glfwGetTime() * glm::radians(35.0f), glm::vec3(0.5f, 1.0f, 0.0f));
			glUniformMatrix4fv(glGetUniformLocation(programId, "model"), 1, GL_FALSE, glm::value_ptr(model));

			normalMatrix = glm::mat3(glm::transpose(glm::inverse(model)));
			glUniformMatrix3fv(glGetUniformLocation(programId, "normalMatrix"), 1, GL_FALSE, glm::value_ptr(normalMatrix));

			glUniform3fv(glGetUniformLocation(programId, "objectColor"), 1, OBJECT_COLOR);
			glUniform3fv(glGetUniformLocation(programId, "lightColor"), 1, LIGHT_COLOR);
			glUniform3fv(glGetUniformLocation(programId, "lightPosition"), 1, LIGHT_POSITION);
			glUniform3fv(glGetUniformLocation(programId, "eyePosition"), 1, EYE_POSITION);

			diffuseMap.Bind();
			glUniform1i(glGetUniformLocation(programId, "material.diffuse"), 0);

			specularMap.Bind();
			glUniform1i(glGetUniformLocation(programId, "material.specular"), 1);

			emissionMap.Bind();
			glUniform1i(glGetUniformLocation(programId, "material.emission"), 2);

			glUniform1f(glGetUniformLocation(programId, "material.shininess"), 32.0f);

			glUniform1ui(glGetUniformLocation(programId, "debug"), GL_FALSE);

			glDrawArrays(GL_TRIANGLES, 0, 36);

			glUseProgram(0);
			cubeVertexArray.Unbind();

			//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

			//////////////////////////////////////////////////////////////// LIGHT ///////////////////////////////////////////////////////////////////////////////

			glUseProgram(lightProgramId);
			lightVertexArray.Bind();

			glUniformMatrix4fv(glGetUniformLocation(lightProgramId, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
			glUniformMatrix4fv(glGetUniformLocation(lightProgramId, "view"), 1, GL_FALSE, glm::value_ptr(view));

			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(LIGHT_POSITION[0], LIGHT_POSITION[1], LIGHT_POSITION[2]));
			model = glm::rotate(model, (float)glfwGetTime() * glm::radians(35.0f), glm::vec3(0.5f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(0.1f));
			glUniformMatrix4fv(glGetUniformLocation(lightProgramId, "model"), 1, GL_FALSE, glm::value_ptr(model));

			glUniform1ui(glGetUniformLocation(lightProgramId, "debug"), GL_FALSE);

			glDrawArrays(GL_TRIANGLES, 0, 36);

			lightVertexArray.Unbind();
			glUseProgram(0);

			//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

			glfwPollEvents();
			glfwSwapBuffers(window);
		}
	}

	glfwTerminate();

	return 0;

}