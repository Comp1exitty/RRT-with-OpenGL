#ifndef _RECTANGLEPLANE_H_
#define _RECTANGLEPLANE_H_

#include"BufferGeometry.h"

namespace RectanglePlane {
	// 矩形结构体
	struct Rectangle
	{
		glm::vec2 m_center;
		GLfloat m_width;
		GLfloat m_height;
		glm::vec2 m_ratio;
		GLfloat m_angle;
		glm::vec3 m_color;

		Rectangle() {
			m_center = glm::vec2(0, 0);
			m_width = 50.0f;
			m_height = 50.0f;
			m_ratio = glm::vec2(1.0f, 1.0f);
			m_angle = 0.0f;
			m_color = glm::vec3(0, 1, 0);
		}
	};
	class RectanglePlane : public BufferGeometry
	{
	private:
		// 矩形顶点与顶点索引
		vertex vertices[4];
		unsigned int indices[6] = { 0,1,2,0,2,3 };
	public:
		RectanglePlane(const Rectangle& geo_rectangle);
		~RectanglePlane();

	protected:

	};

	RectanglePlane::RectanglePlane(const Rectangle& geo_rectangle)
	{
		this->num_vert = 4;
		this->num_indic = 6;

		GLfloat half_wid = geo_rectangle.m_width / 2;
		GLfloat half_hei = geo_rectangle.m_height / 2;

		vertices[0].pixcolor = geo_rectangle.m_color;
		vertices[0].position = glm::vec2(half_wid, half_hei);
		vertices[1].pixcolor = geo_rectangle.m_color;
		vertices[1].position = glm::vec2(half_wid, -half_hei);
		vertices[2].pixcolor = geo_rectangle.m_color;
		vertices[2].position = glm::vec2(-half_wid, -half_hei);
		vertices[3].pixcolor = geo_rectangle.m_color;
		vertices[3].position = glm::vec2(-half_wid, half_hei);

		// 计算变换矩阵
		glm::mat4 transform = glm::mat4(1.0f);
		transform = glm::scale(transform, glm::vec3(geo_rectangle.m_ratio, 1.0f));
		transform = glm::translate(transform, glm::vec3(geo_rectangle.m_center, 0.0f));
		transform = glm::rotate(transform, glm::radians(geo_rectangle.m_angle), glm::vec3(0.0, 0.0, 1.0));

		// 变换矩阵传递父类保存
		this->trans = transform;

		this->SetupBuffers(vertices, indices);
	}

	RectanglePlane::~RectanglePlane()
	{
		
	}

}
#endif
