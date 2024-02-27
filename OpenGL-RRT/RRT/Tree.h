#pragma once

#include "Node.h"

class Tree : public Node
{
public:
	Tree(glm::vec2 pos);
	void AddChildNodeToParentNode(Node* parentNode, Node* childNode);
	Node* GetClosestNode(glm::vec2 pos);
	bool foundPath;
	Node* targetNode;
	Node* RootNode;
	~Tree();

private:
	
};

Tree::Tree(glm::vec2 pos)
{
	RootNode = new Node(pos);
	foundPath = false;
	targetNode = nullptr;
}
Node* Tree::GetClosestNode(glm::vec2 pos) {
	return RootNode->GetClosestNodeInChildren(pos);
}
void Tree::AddChildNodeToParentNode(Node* parentNode, Node* childNode) {
	parentNode->AddChild(childNode);
}

Tree::~Tree()
{
	delete RootNode;
}

