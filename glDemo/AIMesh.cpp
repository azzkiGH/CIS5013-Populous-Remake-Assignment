
#include "AIMesh.h"
#include "TextureLoader.h"

using namespace std;
using namespace glm;


AIMesh::AIMesh(std::string filename, GLuint meshIndex) {

	const struct aiScene* scene = aiImportFile(filename.c_str(),
		aiProcess_GenSmoothNormals |
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType);

	if (!scene) {

		return;
	}

	aiMesh* mesh = scene->mMeshes[meshIndex];


	glGenBuffers(1, &meshVertexPosBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, meshVertexPosBuffer);
	glBufferData(GL_ARRAY_BUFFER, mesh->mNumVertices * sizeof(aiVector3D), mesh->mVertices, GL_STATIC_DRAW);

	if (mesh->mTextureCoords && mesh->mTextureCoords[0]) {

		
		glGenBuffers(1, &meshTexCoordBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, meshTexCoordBuffer);
		glBufferData(GL_ARRAY_BUFFER, mesh->mNumVertices * sizeof(aiVector3D), mesh->mTextureCoords[0], GL_STATIC_DRAW);
	}
	
	

	numFaces = scene->mMeshes[meshIndex]->mNumFaces;

	
	const GLuint numBytes = scene->mMeshes[meshIndex]->mNumFaces * 3 * sizeof(GLuint);
	GLuint* faceIndexArray = (GLuint*)malloc(numBytes);

	GLuint* dstPtr = faceIndexArray;
	for (unsigned int f = 0; f < scene->mMeshes[meshIndex]->mNumFaces; ++f, dstPtr += 3) {

		memcpy_s(dstPtr, 3 * sizeof(GLuint), scene->mMeshes[meshIndex]->mFaces[f].mIndices, 3 * sizeof(GLuint));
	}

	glGenBuffers(1, &meshFaceIndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshFaceIndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numBytes, faceIndexArray, GL_STATIC_DRAW);

	
	aiReleaseImport(scene);
}




void AIMesh::addTexture(GLuint textureID) {

	this->textureID = textureID;
}

void AIMesh::addTexture(std::string filename, FREE_IMAGE_FORMAT format) {

	textureID = loadTexture(filename, format);
}


void AIMesh::preRender() {

	
	glBindBuffer(GL_ARRAY_BUFFER, meshVertexPosBuffer);
	glVertexPointer(3, GL_FLOAT, 0, (GLvoid*)0);
	glEnableClientState(GL_VERTEX_ARRAY);

	if (meshTexCoordBuffer != 0) {

		glBindBuffer(GL_ARRAY_BUFFER, meshTexCoordBuffer);
		glTexCoordPointer(3, GL_FLOAT, 0, (GLvoid*)0);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		if (textureID != 0) {
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, textureID);
		}
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshFaceIndexBuffer);
}


void AIMesh::render() {

	glDrawElements(GL_TRIANGLES, numFaces * 3, GL_UNSIGNED_INT, (const GLvoid*)0);
}


void AIMesh::postRender() {

	glDisableClientState(GL_VERTEX_ARRAY);

	if (meshTexCoordBuffer != 0) {
	
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);

		if (textureID != 0) {
			glDisable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}