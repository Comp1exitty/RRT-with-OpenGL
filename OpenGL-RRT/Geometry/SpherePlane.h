#ifndef _SPHEREPLANE_H_
#define _SPHEREPLANE_H_

#include"BufferGeometry.h"

// 圆形分段数
static const GLint seg = 80;
// 圆形结构体
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
	// 圆形顶点与顶点索引
	vertex* vertices = new vertex[seg + 1];
	unsigned int* indices = new unsigned int[seg * 3];
	/* data */
public:
	// 圆形绘制
	//SpherePlane(glm::vec2 center = glm::vec2(0, 0), GLfloat radius = 5.0f, glm::vec2 ratio = glm::vec2(1.0f, 1.0f),
	//	glm::vec3 color = glm::vec3(0, 1, 0));
	SpherePlane(const Sphere& geo_sphere);
	~SpherePlane();

};
SpherePlane::SpherePlane(const Sphere& geo_sphere)
{
	// 根据圆形分段数确定顶点数量与索引数量
	this->num_vert = seg + 1;
	this->num_indic = 3 * seg;

	vertex vertex_tmp;
	vertex_tmp.position = glm::vec2(0.0f);
	vertex_tmp.pixcolor = geo_sphere.m_color;
	this->vertices[0] = vertex_tmp;
	// 单个分段角度
	GLfloat singal_seg = 2 * PI / seg;
	// 顶点坐标计算
	for (int i = 0; i < seg; ++i)
	{
		vertex_tmp.position.x = geo_sphere.m_radius * glm::sin(singal_seg * i);
		vertex_tmp.position.y = geo_sphere.m_radius * glm::cos(singal_seg * i);
		vertex_tmp.pixcolor = geo_sphere.m_color;
		this->vertices[i + 1] = vertex_tmp;
	}
	// 顶点索引计算
	for (int i = 0; i < seg; ++i)
	{
		this->indices[i * 3] = 0;
		this->indices[i * 3 + 1] = i + 1;
		int tmp = (i + 2) > seg ? 1 : i + 2;
		this->indices[i * 3 + 2] = tmp;
	}
	// 计算变换矩阵
	glm::mat4 transform = glm::mat4(1.0f);
	transform = glm::scale(transform, glm::vec3(geo_sphere.m_radio, 1.0f));
	/*transform = glm::rotate(transform, glm::radians(angle), glm::vec3(0.0, 0.0, 1.0));*/
	transform = glm::translate(transform, glm::vec3(geo_sphere.m_center, 0.0f));
	// 变换矩阵传递父类保存
	this->trans = transform;

	this->SetupBuffers(vertices, indices);
}

SpherePlane::~SpherePlane()
{
	//delete [] vertices;
	//delete [] indices;
}
#endif