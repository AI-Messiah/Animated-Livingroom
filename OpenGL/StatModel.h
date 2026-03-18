#ifndef STAT_MODEL
#define STAT_MODEL

#define GLEW_STATIC
#include "GL/glew.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/material.h"

#include <unordered_set>
#include <stack>
#include <string>
#include <vector>
#include <cassert>

#include "ErrorReport.h"

using namespace std;

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords1;
	glm::vec3 tangent;
	glm::vec3 bitangent;
	uint32_t meshNum;
};

struct Component
{
	std::vector<uint32_t> triangles;
};

class StatModel
{
public:
	StatModel();
	~StatModel();
	void LoadModel(string fileName);
	void Draw();
	bool isLights = false;
	vector<glm::vec3> centroids;
	float rHeight = 0;
	bool recHei = false;
	bool showMats = false;
	float wallLen = 0;
	float ceilHeight = 0;
	bool getVals = false;
	GLFWwindow* window;
private:
	void loadNode(aiNode* node, const aiScene* scene);
	void loadMesh(aiMesh* mesh, const aiScene* scene);
	void initBuffers();
	glm::vec3 computeCentroid(const Component& comp, const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);
	unsigned int meshCnt = 0;
	unsigned int firstIndex = 0;
	int drsize = 0;
	int indcnt = 0;
	int vertcnt = 0;
	bool mLoaded;
	std::vector<GLuint> mIndices;
	std::vector<Vertex> mVertices;
	GLuint mVBO, mVAO, mEBO;

};

#endif