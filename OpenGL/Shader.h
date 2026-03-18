#ifndef SHADER_F
#define SHADER_F

#include <string>
#include <map>
#define GLEW_STATIC
#include "GL/glew.h"
#include "glm/glm.hpp"
#include "ErrorReport.h"
using std::string;

class Shader
{
public:
	Shader();
	~Shader();
	Shader(const Shader&) = delete;
	Shader& operator=(const Shader&) = delete;
	enum ShaderType
	{
		VERTEX,
		FRAGMENT,
		GEOMETRY,
		PROGRAM
	};

	bool load(string shaderName, bool hasGeom = false);

	void use();

	GLuint getProgram() const;

	void bindSSBO(GLuint ssbo);
	void setUniform(const GLchar* name, const glm::vec2& v);
	void setUniform(const GLchar* name, const glm::vec3& v);
	void setUniform(const GLchar* name, const glm::vec4& v);
	void setUniform(const GLchar* name, const glm::mat4& m);
	void setUniform(const GLchar* name, const GLfloat& m);
	void setUniformI(const GLchar* name, const GLint& m);
	void setUniformSampler(const GLchar* name, const GLint slot);
	void setUniform(const string name, const glm::mat4& m);

	GLint getUniformLocation(const GLchar* name);

	GLFWwindow* window;
private:

	string fileToString(string filename);
	bool preCheck(string contents, string fileName);
	void  checkCompileErrors(GLuint shader, ShaderType type);

	string shaderName;

	GLuint block_index;
	GLuint mHandle;
	std::map<string, GLint> mUniformLocations;
};

#endif 