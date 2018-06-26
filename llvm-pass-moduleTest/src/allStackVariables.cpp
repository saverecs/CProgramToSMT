/*
 * allStackVariables.cpp
 *
 *  Created on: 13-Jun-2018
 *      Author: amit
 */


#include "allStackVariables.h"

//allStackVariables allStackVariables::allStackVariables(){ }

struct phiData allStackVariables::getTop() {
	std::list<struct phiData>::iterator it = cond_stack.begin();
	struct phiData top_condition;
	top_condition = (*it); //get item from the top or beginning of the list.
	cond_stack.pop_front();	//delete from the front or top of the list.
	return top_condition;
}

void allStackVariables::pushTop(struct phiData cond) {
	cond_stack.push_front(cond);
}

void allStackVariables::setAllCompoundConditions(phiData phiNode){
	allCompoundConditions.push_front(phiNode);
}

std::list<phiData> allStackVariables::getCondStack(){
	return cond_stack;
}

std::string& allStackVariables::getCurrentBlockName() {
	return currentBlockName;
}
void allStackVariables::setCurrentBlockName(std::string& currentBlockName) {
	this->currentBlockName = currentBlockName;
}
std::string& allStackVariables::getPreviousBlockName() {
	return previousBlockName;
}
void allStackVariables::setPreviousBlockName(std::string& previousBlockName) {
	this->previousBlockName = previousBlockName;
}

std::string& allStackVariables::getBlockName_at_phiNode() {
	return blockName_at_phiNode;
}
void allStackVariables::setBlockName_at_phiNode(std::string& blockName_at_phiNode) {
	this->blockName_at_phiNode = blockName_at_phiNode;
}

bool allStackVariables::isFlagPhiBlock() const {
	return flag_phiBlock;
}

void allStackVariables::setFlagPhiBlock(bool flagPhiBlock) {
	flag_phiBlock = flagPhiBlock;
}

void allStackVariables::setCond_curr_block(struct phiData cond_curr_block) {
	this->cond_curr_block = cond_curr_block;
}
struct phiData allStackVariables::getCond_curr_block() {
	return cond_curr_block;
}



std::pair<unsigned int, std::string> allStackVariables::getTopTrueFalseBlock() {
	std::list<std::pair<unsigned int, std::string> >::iterator it = TrueFalseBlockStack.begin();
	std::pair<unsigned int, std::string> top_block;
	top_block = (*it); //get item from the top of the list.
	TrueFalseBlockStack.pop_front();	//delete from the front or top of the list.
	return top_block;
}

std::list<std::pair<unsigned int, std::string> > allStackVariables::getTrueFalseBlockStack(){
	return TrueFalseBlockStack;
}

void allStackVariables::setTrueFalseBlockStack(std::pair<unsigned int, std::string> TFBlock){
	TrueFalseBlockStack.push_front(TFBlock);
}


std::list<brData> allStackVariables::getBrStack(){
	return brStack;
}
void allStackVariables::setBrStack(brData brInfo){
	brStack.push_front(brInfo);
}


bool allStackVariables::deleteTrueFalseBlockStack(std::list<std::pair<unsigned int, std::string> >::iterator it){
	if (!(TrueFalseBlockStack.empty()))	//if list is not empty
		TrueFalseBlockStack.erase(it);
	else
		return false;
	return true;
}

bool allStackVariables::deleteAllTrueFalseBlockStack(std::pair<unsigned int, std::string> bkName){
	//TrueFalseBlockStack.remove(bkName);
	if (!(TrueFalseBlockStack.empty()))	//if list is not empty
		TrueFalseBlockStack.remove(bkName);
	else
		return false;
	return true;
}

bool allStackVariables::isPreviousBlockConditional(){
	return isPreviousBlockCondition;
}
void allStackVariables::setPreviousBlockCondition(bool brCondition){
	isPreviousBlockCondition = brCondition;
}

void allStackVariables::setPrevious_br_label(std::string label){
	previous_br_label = label;
}
std::string allStackVariables::getPrevious_br_label(){
	return previous_br_label;
}

void allStackVariables::setBlockSequence(struct blockOrdering blockSequenceStack){
	this->blockSequenceStack.push_front(blockSequenceStack);
}

std::list<blockOrdering> allStackVariables::getBlockSequence(){
	return blockSequenceStack;
}
