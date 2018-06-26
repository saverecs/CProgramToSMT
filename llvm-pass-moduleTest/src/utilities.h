/*
 * utilities.h
 *
 *  Created on: 22-May-2018
 *      Author: amit
 */

#ifndef SRC_UTIL_UTILITIES_H_
#define SRC_UTIL_UTILITIES_H_


#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include <llvm/IR/BasicBlock.h>
#include "llvm/ADT/Statistic.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/Debug.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"

#include <iostream>
#include <fstream>
#include <string>
#include "utilities.h"

#include "llvm/IR/IntrinsicInst.h"
#include <llvm/IR/Instruction.h>
#include <llvm/IR/Instructions.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/GlobalVariable.h>
#include <llvm/IR/Constants.h>

#include <boost/algorithm/string.hpp> //string comparison
#include "DataStructures.h"
#include <list>

#include <boost/algorithm/string.hpp> //string comparison
#include <boost/tokenizer.hpp>
#include <boost/foreach.hpp>
#include <boost/config.hpp>
#include <boost/lexical_cast.hpp>

#include <boost/shared_ptr.hpp>

//#include "IRssa.h"   //Solve it using forward declaration
using namespace llvm;


/*
 * Takes an operand and returns its name or value depending its type
 * todo: Further type can also if returned
 */
std::string getVariable_or_Value(Value* op);

/*
 *
 */
std::string getOperator_Integer(llvm::CmpInst::Predicate oper);

std::string getOperator_Float(llvm::CmpInst::Predicate oper);
/*
 * Input: phiData structure
 * Returns: True if search incoming label is found in the phi stack data-structure. Otherwise returns False
 * Ouput: If found the conditionExp is returned as address
 */
bool searchBlock(std::list<struct phiData> phiStack, std::string  searchLabel, std::string& condExp);

bool searchPhiCondition(std::list<blockOrdering>& blockSequenceStack, std::list<struct brData>& brStack, std::string phiBlockName,
		std::string searchLabel, std::string& condName);
bool completeBrLabelSearch(std::list<struct brData>& brStack, std::string searchBlockName, std::string searchLabel, std::string& condName, bool &foundIn);
bool searchBrLabel(std::list<struct brData>& brStack, std::string searchLabel, std::string& condName, bool &foundIn);


#endif /* SRC_UTIL_UTILITIES_H_ */
