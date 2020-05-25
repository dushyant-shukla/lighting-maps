#include <glad/glad.h>
#include <GLFW/glfw3.h>

//#include <glm/glm.hpp>
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


void processInput(GLFWwindow* window, const float& frametime)
{
	float adjustedSpeed = cameraSpeed * frametime;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		cameraPos += adjustedSpeed * cameraFront;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		cameraPos -= adjustedSpeed * cameraFront;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * adjustedSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * adjustedSpeed;
	}

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
	{
		ResetCamera();
	}

	
}

void KeyCallback(GLFWwindow* window, int key, int scanCode, int action, int mod)
{
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
	{
		//normalMapping = !normalMapping;
		normal = !normal;
	}

	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
	{
		//parallexMapping = !parallexMapping;
		normal = true;
		parallex = !parallex;
	}

	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
	{
		texture = !texture;
	}
}

void MouseCallback(GLFWwindow* window, double xPosition, double yPosition)
{
	if (firstMouse)
	{
		lastX = xPosition;
		lastY = yPosition;
		firstMouse = false;
	}

	float xOffset = xPosition - lastX;
	float yOffset = lastY - yPosition;
	lastX = xPosition;
	lastY = yPosition;

	const float senstivity = 0.05f;
	xOffset *= senstivity;
	yOffset *= senstivity;

	yaw += xOffset;
	pitch += yOffset;

	pitch = pitch > PITCH_MAX ? PITCH_MAX : pitch;
	pitch = pitch < PITCH_MIN ? PITCH_MIN : pitch;

	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(direction);
}

void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
	if (fieldOfView >= FIELD_OF_VIEW_MIN && fieldOfView <= FIELD_OF_VIEW_MAX)
	{
		fieldOfView -= yOffset;
	}
	else if (fieldOfView <= FIELD_OF_VIEW_MIN)
	{
		fieldOfView = 1.0f;
	}
	else if (fieldOfView >= FIELD_OF_VIEW_MAX)
	{
		fieldOfView = 45.0f;
	}
}

void CreateGbuffer(GLuint& gBuffer, GLuint& gDepthBuffer, GLuint& gPosition, GLuint& gNormal, GLuint& gColor)
{
	glGenFramebuffers(1, &gBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

	// position color buffer
	glGenTextures(1, &gPosition);
	glBindTexture(GL_TEXTURE_2D, gPosition);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);

	// normal color buffer
	glGenTextures(1, &gNormal);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);

	// color buffer
	glGenTextures(1, &gColor);
	glBindTexture(GL_TEXTURE_2D, gColor);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gColor, 0);

	unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, attachments);

	glGenRenderbuffers(1, &gDepthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, gDepthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SCR_WIDTH, SCR_HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, gDepthBuffer);
	
	// finally check if framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

int main() {

	if (!glfwInit()) {
		std::cout << "GLFW could not be initialized... Aborting!!";
		exit(-1);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4.6);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4.6);
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
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwGetCursorPos(window, &lastX, &lastY);
	glfwSetKeyCallback(window, &KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);
	glfwSetScrollCallback(window, ScrollCallback);

	CalculateCubeTangentsAndBitangents();

	GLuint gBuffer, gDepthBuffer, gPosition, gNormal, gColor;
	CreateGbuffer(gBuffer, gDepthBuffer, gPosition, gNormal, gColor);

	//create shader programs
	ShaderProgram cubeProgram;
	//cubeProgram.AddShader("./source/shaders/cube-shader.vert", GL_VERTEX_SHADER);
	//cubeProgram.AddShader("./source/shaders/cube-shader.frag", GL_FRAGMENT_SHADER);
	cubeProgram.AddShader("./source/shaders/cube-shader-deferred-1.vert", GL_VERTEX_SHADER);
	cubeProgram.AddShader("./source/shaders/cube-shader-deferred-1.frag", GL_FRAGMENT_SHADER);
	cubeProgram.LinkProgram();
	unsigned int geometryPassProgramId = cubeProgram.GetProgramId();

	ShaderProgram cubeProgram2;
	//cubeProgram.AddShader("./source/shaders/cube-shader.vert", GL_VERTEX_SHADER);
	//cubeProgram.AddShader("./source/shaders/cube-shader.frag", GL_FRAGMENT_SHADER);
	cubeProgram2.AddShader("./source/shaders/cube-shader-deferred-2.vert", GL_VERTEX_SHADER);
	cubeProgram2.AddShader("./source/shaders/cube-shader-deferred-2.frag", GL_FRAGMENT_SHADER);
	cubeProgram2.LinkProgram();
	unsigned int lightPassProgramId = cubeProgram2.GetProgramId();

	ShaderProgram lightProgram;
	lightProgram.AddShader("./source/shaders/light-shader.vert", GL_VERTEX_SHADER);
	lightProgram.AddShader("./source/shaders/light-shader.frag", GL_FRAGMENT_SHADER);
	lightProgram.LinkProgram();
	unsigned int lightProgramId = lightProgram.GetProgramId();

	//////////////////////////////////// general shaders //////////////////////////////////////

	VertexBuffer cubeVertexBuffer(CUBE_VERTEX_DATA, sizeof(CUBE_VERTEX_DATA));

	glUseProgram(geometryPassProgramId);

	VertexArray cubeVertexArray;
	VertexBufferLayout cubeVertexLayout(14 * sizeof(float));
	cubeVertexLayout.Push<float>(0, 3, 0);					// position vertex attribute
	cubeVertexLayout.Push<float>(1, 3, sizeof(float) * 5);    // normal vertex attribute
	cubeVertexLayout.Push<float>(2, 2, sizeof(float) * 3);    // texture coordinates
	cubeVertexArray.AddAttributes(cubeVertexBuffer, cubeVertexLayout);
	glUseProgram(0);

	glUseProgram(lightPassProgramId);

	VertexBuffer quadVertexBuffer(QUAD_VERTEX_DATA, sizeof(QUAD_VERTEX_DATA));
	VertexArray quadVertexArray;
	VertexBufferLayout quadVertexLayout(5 * sizeof(float));
	quadVertexLayout.Push<float>(0, 3, 0);						// position vertex attribute
	quadVertexLayout.Push<float>(1, 2, sizeof(float) * 3);		// texture coordinates
	quadVertexArray.AddAttributes(quadVertexBuffer, quadVertexLayout);

	glUniform1i(glGetUniformLocation(lightPassProgramId, "gPosition"), 0);
	glUniform1i(glGetUniformLocation(lightPassProgramId, "gNormal"), 1);
	glUniform1i(glGetUniformLocation(lightPassProgramId, "gColor"), 2);

	glUseProgram(0);

	/////////////////////////////////// light-shaders /////////////////////////////////////////

	glUseProgram(lightProgramId);
	VertexArray lightVertexArray;
	VertexBufferLayout lightVertexLayout(14 * sizeof(float));
	lightVertexLayout.Push<float>(0, 3, 0);	// position vertex attribute
	lightVertexArray.AddAttributes(cubeVertexBuffer, lightVertexLayout);
	lightVertexArray.Unbind();
	glUseProgram(0);

	/////////////////////////////////////////////////////////////////////////////////////////////

	glm::mat4 model;
	glm::mat3 normalMatrix;

	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window)) {

		float frameStart = glfwGetTime();

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		processInput(window, frametime);

		glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(geometryPassProgramId);
		cubeVertexArray.Bind();

		glm::mat4 projection = glm::perspective(glm::radians(fieldOfView), 1000.0f / 600.0f, 0.1f, 100.0f);
		glUniformMatrix4fv(glGetUniformLocation(geometryPassProgramId, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		glUniformMatrix4fv(glGetUniformLocation(geometryPassProgramId, "view"), 1, GL_FALSE, glm::value_ptr(view));

		//////// MAIN-OBJECT ////////

		model = glm::mat4(1.0f);
		model = glm::rotate(model, (float)glfwGetTime() * glm::radians(35.0f), glm::vec3(0.5f, 1.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(geometryPassProgramId, "model"), 1, GL_FALSE, glm::value_ptr(model));

		normalMatrix = glm::mat3(glm::transpose(glm::inverse(model)));
		glUniformMatrix3fv(glGetUniformLocation(geometryPassProgramId, "normalMatrix"), 1, GL_FALSE, glm::value_ptr(normalMatrix));

		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(1.0, 0, 0));
		glUniformMatrix4fv(glGetUniformLocation(geometryPassProgramId, "model"), 1, GL_FALSE, glm::value_ptr(model));

		normalMatrix = glm::mat3(glm::transpose(glm::inverse(model)));
		glUniformMatrix3fv(glGetUniformLocation(geometryPassProgramId, "normalMatrix"), 1, GL_FALSE, glm::value_ptr(normalMatrix));

		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1.0, 0, 0));
		glUniformMatrix4fv(glGetUniformLocation(geometryPassProgramId, "model"), 1, GL_FALSE, glm::value_ptr(model));

		normalMatrix = glm::mat3(glm::transpose(glm::inverse(model)));
		glUniformMatrix3fv(glGetUniformLocation(geometryPassProgramId, "normalMatrix"), 1, GL_FALSE, glm::value_ptr(normalMatrix));

		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0, 1.0, 0));
		glUniformMatrix4fv(glGetUniformLocation(geometryPassProgramId, "model"), 1, GL_FALSE, glm::value_ptr(model));

		normalMatrix = glm::mat3(glm::transpose(glm::inverse(model)));
		glUniformMatrix3fv(glGetUniformLocation(geometryPassProgramId, "normalMatrix"), 1, GL_FALSE, glm::value_ptr(normalMatrix));

		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.0, 1.0, 0));
		glUniformMatrix4fv(glGetUniformLocation(geometryPassProgramId, "model"), 1, GL_FALSE, glm::value_ptr(model));

		normalMatrix = glm::mat3(glm::transpose(glm::inverse(model)));
		glUniformMatrix3fv(glGetUniformLocation(geometryPassProgramId, "normalMatrix"), 1, GL_FALSE, glm::value_ptr(normalMatrix));

		glDrawArrays(GL_TRIANGLES, 0, 36);

		glUseProgram(0);
		cubeVertexArray.Unbind();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		///////// RENDER-PASS //////////

		glUseProgram(lightPassProgramId);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gPosition);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, gNormal);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, gColor);

		glUniform3fv(glGetUniformLocation(lightPassProgramId, "lightColor"), 1, LIGHT_COLOR);
		glUniform3fv(glGetUniformLocation(lightPassProgramId, "lightPosition"), 1, LIGHT_POSITION);
		float   eyePosition[] = { cameraPos.x, cameraPos.y, cameraPos.z };
		glUniform3fv(glGetUniformLocation(lightPassProgramId, "eyePosition"), 1, eyePosition);
		glUniform1f(glGetUniformLocation(lightPassProgramId, "material.shininess"), 64.0f);

		quadVertexArray.Bind();
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		quadVertexArray.Unbind();

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

		glUseProgram(0);
		lightVertexArray.Unbind();

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		glfwPollEvents();
		glfwSwapBuffers(window);

		frametime = glfwGetTime() - frameStart;
	}


	glfwTerminate();

	return 0;

}