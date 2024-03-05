#pragma once
#include <iostream>
#include <random>
#include <algorithm>
#include "Config.h"
class RRTSearchStrategy
{
public:
	RRTSearchStrategy(Config* rrtconfig);
	virtual Node* AddOneNodeToTreeAtPosition(glm::vec2 pos)=0;
	virtual Node* AddOneNodeToTree() = 0;
	bool IsColliding(glm::vec2 begin, glm::vec2 end, std::vector<RectanglePlane::Rectangle>* obstacle);
	glm::vec2 getRandomPosition();
	Tree* _tree;
	~RRTSearchStrategy();

private:

protected:
	Config* m_rrtconfig;
	glm::vec2 m_target;
	std::vector<RectanglePlane::Rectangle>* m_obstacle;
	bool IsCollidingWithObstacle(glm::vec2 begin, glm::vec2 end, RectanglePlane::Rectangle obstacle);
	bool IsCollidingWithTarget(glm::vec2 end);
};

RRTSearchStrategy::RRTSearchStrategy(Config* rrtconfig)
{
	m_rrtconfig = rrtconfig;
	_tree = rrtconfig->m_tree;
	m_target = rrtconfig->gotarget;
	m_obstacle = rrtconfig->m_obstacle;
}
glm::vec2 RRTSearchStrategy::getRandomPosition() {
	// 设置随机数生成器
	std::random_device rd;
	std::mt19937 gen(rd());
	// 定义随机数分布范围
	std::uniform_real_distribution<float> distribution(-500.00, 500.00);
	// 生成随机数
	float random_X = distribution(gen);
	float random_Y = distribution(gen);
	return glm::vec2(random_X, random_Y);
}
bool RRTSearchStrategy::IsColliding(glm::vec2 begin, glm::vec2 end, std::vector<RectanglePlane::Rectangle>* obstacle) {
	for (RectanglePlane::Rectangle i : *obstacle) {
		if (IsCollidingWithObstacle(begin, end, i)) return true;
	}
	return false;
}
bool RRTSearchStrategy::IsCollidingWithObstacle(glm::vec2 begin, glm::vec2 end, RectanglePlane::Rectangle obstacle) {
	// trans1控制矩形障碍物，trans2控制线形
	glm::mat4 trans1 = glm::mat4(1.0f);
	glm::mat4 trans2 = trans1;
	// 自带旋转角度的矩形障碍物反向旋转修正
	trans1 = glm::rotate(trans1, glm::radians(-obstacle.m_angle), glm::vec3(0.0f, 0.0f, 1.0f));
	trans1 = glm::translate(trans1, glm::vec3(obstacle.m_center.x, obstacle.m_center.y, 0.0f));
	trans1 = glm::rotate(trans1, glm::radians(obstacle.m_angle), glm::vec3(0.0f, 0.0f, 1.0f));
	// 线形反向旋转一次
	trans2 = glm::rotate(trans2, glm::radians(-obstacle.m_angle), glm::vec3(0.0f, 0.0f, 1.0f));
	// 原始矩形障碍物顶点坐标
	glm::vec4 right_up = glm::vec4(obstacle.m_width / 2, obstacle.m_height / 2,0,1);
	glm::vec4 left_down = glm::vec4(-obstacle.m_width / 2, -obstacle.m_height / 2,0,1);
	glm::vec4 line_vector_b = glm::vec4(begin.x, begin.y, 0, 1);
	glm::vec4 line_vector_e = glm::vec4(end.x, end.y, 0, 1);
	// 旋转修正后的顶点坐标
	glm::vec4 right_up_new = trans1 * right_up;
	glm::vec4 left_down_new = trans1 * left_down;
	glm::vec4 line_vector_b_new = trans2 * line_vector_b;
	glm::vec4 line_vector_e_new = trans2 * line_vector_e;
	// 判断线形是否与障碍物重叠
	float obstacle_manX = right_up_new.x;
	float obstacle_minX = left_down_new.x;
	float obstacle_manY = right_up_new.y;
	float obstacle_minY = left_down_new.y;
	float line_manX = std::fmax(line_vector_b_new.x, line_vector_e_new.x);
	float line_minX = std::fmin(line_vector_b_new.x, line_vector_e_new.x);
	float line_manY = std::fmax(line_vector_b_new.y, line_vector_e_new.y);
	float line_minY = std::fmin(line_vector_b_new.y, line_vector_e_new.y);
	bool one = (line_manX > obstacle_minX) & (line_minX < obstacle_manX);
	bool two = (line_manY > obstacle_minY) & (line_minY < obstacle_manY);
	if (one && two) return true;
	else
	{
		return false;
	}
}
bool RRTSearchStrategy::IsCollidingWithTarget(glm::vec2 end) {
	if (std::abs(end.x - this->m_target.x) + std::abs(end.y - this->m_target.y) < this->m_rrtconfig->m_threshold) return true;
	else return false;
}
RRTSearchStrategy::~RRTSearchStrategy()
{
}