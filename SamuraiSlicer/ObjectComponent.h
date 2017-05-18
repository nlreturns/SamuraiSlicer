#pragma once
#include "DrawComponent.h"
#include <string>
#include <vector>
#include <list>

#include <windows.h>
#include <gl/GL.h>
#include "Vec.h"
#include "stb_image.h"
class Texture
{
	GLuint textureId;
public:
	Texture(std::string filename) {
		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_2D, textureId);

		int width, height, depth;
		unsigned char* data = stbi_load("texture.png", &width, &height, &depth, 4);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		stbi_image_free(data);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}

	void bind() {
		glBindTexture(GL_TEXTURE_2D, textureId);
	}
};


class ObjectComponent : public DrawComponent
{
private:
	class Vertex
	{
	public:
		int position;
		int normal;
		int texcoord;
	};

	class Face
	{
	public:
		std::list<Vertex> vertices;
	};
	class MaterialInfo
	{
	public:
		MaterialInfo();
		std::string name;
		Texture* texture;
	};

	class ObjGroup
	{
	public:
		std::string name;
		int materialIndex;
		std::list<Face> faces;
	};


	std::vector<Vec3f>	vertices;
	std::vector<Vec3f>	normals;
	std::vector<Vec2f>	texcoords;
	std::vector<ObjGroup*> groups;
	std::vector<MaterialInfo*> materials;


	void loadMaterialFile(const std::string &fileName, const std::string &dirName);
public:
	ObjectComponent(const std::string &filename);
	~ObjectComponent(void);

	void draw();
};
