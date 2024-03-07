#pragma once
#include "Tree.h"
class Config
{
public:
	Config(Tree& treeRRT,glm::vec2 target, float maxBranchLength,std::vector<RectanglePlane::Rectangle>& Obstacle,
		int maxCount,float threshold);
	Tree* m_tree;
	float m_maxBranchLength;
	float m_threshold;
	glm::vec2 gotarget;
	int m_maxcount;
	std::vector<RectanglePlane::Rectangle>* m_obstacle;
	~Config();

private:

};

Config::Config(Tree& treeRRT, glm::vec2 target, float maxBranchLength, std::vector<RectanglePlane::Rectangle>& Obstacle,
	int maxCount, float threshold)
{
	this->m_tree = &treeRRT;
	this->m_maxBranchLength = maxBranchLength;
	this->m_maxcount = maxCount;
	this->m_obstacle = &Obstacle;
	this->gotarget = target;
	this->m_threshold = threshold;
}

Config::~Config()
{
	
}
