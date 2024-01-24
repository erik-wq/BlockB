#include "Components/Mesh.h"
#include "Components/Shader.h"
#include "App/ResourceManager.h"
#if defined(__arm__) || defined(__aarch64__)
#include <GLES3/gl3.h>
#else
#include "glad/glad.h"
#endif

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;
}

void Mesh::Draw(Shader* shader)
{
    BindTextures(shader);

    // draw mesh
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    UnBindTextures();
}

void Mesh::PushToGPU(ResourceManager* resources)
{
	SetupMesh();
    for (unsigned int i = 0; i < textures.size(); i++)
    {
        textures[i].id = resources->GetTexture(textures[i].path).id;
	}   
}

void Mesh::BindTextures(Shader* shader)
{
    GLuint diffuseNr = 1;
    GLuint specularNr = 1;
    GLuint normalNr = 1;
    GLuint heightNr = 1;
    for (unsigned int i = 0; i < textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
        // retrieve texture number (the N in diffuse_textureN)
        std::string number;
        std::string name = textures[i].type;
        if (name == "texture_diffuse")
        {
            number = std::to_string(diffuseNr++);
            shader->setInt(("material.specular" + number), i);
        }
        else if (name == "texture_specular")
        {
            number = std::to_string(specularNr++); // transfer unsigned int to string
            shader->setInt("material.specular", i);
        }
        else if (name == "texture_normal")
        {
            number = std::to_string(normalNr++); // transfer unsigned int to string
            shader->setInt("material.normal", i);
        }
        else if (name == "texture_emissive")
        {
            number = std::to_string(normalNr++); // transfer unsigned int to string
            shader->setInt("material.emissive", i);
        }
        else if (name == "texture_normal")
        {
            number = std::to_string(heightNr++); // transfer unsigned int to string
            shader->setInt("material.normal", i);
        }

        // and finally bind the texture
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }
}

void Mesh::UnBindTextures()
{
    for (unsigned int i = 0; i < textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    // always good practice to set everything back to defaults once configured.
    glActiveTexture(GL_TEXTURE0);
}

void Mesh::DrawInstanced(unsigned int size)
{
    glDrawElementsInstanced(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT,0, size);
    glBindVertexArray(0);
}

void Mesh::BindVAO()
{
    glBindVertexArray(VAO);
}

std::vector<glm::vec3> Mesh::GetVertices()
{
    std::vector<glm::vec3> positions;
    for (auto& vertex : vertices)
    {
        positions.push_back(vertex.Position);
	}
    return positions;
}

void Mesh::SetupMesh()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
		&vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() *
		sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
	// vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		(void*)0);
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		(void*)offsetof(Vertex, Normal));
	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
	 	(void*)offsetof(Vertex, TexCoords));
   
	glBindVertexArray(0);
}
