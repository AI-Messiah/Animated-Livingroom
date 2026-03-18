#include "Shader.h"
#include <fstream>
#include <iostream>
#include <sstream>

#include <glm/gtc/type_ptr.hpp>
Shader::Shader()
	: mHandle(0)
{
}

Shader::~Shader()
{
	glDeleteProgram(mHandle);
}

bool Shader::load(string name, bool hasGeom)
{
	shaderName = name;

	if (!RequireFile(window, "Shaders/" + name + ".vert")) return false;
	if (!RequireFile(window, "Shaders/" + name + ".frag")) return false;
	string vsString = fileToString("Shaders/" + name + ".vert");
	string fsString = fileToString("Shaders/" + name + ".frag");
	if (!preCheck(vsString, "Shaders/" + name + ".vert")) return false;
	if (!preCheck(fsString, "Shaders/" + name + ".frag")) return false;

	const GLchar* vsSourcePtr = vsString.c_str();
	const GLchar* fsSourcePtr = fsString.c_str();

	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vs, 1, &vsSourcePtr, NULL);
	glShaderSource(fs, 1, &fsSourcePtr, NULL);

	glCompileShader(vs);
	checkCompileErrors(vs, VERTEX);

	glCompileShader(fs);
	checkCompileErrors(fs, FRAGMENT);

	mHandle = glCreateProgram();
	if (mHandle == 0)
	{
		std::cerr << "Unable to create shader program!" << std::endl;
		return false;
	}

	glAttachShader(mHandle, vs);
	glAttachShader(mHandle, fs);

	GLuint geo;

	if (hasGeom) {
		if (!RequireFile(window, "Shaders/" + name + ".geom")) return false;
		string geoString = fileToString("Shaders/" + name + ".geom");
		if (!preCheck(geoString, "Shaders/" + name + ".geom")) return false;
		const GLchar* geoSourcePtr = geoString.c_str();
		geo = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geo, 1, &geoSourcePtr, NULL);
		glCompileShader(geo);
		checkCompileErrors(geo, GEOMETRY);
		glAttachShader(mHandle, geo);
	}

	glLinkProgram(mHandle);
	checkCompileErrors(mHandle, PROGRAM);

	glDeleteShader(vs);
	glDeleteShader(fs);
	if (hasGeom) glDeleteShader(geo);

	mUniformLocations.clear();

	block_index = glGetProgramResourceIndex(mHandle, GL_SHADER_STORAGE_BLOCK, "Centers");

	return true;

	return true;
}

void Shader::use()
{
	if (mHandle > 0)
		glUseProgram(mHandle);
}

string Shader::fileToString(string filename)
{
	std::stringstream ss;
	std::ifstream file;

	try
	{
		file.open(filename, std::ios::in);

		if (!file.fail())
		{
			// Using a std::stringstream is easier than looping through each line of the file
			ss << file.rdbuf();
		}

		file.close();
	}
	catch (std::exception ex)
	{
		std::cerr << "Error reading shader filename!" << std::endl;
	}

	return ss.str();
}

bool Shader::preCheck(string contents, string fileName)
{
	{
		if (contents.empty())
		{
			CorruptedFile(window, fileName + "\n\nShader file is empty.");
			return false;
		}
		if (contents.size() < 20)
		{
			CorruptedFile(window, fileName + "\n\nShader too small to be valid.");
			return false;
		}
		if (contents.find('\0') != std::string::npos)
		{
			CorruptedFile(window, fileName + "\n\nShader contains null bytes.");
			return false;
		}
		if (contents.find("#version") == std::string::npos)
		{
			CorruptedFile(window, fileName + "\n\nMissing #version directive.");
			return false;
		}
		return true;
	}
}


void Shader::checkCompileErrors(GLuint shader, ShaderType type)
{
	int status = 0;

	if (type == PROGRAM)
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &status);
		if (status == GL_FALSE)
		{
			FailedShader(window, shaderName + " program");
		}
	}
	else
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
		if (status == GL_FALSE)
		{
			switch (type) {
			case VERTEX:
				FailedShader(window, shaderName + " vertex");
				break;
			case FRAGMENT:
				FailedShader(window, shaderName + " fragment");
				break;
			case GEOMETRY:
				FailedShader(window, shaderName + " geometry");
				break;
			}
		}
	}

}


GLuint Shader::getProgram() const
{
	return mHandle;
}

void Shader::bindSSBO(GLuint ssbo)
{
	GLuint ssbo_binding_point_index = 0;
	glShaderStorageBlockBinding(mHandle, block_index, ssbo_binding_point_index);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, ssbo_binding_point_index, ssbo);
}


void Shader::setUniform(const GLchar* name, const glm::vec2& v)
{
	GLint loc = getUniformLocation(name);
	glUniform2f(loc, v.x, v.y);
}


void Shader::setUniform(const GLchar* name, const glm::vec3& v)
{
	GLint loc = getUniformLocation(name);
	glUniform3f(loc, v.x, v.y, v.z);
}


void Shader::setUniform(const GLchar* name, const glm::vec4& v)
{
	GLint loc = getUniformLocation(name);
	glUniform4f(loc, v.x, v.y, v.z, v.w);
}


void Shader::setUniform(const GLchar* name, const glm::mat4& m)
{
	GLint loc = getUniformLocation(name);

	// loc = location of uniform in shader
	// count = how many matrices (1 if not an array of mats)
	// transpose = False for opengl because column major
	// value = the matrix to set for the uniform
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(m));
}

void Shader::setUniform(const GLchar* name, const GLfloat& m) {
	GLint loc = getUniformLocation(name);
	glUniform1f(loc, m);
}
void Shader::setUniformI(const GLchar* name, const GLint& m)
{
	GLint loc = getUniformLocation(name);
	glUniform1i(loc, m);
}
void Shader::setUniformSampler(const GLchar* name, const GLint slot) {
	glActiveTexture(GL_TEXTURE0 + slot);
	GLint loc = getUniformLocation(name);
	glUniform1i(loc, slot);
}

void Shader::setUniform(const string name, const glm::mat4& m)
{
	setUniform(name.c_str(), m);
}

GLint Shader::getUniformLocation(const GLchar* name)
{
	std::map<string, GLint>::iterator it = mUniformLocations.find(name);

	// Only need to query the shader program IF it doesn't already exist.
	if (it == mUniformLocations.end())
	{
		// Find it and add it to the map
		mUniformLocations[name] = glGetUniformLocation(mHandle, name);
	}

	// Return it
	return mUniformLocations[name];
}
