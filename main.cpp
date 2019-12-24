#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "stb_image.h"

#include <iostream>

#include "Shader.cpp"


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

	//create shader programs
	unsigned int programId = CreateShaderProgram();
	unsigned int lightProgramId = CreateLightShaderProgram();

	float buffer[] = {

		-0.5f, -0.5f, -0.5f,		0.0f, 0.0f,			 0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f,		1.0f, 0.0f,			 0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,		1.0f, 1.0f,			 0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,		1.0f, 1.0f,			 0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,		0.0f, 1.0f,			 0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,		0.0f, 0.0f,			 0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,		0.0f, 0.0f,			 0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,		1.0f, 0.0f,			 0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,		1.0f, 1.0f,			 0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,		1.0f, 1.0f,			 0.0f,  0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,		0.0f, 1.0f,			 0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,		0.0f, 0.0f,			 0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f,  0.5f,		1.0f, 0.0f,			-1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,		1.0f, 1.0f,			-1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,		0.0f, 1.0f,			-1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,		0.0f, 1.0f,			-1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,		0.0f, 0.0f,			-1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,		1.0f, 0.0f,			-1.0f,  0.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,		1.0f, 0.0f,			 1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,		1.0f, 1.0f,			 1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,		0.0f, 1.0f,			 1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,		0.0f, 1.0f,			 1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,		0.0f, 0.0f,			 1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,		1.0f, 0.0f,			 1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,		0.0f, 1.0f,			 0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,		1.0f, 1.0f,			 0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,		1.0f, 0.0f,			 0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,		1.0f, 0.0f,			 0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,		0.0f, 0.0f,			 0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,		0.0f, 1.0f,			 0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,		0.0f, 1.0f,			 0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,		1.0f, 1.0f,			 0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,		1.0f, 0.0f,			 0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,		1.0f, 0.0f,			 0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,		0.0f, 0.0f,			 0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,		0.0f, 1.0f,			 0.0f,  1.0f,  0.0f
	};

	unsigned int vaoId, lightVaoId, bufferId, indexBufferId;
	unsigned int diffuseMap, specularMap, emissionMap;


	//////////////////////////////////// object settings //////////////////////////////////////

	glUseProgram(programId);
	glGenVertexArrays(1, &vaoId);
	glBindVertexArray(vaoId);
	
	glGenBuffers(1, &bufferId);
	glGenBuffers(1, &indexBufferId);

	glBindBuffer(GL_ARRAY_BUFFER, bufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(buffer), buffer, GL_STATIC_DRAW);

	// position vertex attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);

	// texture coordinates
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (sizeof(float) * 3));
	glEnableVertexAttribArray(2);

	// normal vertex attributes
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(sizeof(float) * 5));
	glEnableVertexAttribArray(1);

	glGenTextures(1, &diffuseMap);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, diffuseMap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height, nrChannels;
	unsigned char* data = stbi_load("container.png", &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "\nFailed to load texture..." << std::endl;
	}
	stbi_image_free(data);

	glGenTextures(1, &specularMap);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, specularMap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	data = stbi_load("container-specular.png", &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "\nFailed to load specular texture..." << std::endl;
	}
	stbi_image_free(data);


	glGenTextures(1, &emissionMap);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, emissionMap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	data = stbi_load("matrix.jpg", &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "\nFailed to load specular texture..." << std::endl;
	}
	stbi_image_free(data);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glUseProgram(0);

	////////////////////////////////////////////////////////////////////////////////////////////

	/////////////////////////////////// light settings /////////////////////////////////////////

	glGenVertexArrays(1, &lightVaoId);
	glBindVertexArray(lightVaoId);

	glBindBuffer(GL_ARRAY_BUFFER, bufferId);

	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0));
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	/////////////////////////////////////////////////////////////////////////////////////////////


	glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1000.0f / 600.0f, 0.1f, 100.0f);
	
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

	glm::mat4 model;
	glm::mat3 normalMatrix;

	glm::vec3 lightPos(0.0f, 0.5f, 1.0f);
	float lightPosition[] = { 0.0f, 0.5f, 1.0f };
	float objectColor[] = { 1.0f, 1.0f, 1.0f };
	float lightColor[] = { 1.0f, 1.0f, 1.0f };
	float eyePosition[] = { 1.0f, 0.0f, 1.0f };

	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window)) {
		//glClearColor(0.3f, 0.3f, 0.7f, 1.0f);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		//////////////////////////////////////////////////////////////////////// OBJECT /////////////////////////////////////////////////////////////////////
		glUseProgram(programId);
		glBindVertexArray(vaoId);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);

		glUniformMatrix4fv(glGetUniformLocation(programId, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(programId, "view"), 1, GL_FALSE, glm::value_ptr(view));

		model = glm::mat4(1.0f);
		model = glm::rotate(model, (float)glfwGetTime() * glm::radians(35.0f), glm::vec3(0.5f, 1.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(programId, "model"), 1, GL_FALSE, glm::value_ptr(model));

		normalMatrix = glm::mat3(glm::transpose(glm::inverse(model)));
		glUniformMatrix3fv(glGetUniformLocation(programId, "normalMatrix"), 1, GL_FALSE, glm::value_ptr(normalMatrix));

		glUniform3fv(glGetUniformLocation(programId, "objectColor"), 1, objectColor);
		glUniform3fv(glGetUniformLocation(programId, "lightColor"), 1, lightColor);
		glUniform3fv(glGetUniformLocation(programId, "lightPosition"), 1, lightPosition);
		glUniform3fv(glGetUniformLocation(programId, "eyePosition"), 1, eyePosition);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);
		glUniform1i(glGetUniformLocation(programId, "material.diffuse"), 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap);
		glUniform1i(glGetUniformLocation(programId, "material.specular"), 1);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, emissionMap);
		glUniform1i(glGetUniformLocation(programId, "material.emission"), 2);

		glUniform1f(glGetUniformLocation(programId, "material.shininess"), 32.0f);
		
		glUniform1ui(glGetUniformLocation(programId, "debug"), GL_FALSE);

		glDrawArrays(GL_TRIANGLES, 0, 36);

		/*glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glUniform1ui(glGetUniformLocation(programId, "debug"), GL_TRUE);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);*/

		glUseProgram(0);
		glBindVertexArray(0);

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////// LIGHT ///////////////////////////////////////////////////////////////////////////////


		glUseProgram(lightProgramId);
		glBindVertexArray(lightVaoId);

		glUniformMatrix4fv(glGetUniformLocation(lightProgramId, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(lightProgramId, "view"), 1, GL_FALSE, glm::value_ptr(view));

		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::rotate(model, (float)glfwGetTime() * glm::radians(35.0f), glm::vec3(0.5f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f));
		glUniformMatrix4fv(glGetUniformLocation(lightProgramId, "model"), 1, GL_FALSE, glm::value_ptr(model));

		glUniform1ui(glGetUniformLocation(lightProgramId, "debug"), GL_FALSE);

		glDrawArrays(GL_TRIANGLES, 0, 36);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glUniform1ui(glGetUniformLocation(lightProgramId, "debug"), GL_TRUE);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glUseProgram(0);
		glBindVertexArray(0);

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		glfwPollEvents();
		glfwSwapBuffers(window);
	}


	glDeleteBuffers(1, &bufferId);
	glDeleteVertexArrays(1, &vaoId);
	glDeleteVertexArrays(1, &lightVaoId);

	glfwTerminate();

	return 0;

}