/*
 * DataStructures.h
 *
 *  Created on: 13-May-2018
 *      Author: amit
 */

#ifndef DataStructures_H
#define DataStructures_H

#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Instruction.h>


#include <string>
#include <sstream>
#include <list>

using namespace llvm;


template <typename T>
  std::string NumberToString ( T Number )
  {
     std::ostringstream ss;
     ss << Number;
     return ss.str();
  }


/*
 * working variable: used to maintain the stack data structure for phi-node.
 * blockLabel holds the label/name
 * condName holds the variable/label name
 * conditionExp holds the conditional expression
 */
struct phiData{
	std::string blockLabel;
	std::string condName;
	std::string conditionExp;
};

//All information related to the branching instruction br in the IR
struct brData{
	std::string br_inBlock;		//Name of the block in which br is found
	std::string brConditionName;	//condition name on which br is executed
	std::string trueLabel;		//then-label
	std::string falseLabel;		//else-label
};

//information related to the ordering of blocks in the IR stored as stack. 1):entry block is first pushed in the stack
struct blockOrdering{
	unsigned int blockNumber;	//First block is assigned blockNumber 1 (the entry block)
	std::string blockBlock;		//Name of the block in the order of occurrence
};

struct variable{
	unsigned int id; //for now assume the order in which it appears in the declaration
	std::string varName;
	std::string varType;
	std::list< std::string > versionNames;//last version at the end of the list.
};


//list of pair<blockName, list-of-Instructions> each pair consists of (blockName, list of instructions of the blockName)
typedef std::list<std::pair<std::string, std::list<Instruction *> > > funcDump;


#endif //DataStructures_H
