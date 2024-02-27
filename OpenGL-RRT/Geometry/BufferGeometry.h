#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <vector>
#include <iostream>


const GLfloat PI = glm::pi<GLfloat>();

// 顶点结构体
struct vertex
{
    // 顶点位置
    glm::vec2 position;
    // 顶点颜色
    glm::vec3 pixcolor;
};

class BufferGeometry
{
private:
    /* data */
public:
    unsigned int VAO;
    // 顶点数量与索引数量
    GLint num_vert, num_indic;
    // 着色器变换矩阵（包括变换以及视窗比例同步）
    glm::mat4 trans;
    BufferGeometry(/* args */);
    ~BufferGeometry(); 
    void dispose();
    //virtual void draw();
    virtual void draw();

protected:
    unsigned int VBO, EBO;
    void SetupBuffers(const vertex* vertices, const unsigned int* indices);

};

BufferGeometry::BufferGeometry(/* args */)
{
}

BufferGeometry::~BufferGeometry()
{
}
void BufferGeometry::dispose()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void BufferGeometry::draw() {
    glBindVertexArray(this->VAO);
    glDrawElements(GL_TRIANGLES, this->num_indic, GL_UNSIGNED_INT, 0);
}
void BufferGeometry::SetupBuffers(const vertex* vertices, const unsigned int* indices)
{
    // 创建VAO以及缓冲
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    // 向GPU传输顶点缓冲
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //glBufferData(GL_ARRAY_BUFFER, (num_vert) * sizeof(vertex), &vertices[0], GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, (num_vert) * sizeof(vertex), vertices, GL_STATIC_DRAW);
    // 向GPU传输索引缓冲
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (num_indic) * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    // 设置顶点属性指针
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)0);
    // 设置颜色属性指针
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)(2 * sizeof(GLfloat)));

    // 取消绑定
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


