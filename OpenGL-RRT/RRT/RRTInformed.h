#pragma once
#include "RRTBasic.h"

class RRTInformed :public RRTBasic
{
public:
	RRTInformed(Config* rrtconfig);
	virtual Node* AddOneNodeToTree();
	~RRTInformed();

private:
	int count;
};

RRTInformed::RRTInformed(Config* rrtconfig):RRTBasic(rrtconfig)
{
}
Node* RRTInformed::AddOneNodeToTree() {
	if (count % 10 == 0) {
		count++;
		return AddOneNodeToTreeAtPosition(this->m_rrtconfig->gotarget);
	}
	else
	{
		count++;
		return AddOneNodeToTreeAtPosition(this->getRandomPosition());
	}
	
}
RRTInformed::~RRTInformed()
{
}
