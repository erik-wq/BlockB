#include "App/ModelLoader.h"
#include "Components/Model.h"
#include "App/ResourceManager.h"
#include "AssimpHelper.h"

#include <iostream>

#if defined(__arm__) || defined(__aarch64__)
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES3/gl3.h>
#else
#include "glad/glad.h"
#endif

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <Components/SkeletalMesh.h>


ModelLoader::ModelLoader(ResourceManager* resources)
{
	stbi_set_flip_vertically_on_load(true);
	resourceManager = resources;
}

Model* ModelLoader::LoadModel(std::string path)
{
	Assimp::Importer import;
	const aiScene * scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << "\n";
		return nullptr;
	}
	Model* model = new Model();
	bool skeletal = AssimpHelper::IsSkeletalModel(path);

	std::vector<Mesh*> meshes = ProcessNode(scene->mRootNode, scene, model, skeletal);
	model->SetMeshes(meshes);
	return model;
}

std::vector<Mesh*> ModelLoader::ProcessNode(aiNode* node, const aiScene* scene, Model* model,bool skeletal)
{
	std::vector<Mesh*> meshes;
	// process all the node’s meshes (if any)
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(ProcessMesh(mesh, scene, model, skeletal));
	}
	// then do the same for each of its children
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		std::vector<Mesh*> subMeshes = ProcessNode(node->mChildren[i], scene, model, skeletal);
		meshes.insert(meshes.end(), subMeshes.begin(), subMeshes.end());
	}
	return meshes;
}

Mesh* ModelLoader::ProcessMesh(aiMesh* mesh, const aiScene* scene, Model* model, bool skeletal)
{
	// data to fill
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	// walk through each of the mesh's vertices
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		SetVertexBoneDataToDefault(vertex);
		// positions
		vertex.Position = AssimpHelper::GetGLMVec3(mesh->mVertices[i]);
		// normals
		if (mesh->HasNormals())
		{
			vertex.Normal = AssimpHelper::GetGLMVec3(mesh->mNormals[i]);
		}

		// texture coordinates
		if (mesh->mTextureCoords[0])
		{
			vertex.TexCoords = AssimpHelper::GetGLMVec2(mesh->mTextureCoords[0][i]);
		}
		else
		{
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);
		}

		vertices.push_back(vertex);
	}

	// now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		// retrieve all indices of the face and store them in the indices vector
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
	// process materials
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

	// 1. diffuse maps
	std::vector<Texture> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	// 2. specular maps
	std::vector<Texture> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	// 3. normal maps
	std::vector<Texture> normalMaps = LoadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
	textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
	// 4. height maps
	std::vector<Texture> heightMaps = LoadMaterialTextures(material, aiTextureType_EMISSIVE, "texture_emissive");
	textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

	ExtractBoneWeightForVertices(vertices, mesh, scene, model);

	if (skeletal)
	{
		return new SkeletalMesh(vertices, indices, textures);
	}
	// return a mesh object created from the extracted mesh data
	return new Mesh(vertices, indices, textures);
}

void ModelLoader::SetVertexBoneDataToDefault(Vertex& vertex)
{
	for (int i = 0; i < MAX_BONE_INFLUENCE; i++)
	{
		vertex.boneIDs[i] = -1;
		vertex.weights[i] = 0.0f;
	}
}

unsigned int ModelLoader::LoadCubemap(std::vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	stbi_set_flip_vertically_on_load(false);
	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			stbi_image_free(data);
		}
		else
		{
			printf("Cubemap texture failed to load at path: %s", faces[i].c_str());
			stbi_image_free(data);
		}
	}
	stbi_set_flip_vertically_on_load(true);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return textureID;
}

std::vector<Texture> ModelLoader::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	std::vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		// check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
		Texture texture;
		if (!resourceManager->IsTextureLoaded(str.C_Str()))
		{
			resourceManager->LoadTexture(str.C_Str(), typeName);
		}
		texture = resourceManager->GetTexture(str.C_Str());
		textures.push_back(texture);
	}
	return textures;
}

unsigned char* ModelLoader::TextureFromFile(const std::string& path, Texture& text)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
	if (!data)
	{
		std::cout << "Texture failed to load at path: " << text.path << std::endl;
		stbi_image_free(data);
		return nullptr;
	}
	
	text.width = width;
	text.height = height;
	text.nrComponents = nrComponents;
	text.data = data;
	return data;
}

void ModelLoader::ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* , Model* model)
{
	auto& boneInfoMap = model->GetBoneInfoMap();
	int& boneCount = model->GetBoneCount();

	for (int boneIndex = 0; boneIndex < (int)mesh->mNumBones; ++boneIndex)
	{
		int boneID = -1;
		std::string boneName = mesh->mBones[boneIndex]->mName.C_Str();
		if (boneInfoMap.find(boneName) == boneInfoMap.end())
		{
			BoneInfo newBoneInfo;
			newBoneInfo.id = boneCount;
			newBoneInfo.offset = AssimpHelper::ConvertMatrixToGLM(mesh->mBones[boneIndex]->mOffsetMatrix);
			boneInfoMap[boneName] = newBoneInfo;
			boneID = boneCount;
			boneCount++;
		}
		else
		{
			boneID = boneInfoMap[boneName].id;
		}
		assert(boneID != -1);
		auto weights = mesh->mBones[boneIndex]->mWeights;
		int numWeights = mesh->mBones[boneIndex]->mNumWeights;

		for (int weightIndex = 0; weightIndex < numWeights; ++weightIndex)
		{
			int vertexId = weights[weightIndex].mVertexId;
			float weight = weights[weightIndex].mWeight;
			assert(vertexId <= static_cast<int>(vertices.size()));
			SetVertexBoneData(vertices[vertexId], boneID, weight);
		}
	}
}

void ModelLoader::SetVertexBoneData(Vertex& vertex, int boneID, float weight)
{
	for (int i = 0; i < MAX_BONE_INFLUENCE; ++i)
	{
		if (vertex.boneIDs[i] < 0)
		{
			vertex.weights[i] = weight;
			vertex.boneIDs[i] = boneID;
			break;
		}
	}
}

void ModelLoader::GenerateTexture(Texture& text)
{
	if (text.data)
	{
		unsigned int textureID;
		glGenTextures(1, &textureID);

		GLenum format = GL_RGBA;
		if (text.nrComponents == 1)
			format = GL_RED;
		else if (text.nrComponents == 3)
			format = GL_RGB;
		else if (text.nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, text.width, text.height, 0, format, GL_UNSIGNED_BYTE, text.data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(text.data);

		text.id = textureID;
	}
	text.data = nullptr;
}