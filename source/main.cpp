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

	//create shader programs
	ShaderProgram cubeProgram;
	cubeProgram.AddShader("./source/shaders/cube-shader.vert", GL_VERTEX_SHADER);
	cubeProgram.AddShader("./source/shaders/cube-shader.frag", GL_FRAGMENT_SHADER);
	cubeProgram.LinkProgram();
	unsigned int programId = cubeProgram.GetProgramId();
	GLuint colorSubroutineIndex;
	GLuint textureSubroutineIndex;

	ShaderProgram lightProgram;
	lightProgram.AddShader("./source/shaders/light-shader.vert", GL_VERTEX_SHADER);
	lightProgram.AddShader("./source/shaders/light-shader.frag", GL_FRAGMENT_SHADER);
	lightProgram.LinkProgram();
	unsigned int lightProgramId = lightProgram.GetProgramId();

	ShaderProgram normalMappingProgram;
	normalMappingProgram.AddShader("./source/shaders/normal-mapping.vert", GL_VERTEX_SHADER);
	normalMappingProgram.AddShader("./source/shaders/normal-mapping.frag", GL_FRAGMENT_SHADER);
	normalMappingProgram.LinkProgram();
	unsigned int normalMappingProgramId = normalMappingProgram.GetProgramId();
	GLuint phongLightingSubroutineIndex;
	GLuint defaultLightingSubroutineIndex;
	GLuint defaultShadingSubroutineIndex;
	GLuint normalMappingSubroutineIndex;
	GLuint parallexMappingSubroutineIndex;

	GLint colorOrTextureUniform;
	GLint normalOrParallexUniform;

	GLuint subroutineIndexes[2];

	//////////////////////////////////// general shaders //////////////////////////////////////

	VertexBuffer cubeVertexBuffer(CUBE_VERTEX_DATA, sizeof(CUBE_VERTEX_DATA));

	glUseProgram(programId);

	VertexArray cubeVertexArray;
	VertexBufferLayout cubeVertexLayout(14 * sizeof(float));
	cubeVertexLayout.Push<float>(0, 3, 0);					// position vertex attribute
	cubeVertexLayout.Push<float>(1, 3, sizeof(float) * 5);    // normal vertex attribute
	cubeVertexLayout.Push<float>(2, 2, sizeof(float) * 3);    // texture coordinates
	cubeVertexArray.AddAttributes(cubeVertexBuffer, cubeVertexLayout);

	Texture2D diffuseMap(GL_TEXTURE0, "./assets/container.png");
	diffuseMap.GenerateTexture();

	Texture2D specularMap(GL_TEXTURE1, "./assets/container-specular.png");
	specularMap.GenerateTexture();

	Texture2D emissionMap(GL_TEXTURE2, "./assets/matrix.jpg");
	emissionMap.GenerateTexture();

	printf("general shader:\n");
	int maxSub, maxSubU, activeS, countActiveSU;
	char name[256]; int len, numCompS;

	glGetIntegerv(GL_MAX_SUBROUTINES, &maxSub);
	glGetIntegerv(GL_MAX_SUBROUTINE_UNIFORM_LOCATIONS, &maxSubU);
	printf("Max Subroutines: %d  Max Subroutine Uniforms: %d\n", maxSub, maxSubU);

	glGetProgramStageiv(programId, GL_FRAGMENT_SHADER, GL_ACTIVE_SUBROUTINE_UNIFORMS, &countActiveSU);
	printf("Active Subroutines: %d\n", countActiveSU);

	for (int i = 0; i < countActiveSU; ++i) {

		glGetActiveSubroutineUniformName(programId, GL_FRAGMENT_SHADER, i, 256, &len, name);

		printf("Suroutine Uniform: %d name: %s\n", i, name);
		glGetActiveSubroutineUniformiv(programId, GL_FRAGMENT_SHADER, i, GL_NUM_COMPATIBLE_SUBROUTINES, &numCompS);

		int* s = (int*)malloc(sizeof(int) * numCompS);
		glGetActiveSubroutineUniformiv(programId, GL_FRAGMENT_SHADER, i, GL_COMPATIBLE_SUBROUTINES, s);
		printf("Compatible Subroutines:\n");
		for (int j = 0; j < numCompS; ++j) {

			glGetActiveSubroutineName(programId, GL_FRAGMENT_SHADER, s[j], 256, &len, name);
			printf("\t%d - %s\n", s[j], name);
		}
		printf("\n");
		free(s);
	}

	textureSubroutineIndex = glGetSubroutineIndex(programId, GL_FRAGMENT_SHADER, "PhongLightingWithTexture");
	colorSubroutineIndex = glGetSubroutineIndex(programId, GL_FRAGMENT_SHADER, "PhongLightingDefault");

	//cubeVertexBuffer.Unbind();
	//cubeVertexArray.Unbind();
	diffuseMap.Unbind();
	specularMap.Unbind();
	emissionMap.Unbind();
	glUseProgram(0);

	//////////////////////////////////////// normal-mapping shaders //////////////////////////////////////////

	glUseProgram(normalMappingProgramId);

	VertexArray cubeVertexArray2;
	VertexBufferLayout cubeVertexLayout2(14 * sizeof(float));
	cubeVertexLayout2.Push<float>(0, 3, 0);					// position vertex attribute
	cubeVertexLayout2.Push<float>(1, 3, sizeof(float) * 5);    // normal vertex attribute
	cubeVertexLayout2.Push<float>(2, 2, sizeof(float) * 3);    // texture coordinates
	cubeVertexLayout2.Push<float>(3, 3, sizeof(float) * 8);
	cubeVertexLayout2.Push<float>(3, 3, sizeof(float) * 11);
	cubeVertexArray2.AddAttributes(cubeVertexBuffer, cubeVertexLayout2);

	//Texture2D diffuseMap2(GL_TEXTURE0, "./assets/brickwall.jpg");
	//diffuseMap2.GenerateTexture();
	//Texture2D floorDiffuseMap(GL_TEXTURE0, "./assets/pebbles/Pebbles_019_Base Color.jpg");
	Texture2D floorDiffuseMap(GL_TEXTURE0, "./assets/stones/Stone_Path_003_baseColor.jpg");
	floorDiffuseMap.GenerateTexture();

	//Texture2D normalMap(GL_TEXTURE1, "./assets/brickwall_normal.jpg");
	//Texture2D floorNormalMap(GL_TEXTURE1, "./assets/pebbles/Pebbles_019_Normal.jpg");
	Texture2D floorNormalMap(GL_TEXTURE1, "./assets/stones/Stone_Path_003_normal.jpg");
	floorNormalMap.GenerateTexture();
	Texture2D floorDepthMap(GL_TEXTURE2, "./assets/stones/Stone_Path_003_height.png");
	floorDepthMap.GenerateTexture();

	Texture2D wallDiffuseMap(GL_TEXTURE0, "./assets/walls/bricks.jpg");
	wallDiffuseMap.GenerateTexture();
	Texture2D wallNormalMap(GL_TEXTURE1, "./assets/walls/bricks_normal.jpg");
	wallNormalMap.GenerateTexture();
	Texture2D wallDepthMap(GL_TEXTURE2, "./assets/walls/bricks_heightmap.jpg");
	wallDepthMap.GenerateTexture();


	printf("Normal mapping shader:\n");
	//int maxSub, maxSubU, activeS, countActiveSU;
	//char name[256]; int len, numCompS;

	glGetIntegerv(GL_MAX_SUBROUTINES, &maxSub);
	glGetIntegerv(GL_MAX_SUBROUTINE_UNIFORM_LOCATIONS, &maxSubU);
	printf("Max Subroutines: %d  Max Subroutine Uniforms: %d\n", maxSub, maxSubU);

	glGetProgramStageiv(normalMappingProgramId, GL_FRAGMENT_SHADER, GL_ACTIVE_SUBROUTINE_UNIFORMS, &countActiveSU);
	printf("Active Subroutines: %d\n", countActiveSU);

	for (int i = 0; i < countActiveSU; ++i) {

		glGetActiveSubroutineUniformName(normalMappingProgramId, GL_FRAGMENT_SHADER, i, 256, &len, name);

		printf("Suroutine Uniform: %d name: %s\n", i, name);
		glGetActiveSubroutineUniformiv(normalMappingProgramId, GL_FRAGMENT_SHADER, i, GL_NUM_COMPATIBLE_SUBROUTINES, &numCompS);

		int* s = (int*)malloc(sizeof(int) * numCompS);
		glGetActiveSubroutineUniformiv(normalMappingProgramId, GL_FRAGMENT_SHADER, i, GL_COMPATIBLE_SUBROUTINES, s);
		printf("Compatible Subroutines:\n");
		for (int j = 0; j < numCompS; ++j) {

			glGetActiveSubroutineName(normalMappingProgramId, GL_FRAGMENT_SHADER, s[j], 256, &len, name);
			printf("\t%d - %s\n", s[j], name);
		}
		printf("\n");
		free(s);
	}

	phongLightingSubroutineIndex = glGetSubroutineIndex(normalMappingProgramId, GL_FRAGMENT_SHADER, "PhongWithTexture");
	defaultLightingSubroutineIndex = glGetSubroutineIndex(normalMappingProgramId, GL_FRAGMENT_SHADER, "PhongWithColor");

	defaultShadingSubroutineIndex = glGetSubroutineIndex(normalMappingProgramId, GL_FRAGMENT_SHADER, "DefaultShading");
	normalMappingSubroutineIndex = glGetSubroutineIndex(normalMappingProgramId, GL_FRAGMENT_SHADER, "NormalMapping");
	parallexMappingSubroutineIndex = glGetSubroutineIndex(normalMappingProgramId, GL_FRAGMENT_SHADER, "ParallexMapping");

	colorOrTextureUniform = glGetSubroutineUniformLocation(normalMappingProgramId, GL_FRAGMENT_SHADER, "colorTechnique");
	normalOrParallexUniform = glGetSubroutineUniformLocation(normalMappingProgramId, GL_FRAGMENT_SHADER, "shadingTechnique");

	glUseProgram(0);

	/////////////////////////////////// light shaders /////////////////////////////////////////

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

		glUseProgram(programId);
		cubeVertexArray.Bind();

		glm::mat4 projection = glm::perspective(glm::radians(fieldOfView), 1000.0f / 600.0f, 0.1f, 100.0f);
		glUniformMatrix4fv(glGetUniformLocation(programId, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		//const float radius = 3.0f;
		//float camX = sin(glfwGetTime()) * radius;
		//float camZ = cos(glfwGetTime()) * radius;
		//glm::mat4 view = glm::lookAt(glm::vec3(EYE_POSITION[0], EYE_POSITION[1], EYE_POSITION[2]), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 3.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		//glm::mat4 view = glm::lookAt(glm::vec3(camX, 0.0f, camZ), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		glUniformMatrix4fv(glGetUniformLocation(programId, "view"), 1, GL_FALSE, glm::value_ptr(view));

		glUniform3fv(glGetUniformLocation(programId, "objectColor"), 1, OBJECT_COLOR);
		glUniform3fv(glGetUniformLocation(programId, "lightColor"), 1, LIGHT_COLOR);
		glUniform3fv(glGetUniformLocation(programId, "lightPosition"), 1, LIGHT_POSITION);
		float   eyePosition[] = { cameraPos.x, cameraPos.y, cameraPos.z };
		glUniform3fv(glGetUniformLocation(programId, "eyePosition"), 1, eyePosition/*EYE_POSITION*/);

		diffuseMap.Bind();
		glUniform1i(glGetUniformLocation(programId, "material.diffuse"), 0);

		specularMap.Bind();
		glUniform1i(glGetUniformLocation(programId, "material.specular"), 1);

		emissionMap.Bind();
		glUniform1i(glGetUniformLocation(programId, "material.emission"), 2);

		glUniform1f(glGetUniformLocation(programId, "material.shininess"), 64.0f);

		if (texture)
		{
			glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &textureSubroutineIndex);
		}
		else
		{
			glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &colorSubroutineIndex);
		}

		//diffuseMap.Unbind();

		//////////////////////////////////////////////////////////////////////// MAIN-OBJECT /////////////////////////////////////////////////////////////////////

		model = glm::mat4(1.0f);
		model = glm::rotate(model, (float)glfwGetTime() * glm::radians(35.0f), glm::vec3(0.5f, 1.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(programId, "model"), 1, GL_FALSE, glm::value_ptr(model));

		normalMatrix = glm::mat3(glm::transpose(glm::inverse(model)));
		glUniformMatrix3fv(glGetUniformLocation(programId, "normalMatrix"), 1, GL_FALSE, glm::value_ptr(normalMatrix));

		glDrawArrays(GL_TRIANGLES, 0, 36);

		glUseProgram(0);
		cubeVertexArray.Unbind();

		////////////////////////// CUBE - 2 (PLATFORM) shaded with normal map //////////////////////////////////

		glUseProgram(normalMappingProgramId);

		cubeVertexArray2.Bind();

		//glm::mat4 projection = glm::perspective(glm::radians(fieldOfView), 1000.0f / 600.0f, 0.1f, 100.0f);
		glUniformMatrix4fv(glGetUniformLocation(normalMappingProgramId, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		//glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		glUniformMatrix4fv(glGetUniformLocation(normalMappingProgramId, "view"), 1, GL_FALSE, glm::value_ptr(view));

		glUniform3fv(glGetUniformLocation(normalMappingProgramId, "objectColor"), 1, OBJECT_COLOR);
		glUniform3fv(glGetUniformLocation(normalMappingProgramId, "lightColor"), 1, LIGHT_COLOR);
		glUniform3fv(glGetUniformLocation(normalMappingProgramId, "lightPosition"), 1, LIGHT_POSITION);
		float eyePosition2[] = { cameraPos.x, cameraPos.y, cameraPos.z };
		glUniform3fv(glGetUniformLocation(programId, "eyePosition"), 1, eyePosition2/*EYE_POSITION*/);
		//glUniform3fv(glGetUniformLocation(normalMappingProgramId, "eyePosition"), 1, cameraPos/*EYE_POSITION*/);

		floorDiffuseMap.Bind();
		glUniform1i(glGetUniformLocation(normalMappingProgramId, "material.diffuse"), 0);
		floorNormalMap.Bind();
		glUniform1i(glGetUniformLocation(normalMappingProgramId, "material.normalMap"), 1);
		floorDepthMap.Bind();
		glUniform1i(glGetUniformLocation(normalMappingProgramId, "material.depthMap"), 2);

		glUniform1f(glGetUniformLocation(normalMappingProgramId, "material.shininess"), 64.0f);

		glUniform1ui(glGetUniformLocation(normalMappingProgramId, "parallexMapping"), parallexMapping);
		glUniform1ui(glGetUniformLocation(normalMappingProgramId, "normalMapping"), normalMapping);

		if (texture)
		{
			subroutineIndexes[0] = phongLightingSubroutineIndex;
		}
		else
		{
			subroutineIndexes[0] = defaultLightingSubroutineIndex;
		}

		if (normal && !parallex)
		{
			subroutineIndexes[1] = normalMappingSubroutineIndex;
		}
		else if (parallex)
		{
			subroutineIndexes[1] = parallexMappingSubroutineIndex;
		}
		else
		{
			subroutineIndexes[1] = defaultShadingSubroutineIndex;
		}

		glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 2, subroutineIndexes);


		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -0.65f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 0.02f, 5.0f));
		glUniformMatrix4fv(glGetUniformLocation(programId, "model"), 1, GL_FALSE, glm::value_ptr(model));

		normalMatrix = glm::mat3(glm::transpose(glm::inverse(model)));
		glUniformMatrix3fv(glGetUniformLocation(programId, "normalMatrix"), 1, GL_FALSE, glm::value_ptr(normalMatrix));

		glDrawArrays(GL_TRIANGLES, 0, 36);

		floorDiffuseMap.Unbind();
		floorNormalMap.Unbind();
		//floorDepthMap.Unbind();

		// wall back
		wallDiffuseMap.Bind();
		glUniform1i(glGetUniformLocation(normalMappingProgramId, "material.diffuse"), 0);
		wallNormalMap.Bind();
		glUniform1i(glGetUniformLocation(normalMappingProgramId, "material.normalMap"), 1);
		wallDepthMap.Bind();
		glUniform1i(glGetUniformLocation(normalMappingProgramId, "material.depthMap"), 2);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -0.3f, -1.5f));
		model = glm::scale(model, glm::vec3(5.0f, 1.5f, 0.5f));
		//model = glm::scale(model, glm::vec3(5.0f, 5.0f, 0.5f));
		glUniformMatrix4fv(glGetUniformLocation(programId, "model"), 1, GL_FALSE, glm::value_ptr(model));

		normalMatrix = glm::mat3(glm::transpose(glm::inverse(model)));
		glUniformMatrix3fv(glGetUniformLocation(programId, "normalMatrix"), 1, GL_FALSE, glm::value_ptr(normalMatrix));

		glDrawArrays(GL_TRIANGLES, 0, 36);

		// wall front
		//model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(0.0f, -0.3f, 1.5f));
		//model = glm::scale(model, glm::vec3(5.0f, 1.5f, 0.5f));
		//glUniformMatrix4fv(glGetUniformLocation(programId, "model"), 1, GL_FALSE, glm::value_ptr(model));

		//normalMatrix = glm::mat3(glm::transpose(glm::inverse(model)));
		//glUniformMatrix3fv(glGetUniformLocation(programId, "normalMatrix"), 1, GL_FALSE, glm::value_ptr(normalMatrix));

		//glDrawArrays(GL_TRIANGLES, 0, 36);

		// wall right
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(1.5f, -0.3f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 1.5f, 5.0f));
		glUniformMatrix4fv(glGetUniformLocation(programId, "model"), 1, GL_FALSE, glm::value_ptr(model));

		normalMatrix = glm::mat3(glm::transpose(glm::inverse(model)));
		glUniformMatrix3fv(glGetUniformLocation(programId, "normalMatrix"), 1, GL_FALSE, glm::value_ptr(normalMatrix));

		glDrawArrays(GL_TRIANGLES, 0, 36);

		// wall left
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1.5f, -0.3f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 1.5f, 5.0f));
		glUniformMatrix4fv(glGetUniformLocation(programId, "model"), 1, GL_FALSE, glm::value_ptr(model));

		normalMatrix = glm::mat3(glm::transpose(glm::inverse(model)));
		glUniformMatrix3fv(glGetUniformLocation(programId, "normalMatrix"), 1, GL_FALSE, glm::value_ptr(normalMatrix));

		glDrawArrays(GL_TRIANGLES, 0, 36);

		wallDiffuseMap.Unbind();
		wallNormalMap.Unbind();
		wallDepthMap.Unbind();

		glUseProgram(0);
		cubeVertexArray2.Unbind();

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