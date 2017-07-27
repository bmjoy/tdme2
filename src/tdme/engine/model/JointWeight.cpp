// Generated from /tdme/src/tdme/engine/model/JointWeight.java
#include <tdme/engine/model/JointWeight.h>

#include <java/lang/String.h>
#include <java/lang/StringBuilder.h>

using tdme::engine::model::JointWeight;
using java::lang::String;
using java::lang::StringBuilder;

JointWeight::JointWeight()
{
	this->jointIndex = -1;
	this->weightIndex = -1;
}

JointWeight::JointWeight(int32_t jointIndex, int32_t weightIndex) 
{
	this->jointIndex = jointIndex;
	this->weightIndex = weightIndex;
}

int32_t JointWeight::getJointIndex()
{
	return jointIndex;
}

int32_t JointWeight::getWeightIndex()
{
	return weightIndex;
}
