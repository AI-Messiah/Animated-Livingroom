#include "StatModel.h"
#include <fstream>

StatModel::StatModel()
{
}

StatModel::~StatModel()
{
	if (mVAO != NULL) {
		glDeleteVertexArrays(1, &mVAO);
		glDeleteBuffers(1, &mVBO);
		glDeleteBuffers(1, &mEBO);
	}
}

void StatModel::LoadModel(string fileName)
{
	fileName = "../Models/" + fileName;

	if (!RequireFile(window, fileName))
		return;

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(fileName, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);

	if (!scene ||
		scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
		!scene->mRootNode)
	{
		CorruptedFile(window, fileName + "\n\n" + importer.GetErrorString());
		return;
	}

	loadNode(scene->mRootNode, scene);

	if (mVertices.size() > 0) {
		if (getVals) {
			for (int i = 0; i < mVertices.size(); i++) {
				glm::vec4 tpos = glm::rotate(glm::mat4(1), glm::radians(270.0f), glm::vec3(1, 0, 0)) * glm::vec4(mVertices[i].position.x, mVertices[i].position.y, mVertices[i].position.z, 1);
				glm::vec3 rpos = glm::vec3(tpos.x / tpos.w, tpos.y / tpos.w, tpos.z / tpos.w);
				if (rpos.x > wallLen) wallLen = rpos.x;
				if (rpos.y > ceilHeight) ceilHeight = rpos.y;
			}
		}

		initBuffers();
		drsize = mIndices.size();
		mVertices.clear();
		mIndices.clear();
	}
}

void StatModel::Draw()
{
	if (drsize > 0) {
		glBindVertexArray(mVAO);
		glDrawElements(GL_TRIANGLES, drsize, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
}

void StatModel::loadNode(aiNode* node, const aiScene* scene)
{
	for (size_t i = 0; i < node->mNumMeshes; i++) loadMesh(scene->mMeshes[node->mMeshes[i]], scene);

	for (size_t i = 0; i < node->mNumChildren; i++) loadNode(node->mChildren[i], scene);
}

void StatModel::loadMesh(aiMesh* mesh, const aiScene* scene)
{
	aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];

	string mname = "";

	aiString matName;
	if (mat->Get(AI_MATKEY_NAME, matName) == AI_SUCCESS)
	{
		std::string name(matName.C_Str());
		mname = name;
	}

	Vertex vert;

	if (mesh->mNumVertices > 0) {
		mVertices.resize(mVertices.size() + mesh->mNumVertices);

		for (size_t i = 0; i < mesh->mNumVertices; i++) {
			vert.position = { mesh->mVertices[i].x, mesh->mVertices[i].y ,mesh->mVertices[i].z };
			vert.normal = { mesh->mNormals[i].x , mesh->mNormals[i].y, mesh->mNormals[i].z };
			if (mesh->HasTangentsAndBitangents()) {
				vert.tangent = { mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z };
				vert.bitangent = { mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z };
			}
			else {
				vert.tangent = glm::vec3(0, 0, 0);
				vert.bitangent = glm::vec3(0, 0, 0);
			}
			if (mesh->mTextureCoords[0]) {
				vert.texCoords1 = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };
			}
			else {
				vert.texCoords1 = { 0, 0 };
			}
			vert.meshNum = meshCnt;
			mVertices[vertcnt] = vert;
			vertcnt++;
		}

		mIndices.resize(mIndices.size() + mesh->mNumFaces * 3);
		for (size_t i = 0; i < mesh->mNumFaces; i++) {
			aiFace face = mesh->mFaces[i];
			for (size_t j = 0; j < 3; j++) {
				mIndices[indcnt] = firstIndex + face.mIndices[j];
				indcnt++;
			}
		}
		meshCnt++;
		firstIndex += mesh->mNumVertices;
	}
}

void StatModel::initBuffers()
{
	if (isLights) {
		std::vector<std::vector<uint32_t>> vertexToTriangles(mVertices.size());

		for (uint32_t t = 0; t < mIndices.size() / 3; ++t)
		{
			for (int i = 0; i < 3; ++i)
			{
				uint32_t v = mIndices[t * 3 + i];
				vertexToTriangles[v].push_back(t);
			}
		}

		std::vector<bool> visited(mIndices.size() / 3, false);

		std::vector<Component> components;

		for (uint32_t t = 0; t < mIndices.size() / 3; ++t)
		{
			if (visited[t]) continue;

			Component comp;
			std::stack<uint32_t> stack;
			stack.push(t);
			visited[t] = true;

			while (!stack.empty())
			{
				uint32_t cur = stack.top();
				stack.pop();
				comp.triangles.push_back(cur);

				for (int i = 0; i < 3; ++i)
				{
					uint32_t v = mIndices[cur * 3 + i];
					for (uint32_t adj : vertexToTriangles[v])
					{
						if (!visited[adj])
						{
							visited[adj] = true;
							stack.push(adj);
						}
					}
				}
			}

			components.push_back(std::move(comp));
		}

		for (int i = 0; i < components.size(); i++) {
			glm::vec3 val = computeCentroid(components[i], mVertices, mIndices);
			centroids.push_back(val);
		}
	}

	if (recHei) {
		float height = -10000;
		float low = 10000;
		for (int i = 0; i < mVertices.size(); i++) {
			glm::vec4 spos = glm::vec4(mVertices[i].position, 1);
			glm::vec4 tpos = glm::rotate(glm::mat4(1), glm::radians(270.0f), glm::vec3(1, 0, 0)) * spos;
			float hei = tpos.y / tpos.w;
			if (hei > height) height = hei;
			if (hei < low) low = hei;
		}
		rHeight = height - low;
	}


	glGenVertexArrays(1, &mVAO);
	glGenBuffers(1, &mVBO);
	glGenBuffers(1, &mEBO);

	glBindVertexArray(mVAO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(Vertex), &mVertices[0], GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(GLuint), &mIndices[0], GL_STATIC_DRAW);

	// Vertex Positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Normals attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// Vertex Texture Coords
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(8 * sizeof(GLfloat)));
	glEnableVertexAttribArray(3);

	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(11 * sizeof(GLfloat)));
	glEnableVertexAttribArray(4);

	glVertexAttribIPointer(5, 1, GL_UNSIGNED_INT, sizeof(Vertex), (GLvoid*)(14 * sizeof(GLfloat)));
	glEnableVertexAttribArray(5);
	// unbind to make sure other code does not change it somewhere else
	glBindVertexArray(0);
}

glm::vec3 StatModel::computeCentroid(const Component& comp, const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices)
{
	glm::vec3 sum(0.0f);
	float totalArea = 0.0f;

	for (uint32_t t : comp.triangles)
	{
		glm::vec3 a = vertices[indices[t * 3 + 0]].position;
		glm::vec3 b = vertices[indices[t * 3 + 1]].position;
		glm::vec3 c = vertices[indices[t * 3 + 2]].position;

		glm::vec3 centroid = (a + b + c) / 3.0f;
		float area = glm::length(glm::cross(b - a, c - a)) * 0.5f;

		sum += centroid * area;
		totalArea += area;
	}

	return (totalArea > 0.0f) ? sum / totalArea : glm::vec3(0.0f);
}


