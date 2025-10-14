#pragma once
#include "Object.h"
class TPentagon : public Object
{
private:
	vec3 setVertex[5];
	vec3 ExtraVertex[4];
	GLint ShapeType = 0; //0:오각형, 1:선, 2:삼각형, 3:사각형
	GLfloat size = 1.0f;

	GLvoid TransPentagon();
	GLvoid TransLine();
	GLvoid TransTriangle();
	GLvoid TransRectangle();
public:
	TPentagon(GLfloat x, GLfloat y, GLfloat size, GLint type, GLint mode);
	~TPentagon();

	GLvoid DoTransPentagon() { TransPentagon(); }
	GLvoid DoTransLine() { TransLine(); }
	GLvoid DoTransTriangle() { TransTriangle(); }
	GLvoid DoTransRectangle() { TransRectangle(); }
	GLvoid DoUpload() {
		GLfloat pentagonVertices[] = {
		vPos.x, vPos.y, vPos.z,         vColor.x, vColor.y, vColor.z,
		ExtraVertex[0].x, ExtraVertex[0].y, ExtraVertex[0].z,   vColor.x, vColor.y, vColor.z,
		ExtraVertex[3].x, ExtraVertex[3].y, ExtraVertex[3].z,   vColor.x, vColor.y, vColor.z,

		ExtraVertex[3].x, ExtraVertex[3].y, ExtraVertex[3].z,   vColor.x, vColor.y, vColor.z,
		ExtraVertex[0].x, ExtraVertex[0].y, ExtraVertex[0].z,   vColor.x, vColor.y, vColor.z,
		ExtraVertex[1].x, ExtraVertex[1].y, ExtraVertex[1].z,   vColor.x, vColor.y, vColor.z,

		ExtraVertex[3].x, ExtraVertex[3].y, ExtraVertex[3].z,   vColor.x, vColor.y, vColor.z,
		ExtraVertex[1].x, ExtraVertex[1].y, ExtraVertex[1].z,   vColor.x, vColor.y, vColor.z,
		ExtraVertex[2].x, ExtraVertex[2].y, ExtraVertex[2].z,   vColor.x, vColor.y, vColor.z
		};
		upload(pentagonVertices, sizeof(pentagonVertices));
	}

	virtual GLvoid draw() override;
	virtual GLvoid update() override;
};

