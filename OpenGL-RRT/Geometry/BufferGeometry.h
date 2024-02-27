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

// ����ṹ��
struct vertex
{
    // ����λ��
    glm::vec2 position;
    // ������ɫ
    glm::vec3 pixcolor;
};

class BufferGeometry
{
private:
    /* data */
public:
    unsigned int VAO;
    // ������������������
    GLint num_vert, num_indic;
    // ��ɫ���任���󣨰����任�Լ��Ӵ�����ͬ����
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
    // ����VAO�Լ�����
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    // ��GPU���䶥�㻺��
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //glBufferData(GL_ARRAY_BUFFER, (num_vert) * sizeof(vertex), &vertices[0], GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, (num_vert) * sizeof(vertex), vertices, GL_STATIC_DRAW);
    // ��GPU������������
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (num_indic) * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    // ���ö�������ָ��
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)0);
    // ������ɫ����ָ��
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)(2 * sizeof(GLfloat)));

    // ȡ����
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


