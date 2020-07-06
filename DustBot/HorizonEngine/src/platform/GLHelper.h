#pragma once
#include <GL/glew.h>
#include <sstream>

#include "utilities\Logger.h"
#include <iostream>

namespace Horizon
{
	namespace Graphics
	{

		//Maps GL error Glenum values to string equivalents.
		static std::string GetGLErrorName(const GLenum error)
		{
			switch (error)
			{
			case GL_NO_ERROR: return "GL_NO_ERROR";
			case GL_INVALID_ENUM: return "GL_INVALID_ENUM";
			case GL_INVALID_VALUE: return "GL_INVALID_VALUE";
			case GL_INVALID_OPERATION: return "GL_INVALID_OPERATION";
			case GL_INVALID_FRAMEBUFFER_OPERATION: return "GL_INVALID_FRAMEBUFFER_OPERATION";
			case GL_OUT_OF_MEMORY: return "GL_OUT_OF_MEMORY";
			case GL_STACK_UNDERFLOW: return "GL_STACK_UNDERFLOW";
			case GL_STACK_OVERFLOW: return "GL_STACK_OVERFLOW";
			default: return "";
			}
		}

		//Clear glGetError() flags.
		static void ClearGLError()
		{
			while (glGetError() != GL_NO_ERROR) {
				std::cout << glGetError() << std::endl;
			}
		}

		//Log glGetError() flags.
		static bool LogGLError(const char* function, const char* file, const int line)
		{
			while (const GLenum currentError = glGetError())
			{
				std::stringstream ss;
				ss << "[GL] [" << GetGLErrorName(currentError) << "] occurred calling function " << function << " file " << file << " line: " << std::to_string(line);

				Horizon::Logger::GetInstance()->LogError(ss.str());
				return false;
			}
			return true;
		}

#define GL(x) \
		ClearGLError();\
		x;\
		if(!LogGLError(#x, __FILE__, __LINE__)) __debugbreak()
	}
}