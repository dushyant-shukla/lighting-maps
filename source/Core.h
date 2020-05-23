#pragma once

#include <iostream>
#include <string>

#include <glm/glm.hpp>

#define ASSERT(x) if(!(x)) __debugbreak();

static float CUBE_VERTEX_DATA[] = {
			// position(3)				texture(2)			normal(3)				tangent(3)				bi-tangent(3)
			//0		1		2			3		4			5		6		7		8		9		10		11		12		13
			-0.5f, -0.5f, -0.5f,		0.0f, 0.0f,		    0.0f,  0.0f, -1.0f,		0.0f,	0.0f,	0.0f,	0.0f,	0.0f,	0.0f,// 1

			//14	15		16			17		18			19		20		21		22		23		24		25		26		27
			 0.5f, -0.5f, -0.5f,		1.0f, 0.0f,		    0.0f,  0.0f, -1.0f,		0.0f,	0.0f,	0.0f,	0.0f,	0.0f,	0.0f,// 2

			//28	29		30			31		32			33		34		35		36		37		38		39		40		41
			 0.5f,  0.5f, -0.5f,		1.0f, 1.0f,		    0.0f,  0.0f, -1.0f,		0.0f,	0.0f,	0.0f,	0.0f,	0.0f,	0.0f,// 3
			 0.5f,  0.5f, -0.5f,		1.0f, 1.0f,		    0.0f,  0.0f, -1.0f,		0.0f,	0.0f,	0.0f,	0.0f,	0.0f,	0.0f,// 3
			-0.5f,  0.5f, -0.5f,		0.0f, 1.0f,		    0.0f,  0.0f, -1.0f,		0.0f,	0.0f,	0.0f,	0.0f,	0.0f,	0.0f,// 4
			-0.5f, -0.5f, -0.5f,		0.0f, 0.0f,		    0.0f,  0.0f, -1.0f,		0.0f,	0.0f,	0.0f,	0.0f,	0.0f,	0.0f,// 1

			-0.5f, -0.5f,  0.5f,		0.0f, 0.0f,		    0.0f,  0.0f, 1.0f,		0.0f,	0.0f,	0.0f,	0.0f,	0.0f,	0.0f,
			 0.5f, -0.5f,  0.5f,		1.0f, 0.0f,		    0.0f,  0.0f, 1.0f,		0.0f,	0.0f,	0.0f,	0.0f,	0.0f,	0.0f,
			 0.5f,  0.5f,  0.5f,		1.0f, 1.0f,		    0.0f,  0.0f, 1.0f,		0.0f,	0.0f,	0.0f,	0.0f,	0.0f,	0.0f,
			 0.5f,  0.5f,  0.5f,		1.0f, 1.0f,		    0.0f,  0.0f, 1.0f,		0.0f,	0.0f,	0.0f,	0.0f,	0.0f,	0.0f,
			-0.5f,  0.5f,  0.5f,		0.0f, 1.0f,		    0.0f,  0.0f, 1.0f,		0.0f,	0.0f,	0.0f,	0.0f,	0.0f,	0.0f,
			-0.5f, -0.5f,  0.5f,		0.0f, 0.0f,		    0.0f,  0.0f, 1.0f,		0.0f,	0.0f,	0.0f,	0.0f,	0.0f,	0.0f,

			-0.5f,  0.5f,  0.5f,		1.0f, 0.0f,		    -1.0f,  0.0f,  0.0f,	0.0f,	0.0f,	0.0f,	0.0f,	0.0f,	0.0f,
			-0.5f,  0.5f, -0.5f,		1.0f, 1.0f,		    -1.0f,  0.0f,  0.0f,	0.0f,	0.0f,	0.0f,	0.0f,	0.0f,	0.0f,
			-0.5f, -0.5f, -0.5f,		0.0f, 1.0f,		    -1.0f,  0.0f,  0.0f,	0.0f,	0.0f,	0.0f,	0.0f,	0.0f,	0.0f,
			-0.5f, -0.5f, -0.5f,		0.0f, 1.0f,		    -1.0f,  0.0f,  0.0f,	0.0f,	0.0f,	0.0f,	0.0f,	0.0f,	0.0f,
			-0.5f, -0.5f,  0.5f,		0.0f, 0.0f,		    -1.0f,  0.0f,  0.0f,	0.0f,	0.0f,	0.0f,	0.0f,	0.0f,	0.0f,
			-0.5f,  0.5f,  0.5f,		1.0f, 0.0f,		    -1.0f,  0.0f,  0.0f,	0.0f,	0.0f,	0.0f,	0.0f,	0.0f,	0.0f,

			 0.5f,  0.5f,  0.5f,		1.0f, 0.0f,		    1.0f,  0.0f,  0.0f,		0.0f,	0.0f,	0.0f,	0.0f,	0.0f,	0.0f,
			 0.5f,  0.5f, -0.5f,		1.0f, 1.0f,		    1.0f,  0.0f,  0.0f,		0.0f,	0.0f,	0.0f,	0.0f,	0.0f,	0.0f,
			 0.5f, -0.5f, -0.5f,		0.0f, 1.0f,		    1.0f,  0.0f,  0.0f,		0.0f,	0.0f,	0.0f,	0.0f,	0.0f,	0.0f,
			 0.5f, -0.5f, -0.5f,		0.0f, 1.0f,		    1.0f,  0.0f,  0.0f,		0.0f,	0.0f,	0.0f,	0.0f,	0.0f,	0.0f,
			 0.5f, -0.5f,  0.5f,		0.0f, 0.0f,		    1.0f,  0.0f,  0.0f,		0.0f,	0.0f,	0.0f,	0.0f,	0.0f,	0.0f,
			 0.5f,  0.5f,  0.5f,		1.0f, 0.0f,		    1.0f,  0.0f,  0.0f,		0.0f,	0.0f,	0.0f,	0.0f,	0.0f,	0.0f,

			-0.5f, -0.5f, -0.5f,		0.0f, 1.0f,		    0.0f, -1.0f,  0.0f,		0.0f,	0.0f,	0.0f,	0.0f,	0.0f,	0.0f,
			 0.5f, -0.5f, -0.5f,		1.0f, 1.0f,		    0.0f, -1.0f,  0.0f,		0.0f,	0.0f,	0.0f,	0.0f,	0.0f,	0.0f,
			 0.5f, -0.5f,  0.5f,		1.0f, 0.0f,		    0.0f, -1.0f,  0.0f,		0.0f,	0.0f,	0.0f,	0.0f,	0.0f,	0.0f,
			 0.5f, -0.5f,  0.5f,		1.0f, 0.0f,		    0.0f, -1.0f,  0.0f,		0.0f,	0.0f,	0.0f,	0.0f,	0.0f,	0.0f,
			-0.5f, -0.5f,  0.5f,		0.0f, 0.0f,		    0.0f, -1.0f,  0.0f,		0.0f,	0.0f,	0.0f,	0.0f,	0.0f,	0.0f,
			-0.5f, -0.5f, -0.5f,		0.0f, 1.0f,		    0.0f, -1.0f,  0.0f,		0.0f,	0.0f,	0.0f,	0.0f,	0.0f,	0.0f,

			-0.5f,  0.5f, -0.5f,		0.0f, 1.0f,		    0.0f,  1.0f,  0.0f,		0.0f,	0.0f,	0.0f,	0.0f,	0.0f,	0.0f,
			 0.5f,  0.5f, -0.5f,		1.0f, 1.0f,		    0.0f,  1.0f,  0.0f,		0.0f,	0.0f,	0.0f,	0.0f,	0.0f,	0.0f,
			 0.5f,  0.5f,  0.5f,		1.0f, 0.0f,		    0.0f,  1.0f,  0.0f,		0.0f,	0.0f,	0.0f,	0.0f,	0.0f,	0.0f,
			 0.5f,  0.5f,  0.5f,		1.0f, 0.0f,		    0.0f,  1.0f,  0.0f,		0.0f,	0.0f,	0.0f,	0.0f,	0.0f,	0.0f,
			-0.5f,  0.5f,  0.5f,		0.0f, 0.0f,		    0.0f,  1.0f,  0.0f,		0.0f,	0.0f,	0.0f,	0.0f,	0.0f,	0.0f,
																														    //503
			-0.5f,  0.5f, -0.5f,		0.0f, 1.0f,		    0.0f,  1.0f,  0.0f,		0.0f,	0.0f,	0.0f,	0.0f,	0.0f,	0.0f
};

static void CalculateCubeTangentsAndBitangents()
{
	int vertexDataLastIndex = 490; // 503; // 14 * 36 - 1
	
	int i = 0;

	while (i < vertexDataLastIndex)
	{
		glm::vec3 edge1(CUBE_VERTEX_DATA[i + 14]- CUBE_VERTEX_DATA[i],
						CUBE_VERTEX_DATA[i + 1 + 14] - CUBE_VERTEX_DATA[i + 1],
						CUBE_VERTEX_DATA[i + 2 + 14] - CUBE_VERTEX_DATA[i + 2]
					); // pos2 - pos1

		glm::vec3 edge2(CUBE_VERTEX_DATA[i + 28] - CUBE_VERTEX_DATA[i],
						CUBE_VERTEX_DATA[i + 1 + 28] - CUBE_VERTEX_DATA[i + 1],
						CUBE_VERTEX_DATA[i + 2 + 28] - CUBE_VERTEX_DATA[i + 2]
					); // pos3 - pos1

		glm::vec2 deltaUV1(	CUBE_VERTEX_DATA[i + 14 + 3] - CUBE_VERTEX_DATA[i + 3],
							CUBE_VERTEX_DATA[i + 14 + 4] - CUBE_VERTEX_DATA[i + 4]
						);

		glm::vec2 deltaUV2(	CUBE_VERTEX_DATA[i + 28 + 3] - CUBE_VERTEX_DATA[i + 3],
							CUBE_VERTEX_DATA[i + 28 + 4] - CUBE_VERTEX_DATA[i + 4]
						);

		float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

		float x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
		float y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
		float z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
		glm::vec3 tangent(x, y, z);
		tangent = glm::normalize(tangent);

		x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
		y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
		z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
		glm::vec3 biTangent(x, y, z);
		biTangent = glm::normalize(biTangent);

		// tangents
		CUBE_VERTEX_DATA[i + 8] = tangent.x; // x
		CUBE_VERTEX_DATA[i + 14 + 8] = tangent.x; // x
		CUBE_VERTEX_DATA[i + 28 + 8] = tangent.x; // x

		CUBE_VERTEX_DATA[i + 8 + 1] = tangent.y; //y
		CUBE_VERTEX_DATA[i + 14 + 8 + 1] = tangent.y; //y
		CUBE_VERTEX_DATA[i + 28 + 8 + 1] = tangent.y; //y

		CUBE_VERTEX_DATA[i + 8 + 2] = tangent.z; //z
		CUBE_VERTEX_DATA[i + 14 + 8 + 2] = tangent.z; //z
		CUBE_VERTEX_DATA[i + 28 + 8 + 2] = tangent.z; //z

		// bi-tangents
		CUBE_VERTEX_DATA[i + 11] = biTangent.x; // x
		CUBE_VERTEX_DATA[i + 14 + 11] = biTangent.x; // x
		CUBE_VERTEX_DATA[i + 28 + 11] = biTangent.x; // x

		CUBE_VERTEX_DATA[i + 11 + 1] = biTangent.y; //y
		CUBE_VERTEX_DATA[i + 14 + 11 + 1] = biTangent.y; //y
		CUBE_VERTEX_DATA[i + 28 + 11 + 1] = biTangent.y; //y

		CUBE_VERTEX_DATA[i + 11 + 2] = biTangent.z; //z
		CUBE_VERTEX_DATA[i + 14 + 11 + 2] = biTangent.z; //z
		CUBE_VERTEX_DATA[i + 28 + 11 + 2] = biTangent.z; //z

		i += 14 * 3; // [jump 42 indexes or 1 triangle's data or 3 vertices]
	}

	CUBE_VERTEX_DATA[0];
}

//static const float LIGHT_POSITION[]	= { 0.0f, 0.5f, 1.0f };
static const float LIGHT_POSITION[] = { 0.0f, 5.0f, 0.0f };
static const float   OBJECT_COLOR[]	= { 1.0f, 1.0f, 1.0f };
static const float    LIGHT_COLOR[]	= { 1.0f, 1.0f, 1.0f };
static const float   EYE_POSITION[]	= { 0.0f, 0.0f, 3.0f };
//static const float   EYE_POSITION[]	= { 0.0f, 0.0f, -3.0f };

static glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
static glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
static glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
static const float cameraSpeed = 2.5f;
static double yaw = -90.0;
static double pitch = 0.0;
static const double PITCH_MAX = 89.0;
static const double PITCH_MIN = -89.0;
static float fieldOfView = 45.0f;
static const float FIELD_OF_VIEW_MIN = 1.0;
static const float FIELD_OF_VIEW_MAX = 45.0;

static void ResetCamera()
{
	cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	yaw = -90.0;
	pitch = 0.0;
	fieldOfView = 45.0f;
}


static float frametime = 0.0f;

// Mouse
static double lastX = 0.0f;
static double lastY = 0.0f;
static bool firstMouse = true;

// Rendering Techniques state variables
static bool texture		= true;
static bool normal		= true;
static bool parallex	= false;

static unsigned int parallexMapping = 0;
static unsigned int normalMapping = 0;
