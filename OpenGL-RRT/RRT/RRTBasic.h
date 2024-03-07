#pragma once
#include"RRTSearchStrategy.h"

class RRTBasic:public RRTSearchStrategy
{
public:
	RRTBasic(Config& rrtconfig);
	virtual Node* AddOneNodeToTreeAtPosition(glm::vec2 pos);
	virtual Node* AddOneNodeToTree();
	~RRTBasic();

private:

};

RRTBasic::RRTBasic(Config& rrtconfig):RRTSearchStrategy(rrtconfig)
{
	
}
Node* RRTBasic::AddOneNodeToTreeAtPosition(glm::vec2 pos) {
	Node* closestnode = this->_tree->GetClosestNode(pos);
	glm::vec2 direction = pos - closestnode->m_position;
	Node* newnode = new Node(closestnode->m_position+ (this->m_rrtconfig->m_maxBranchLength > glm::length(direction) ? glm::length(direction) : this->m_rrtconfig->m_maxBranchLength) *glm::normalize(direction));
	if (this->IsColliding(closestnode->m_position, newnode->m_position, this->m_obstacle)) {
		delete	newnode;
		return nullptr;
	}
	this->_tree->AddChildNodeToParentNode(closestnode, newnode);
	if (this->IsCollidingWithTarget(newnode->m_position)) {
		this->_tree->targetNode = newnode;
		this->_tree->foundPath = true;
		std::cout << "Â·¾¶ÒÑÕÒµ½" << std::endl;
	}
	return newnode;

}
Node* RRTBasic::AddOneNodeToTree() {
	return AddOneNodeToTreeAtPosition(this->getRandomPosition());
}


RRTBasic::~RRTBasic()
{

}
