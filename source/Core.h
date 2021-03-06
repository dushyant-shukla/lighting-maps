#pragma once

#include <iostream>
#include <string>

#define ASSERT(x) if(!(x)) __debugbreak();

static const float CUBE_VERTEX_DATA[] = {

			-0.5f, -0.5f, -0.5f,		0.0f, 0.0f,		    0.0f,  0.0f, -1.0f,
			 0.5f, -0.5f, -0.5f,		1.0f, 0.0f,		    0.0f,  0.0f, -1.0f,
			 0.5f,  0.5f, -0.5f,		1.0f, 1.0f,		    0.0f,  0.0f, -1.0f,
			 0.5f,  0.5f, -0.5f,		1.0f, 1.0f,		    0.0f,  0.0f, -1.0f,
			-0.5f,  0.5f, -0.5f,		0.0f, 1.0f,		    0.0f,  0.0f, -1.0f,
			-0.5f, -0.5f, -0.5f,		0.0f, 0.0f,		    0.0f,  0.0f, -1.0f,

			-0.5f, -0.5f,  0.5f,		0.0f, 0.0f,		    0.0f,  0.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,		1.0f, 0.0f,		    0.0f,  0.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,		1.0f, 1.0f,		    0.0f,  0.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,		1.0f, 1.0f,		    0.0f,  0.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,		0.0f, 1.0f,		    0.0f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,		0.0f, 0.0f,		    0.0f,  0.0f, 1.0f,

			-0.5f,  0.5f,  0.5f,		1.0f, 0.0f,		    1.0f,  0.0f,  0.0f,
			-0.5f,  0.5f, -0.5f,		1.0f, 1.0f,		    1.0f,  0.0f,  0.0f,
			-0.5f, -0.5f, -0.5f,		0.0f, 1.0f,		    1.0f,  0.0f,  0.0f,
			-0.5f, -0.5f, -0.5f,		0.0f, 1.0f,		    1.0f,  0.0f,  0.0f,
			-0.5f, -0.5f,  0.5f,		0.0f, 0.0f,		    1.0f,  0.0f,  0.0f,
			-0.5f,  0.5f,  0.5f,		1.0f, 0.0f,		    1.0f,  0.0f,  0.0f,

			 0.5f,  0.5f,  0.5f,		1.0f, 0.0f,		    1.0f,  0.0f,  0.0f,
			 0.5f,  0.5f, -0.5f,		1.0f, 1.0f,		    1.0f,  0.0f,  0.0f,
			 0.5f, -0.5f, -0.5f,		0.0f, 1.0f,		    1.0f,  0.0f,  0.0f,
			 0.5f, -0.5f, -0.5f,		0.0f, 1.0f,		    1.0f,  0.0f,  0.0f,
			 0.5f, -0.5f,  0.5f,		0.0f, 0.0f,		    1.0f,  0.0f,  0.0f,
			 0.5f,  0.5f,  0.5f,		1.0f, 0.0f,		    1.0f,  0.0f,  0.0f,

			-0.5f, -0.5f, -0.5f,		0.0f, 1.0f,		    0.0f, -1.0f,  0.0f,
			 0.5f, -0.5f, -0.5f,		1.0f, 1.0f,		    0.0f, -1.0f,  0.0f,
			 0.5f, -0.5f,  0.5f,		1.0f, 0.0f,		    0.0f, -1.0f,  0.0f,
			 0.5f, -0.5f,  0.5f,		1.0f, 0.0f,		    0.0f, -1.0f,  0.0f,
			-0.5f, -0.5f,  0.5f,		0.0f, 0.0f,		    0.0f, -1.0f,  0.0f,
			-0.5f, -0.5f, -0.5f,		0.0f, 1.0f,		    0.0f, -1.0f,  0.0f,

			-0.5f,  0.5f, -0.5f,		0.0f, 1.0f,		    0.0f,  1.0f,  0.0f,
			 0.5f,  0.5f, -0.5f,		1.0f, 1.0f,		    0.0f,  1.0f,  0.0f,
			 0.5f,  0.5f,  0.5f,		1.0f, 0.0f,		    0.0f,  1.0f,  0.0f,
			 0.5f,  0.5f,  0.5f,		1.0f, 0.0f,		    0.0f,  1.0f,  0.0f,
			-0.5f,  0.5f,  0.5f,		0.0f, 0.0f,		    0.0f,  1.0f,  0.0f,
			-0.5f,  0.5f, -0.5f,		0.0f, 1.0f,		    0.0f,  1.0f,  0.0f
};

static float LIGHT_POSITION[] = { 0.0f, 0.5f, 1.0f };
static float OBJECT_COLOR[]	  = { 1.0f, 1.0f, 1.0f };
static float LIGHT_COLOR[]	  = { 1.0f, 1.0f, 1.0f };
static float EYE_POSITION[]	  = { 1.0f, 0.0f, 1.0f };
