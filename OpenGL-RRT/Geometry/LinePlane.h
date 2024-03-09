#ifndef _LINEPLANE_H_
#define _LINEPLANE_H_

#include"BufferGeometry.h"
// 线形结构体
struct Line
{
	glm::vec2 m_start ;
	glm::vec2 m_end ;
	glm::vec2 m_ratio ;
	glm::vec3 m_color ;
	GLint m_width ;
	

	Line() {
		m_start = glm::vec2(0, 0);
		m_end = glm::vec2(0, 0);
		m_ratio = glm::vec2(1.0f, 1.0f);
		m_color = glm::vec3(0, 1, 0);
		m_width = 3;
	}
};
class LinePlane : public BufferGeometry
{
public:
	LinePlane(const Line& line);
	~LinePlane();
	virtual void draw() const;

private:
	// 直线顶点
	vertex vertices[2];
};

LinePlane::LinePlane(const Line& line)
{
	this->num_vert = 2;
	this->num_indic = 2;
	vertices[0].pixcolor = line.m_color;
	vertices[0].position = line.m_start;
	vertices[1].pixcolor = line.m_color;
	vertices[1].position = line.m_end;
	// 计算变换矩阵
	glm::mat4 transform = glm::mat4(1.0f);
	transform = glm::scale(transform, glm::vec3(line.m_ratio, 1.0f));
	//transform = glm::rotate(transform, glm::radians(angle), glm::vec3(0.0, 0.0, 1.0));
	//transform = glm::translate(transform, glm::vec3(center, 0.0f));
	
	// 变换矩阵传递父类保存
	this->trans = transform;

	this->SetupBuffers(vertices, nullptr);
}

LinePlane::~LinePlane()
{

}

void LinePlane::draw() const {
	glBindVertexArray(this->VAO);
	glDrawArrays(GL_LINES, 0, 2);
}

#endif
