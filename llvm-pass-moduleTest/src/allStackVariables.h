/*
 * allStackVariables.h
 *
 *  Created on: 13-Jun-2018
 *      Author: amit
 */

#ifndef SRC_ALLSTACKVARIABLES_H_
#define SRC_ALLSTACKVARIABLES_H_


#include <llvm/IR/Instruction.h>
#include <llvm/IR/Instructions.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/GlobalVariable.h>
#include <llvm/IR/Constants.h>
#include <boost/shared_ptr.hpp>
#include <boost/algorithm/string.hpp> //string comparison

#include "DataStructures.h"
#include "utilities.h"


class allStackVariables{

	//working variable: used to maintain the stack data structure for processing phi-node.
	std::list<phiData> cond_stack;	//std::list<std::pair<std::string, std::string>> cond_stack;
	/*
	 * If the cond_stack contains compound conditions in any of its element in the list
	 * then before merging the elements into a single compound condition it is pushed into allCompoundConditions.
	 */
	std::list<phiData> allCompoundConditions;
	//working variable: to keep track of the current condition/label for the phi-node(s) of the block
	struct phiData cond_curr_block;


	std::string previousBlockName;//working variable: to keep track of pop operation for phi-node
	std::string currentBlockName;//working variable: to keep track of pop operation for phi-node
	std::string blockName_at_phiNode;//working variable: to keep track of the first phi-node found within a block

	bool flag_phiBlock;//working variable: to keep track of if we entered into a specific phi-node block

	/*
	 * working stack structure to keep track of the blockName encountered in the br instructions
	 * The only difference here is search and delete, since there occurrence is not in order
	 * Added the ssaLineNo also into the same stack
	 */
	std::list<std::pair<unsigned int, std::string> > TrueFalseBlockStack;

	//working stack structure to keep track of the br instructions encountered in blocks. May be useful later
	std::list<brData> brStack;

	/*
	 * Data used to be tested if the previous block had a conditional br instruction
	 * Returns: True if it was a conditional, br instruction
	 * Returns: False if it was not a conditional, br instruction, i.e., the br instruction had a single label
	 */
	bool isPreviousBlockCondition;

	std::string previous_br_label;	//store the label of the br instruction if its a single label

	std::list<blockOrdering> blockSequenceStack; //block sequence stored as stack

public:
	typedef boost::shared_ptr<allStackVariables> ptr;

	//allStackVariables allStackVariables();

	std::list<phiData> getCondStack();

	struct phiData getTop(); //get the item from the top of the stack data structure
	void pushTop(struct phiData cond); //push at the top of the stack data structure

	void setAllCompoundConditions(phiData phiNode);

	std::string& getCurrentBlockName();
	void setCurrentBlockName(std::string& currentBlockName);
	std::string& getPreviousBlockName();
	void setPreviousBlockName(std::string& previousBlockName);

	void setPrevious_br_label(std::string label);
	std::string getPrevious_br_label();

	std::string& getBlockName_at_phiNode();
	void setBlockName_at_phiNode(std::string& blockName_at_phiNode);


	bool isFlagPhiBlock() const;
	void setFlagPhiBlock(bool flagPhiBlock);

	void setCond_curr_block(struct phiData cond_curr_block);
	struct phiData getCond_curr_block();

	//Returns the element from the top of the stack of "TrueFalseBlockStack"
	std::pair<unsigned int, std::string>  getTopTrueFalseBlock();
	std::list<std::pair<unsigned int, std::string> > getTrueFalseBlockStack();
	void setTrueFalseBlockStack(std::pair<unsigned int, std::string> TFBlock);

	std::list<brData> getBrStack();
	void setBrStack(brData brInfo);

	//Delete an item from the list "TrueFalseBlockStack" given its position. If not found return false otherwise true
	bool deleteTrueFalseBlockStack(std::list<std::pair<unsigned int, std::string> >::iterator it);

	bool deleteAllTrueFalseBlockStack(std::pair<unsigned int, std::string> bkName);

	/*
	 * Data used to be tested if the previous block had a conditional br instruction
	 * Returns: True if it was a conditional, br instruction
	 * Returns: False if it was not a conditional, br instruction, i.e., the br instruction had a single label
	 */
	bool isPreviousBlockConditional();
	void setPreviousBlockCondition(bool brCondition);

	void setBlockSequence(struct blockOrdering blockSequenceStack);
	std::list<blockOrdering> getBlockSequence();

};


#endif /* SRC_ALLSTACKVARIABLES_H_ */
