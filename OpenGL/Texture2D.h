#define GLEW_STATIC
#include "GL/glew.h"
#include <string>
#include "ErrorReport.h"
using std::string;

class Texture2D
{
public:
	Texture2D();
	virtual ~Texture2D();

	bool loadTexture(string fileName, bool generateMipMaps = true);
	void bind(GLuint texUnit = 0);
	void unbind(GLuint texUnit = 0);
	GLFWwindow* window;
private:
	GLuint mTexture;
};

