#pragma once

#include <string>
#include <vector>
#include <iostream>

#include "..\Geometry\RectanglePlane.h"

#include <glm/glm.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Node
{
public:
	Node(glm::vec2 position);
	Node();
	std::vector<Node*> all_children;
	Node* parent;
	void AddChild(Node* child);
	Node* GetClosestNodeInChildren(const glm::vec2& target);
	~Node();
	glm::vec2 m_position;
private:
	
protected:
	
};

Node::Node(glm::vec2 position)
{
	m_position = position;
	parent = nullptr;
}
Node::Node()
{
	m_position = glm::vec2(0, 0);
	parent = nullptr;
}
void Node::AddChild(Node* child)
{
	child->parent=this;
	this->all_children.push_back(child);
}
Node* Node::GetClosestNodeInChildren(const glm::vec2& target) {
	float min_dis = std::abs(this->m_position.x - target.x) + std::abs(this->m_position.y - target.y);
	Node* minNode=this;
	if (all_children.size() > 0) {
		for (Node* i : all_children) {
			Node* tmp_node = i->GetClosestNodeInChildren(target);
			float dis_tmp = std::abs(tmp_node->m_position.x- target.x) + std::abs(tmp_node->m_position.y - target.y);
			if (dis_tmp < min_dis) {
				min_dis = dis_tmp;
				minNode = tmp_node;
			}
		}
	}
	return minNode;
}

Node::~Node()
{

}
