#ifndef _SPHEREPLANE_H_
#define _SPHEREPLANE_H_

#include"BufferGeometry.h"

// Բ�ηֶ���
static const GLint seg = 80;
// Բ�νṹ��
struct Sphere
{
	glm::vec2 m_center;
	GLfloat m_radius;
	glm::vec2 m_radio;
	glm::vec3 m_color;
	Sphere() {
		m_center = glm::vec2(0, 0);
		m_radius = 5.0f;
		m_radio = glm::vec2(1.0f, 1.0f);
		m_color = glm::vec3(0, 1, 0);
	}
};
class SpherePlane : public BufferGeometry
{
private:
	// Բ�ζ����붥������
	vertex* vertices = new vertex[seg + 1];
	unsigned int* indices = new unsigned int[seg * 3];
	/* data */
public:
	// Բ�λ���
	//SpherePlane(glm::vec2 center = glm::vec2(0, 0), GLfloat radius = 5.0f, glm::vec2 ratio = glm::vec2(1.0f, 1.0f),
	//	glm::vec3 color = glm::vec3(0, 1, 0));
	SpherePlane(const Sphere& geo_sphere);
	~SpherePlane();

};
SpherePlane::SpherePlane(const Sphere& geo_sphere)
{
	// ����Բ�ηֶ���ȷ��������������������
	this->num_vert = seg + 1;
	this->num_indic = 3 * seg;

	vertex vertex_tmp;
	vertex_tmp.position = glm::vec2(0.0f);
	vertex_tmp.pixcolor = geo_sphere.m_color;
	this->vertices[0] = vertex_tmp;
	// �����ֶνǶ�
	GLfloat singal_seg = 2 * PI / seg;
	// �����������
	for (int i = 0; i < seg; ++i)
	{
		vertex_tmp.position.x = geo_sphere.m_radius * glm::sin(singal_seg * i);
		vertex_tmp.position.y = geo_sphere.m_radius * glm::cos(singal_seg * i);
		vertex_tmp.pixcolor = geo_sphere.m_color;
		this->vertices[i + 1] = vertex_tmp;
	}
	// ������������
	for (int i = 0; i < seg; ++i)
	{
		this->indices[i * 3] = 0;
		this->indices[i * 3 + 1] = i + 1;
		int tmp = (i + 2) > seg ? 1 : i + 2;
		this->indices[i * 3 + 2] = tmp;
	}
	// ����任����
	glm::mat4 transform = glm::mat4(1.0f);
	transform = glm::scale(transform, glm::vec3(geo_sphere.m_radio, 1.0f));
	/*transform = glm::rotate(transform, glm::radians(angle), glm::vec3(0.0, 0.0, 1.0));*/
	transform = glm::translate(transform, glm::vec3(geo_sphere.m_center, 0.0f));
	// �任���󴫵ݸ��ౣ��
	this->trans = transform;

	this->SetupBuffers(vertices, indices);
}

SpherePlane::~SpherePlane()
{
	//delete [] vertices;
	//delete [] indices;
}
#endif