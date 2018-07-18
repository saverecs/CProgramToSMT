/*
 * utilities.cpp
 *
 *  Created on: 22-May-2018
 *      Author: amit
 */

#include "utilities.h"
#include <iostream>

using namespace llvm;
using namespace std;



std::string getVariable_or_Value(Value* op) {
	std::string st = "";

	if (Constant *c = dyn_cast<Constant>(op)) {
		if (c->getType()->isIntegerTy()) {
			int val = c->getUniqueInteger().getSExtValue();
			st = NumberToString(val);
		} else if (c->getType()->isDoubleTy()) {
			if (ConstantFP *Op = dyn_cast<ConstantFP>(c)) {
				double val = Op->getValueAPF().convertToDouble();
				st = NumberToString(val);
			}
		} else if (c->getType()->isFloatTy()) {
			if (ConstantFP *Op = dyn_cast<ConstantFP>(c)) {
				float val = Op->getValueAPF().convertToFloat();
				st = NumberToString(val);
			}
		}
	} else {
		//outs()<<"  Operand 0= "<< storeInst->getOperand(0)->getName();
		st = op->getName();
	}
	return st;
}

std::string getOperator_Integer(llvm::CmpInst::Predicate oper){
	std::string st;
	switch (oper) {//Note currently signed (S) and unsigned (U) are handled together
	case llvm::CmpInst::ICMP_EQ:
		st = "= ";	//single space after operation for pre-fix notation
		break;
	case llvm::CmpInst::ICMP_NE:
		st = "!= ";		//handle it at the receiving end: for converting to smt2 e.g., (not (= op1 op2))
		break;
	case llvm::CmpInst::ICMP_UGT:
		st = "> ";
		break;
	case llvm::CmpInst::ICMP_UGE:
		st = ">= ";
		break;
	case llvm::CmpInst::ICMP_ULT:
		st = "< ";
		break;
	case llvm::CmpInst::ICMP_ULE:
		st = "<= ";
		break;

	case llvm::CmpInst::ICMP_SGT:
		st = "> ";
		break;
	case llvm::CmpInst::ICMP_SGE:
		st = ">= ";
		break;
	case llvm::CmpInst::ICMP_SLT:
		st = "< ";
		break;
	case llvm::CmpInst::ICMP_SLE:
		st = "<= ";
		break;
	}
	return st;
}


std::string getOperator_Float(llvm::CmpInst::Predicate oper){
	std::string st;
	switch (oper) {//Note currently signed (S) and unsigned (U) are handled together
	case llvm::CmpInst::FCMP_OGT:
		st = "> ";	//single space after operation for pre-fix notation
		break;
	case llvm::CmpInst::FCMP_OGE:
		st = ">= ";
		break;
	case llvm::CmpInst::FCMP_OLT:
		st = "< ";
		break;
	case llvm::CmpInst::FCMP_OLE:
		st = "<= ";
		break;
	case llvm::CmpInst::FCMP_ONE:
		st = "!= ";		//handle it at the receiving end: for converting to smt2 e.g., (not (= op1 op2))
		break;
	case llvm::CmpInst::FCMP_ORD:
		st = "true ";	//todo :: need to understand how to handle
		break;
	case llvm::CmpInst::FCMP_UNO:
		st = "true ";	//todo :: need to understand how to handle
		break;
	case llvm::CmpInst::FCMP_UEQ:
		st = "= ";
		break;
	case llvm::CmpInst::FCMP_UGT:
		st = "> ";
		break;
	case llvm::CmpInst::FCMP_UGE:
		st = ">= ";
		break;
	case llvm::CmpInst::FCMP_ULT:
		st = "< ";
		break;
	case llvm::CmpInst::FCMP_ULE:
		st = "<= ";
		break;
	case llvm::CmpInst::FCMP_UNE:
		st = "!= ";		//handle it at the receiving end: for converting to smt2 e.g., (not (= op1 op2))
		break;
	case llvm::CmpInst::FCMP_TRUE:
		st = "true ";
		break;
	}
	return st;
}

bool searchBlock(std::list<struct phiData> phiStack, std::string  searchLabel, std::string& condExp){

	for (std::list<struct phiData>::iterator it= phiStack.begin();it!=phiStack.end();it++){
		if (boost::iequals((*it).blockLabel,searchLabel)){
			condExp = (*it).conditionExp;//The first instance that it find return the conditionExpression
			return true;
		}
	}
	return false;
}

/* ====== Main search is called from here ======
 * blockSequenceStack: is a stack containing the ordering of the Blocks
 * brStack: is a stack containing the conditional instructions with TrueLabel and FalseLabel
 * searchLabel: the label to be searched recursively/iteratively in all previous blocks
 *  If a conditional br is found in the brStack with the matching blockName as searchLabel, then the condName is returned,
 *  otherwise the search is made iterative from backwards(stack top) to find
 *
 * Outputs:
 * condName: should be returned either condName or (not condName) depending upon True or False incoming path
 * Returns: true if search found otherwise false
 */
bool searchPhiCondition(std::list<blockOrdering>& blockSequenceStack,
		std::list<struct brData>& brStack, std::string phiBlockName,
		std::string searchLabel, std::string& condName) {

	// ======= First search on the brData if found return the value =======
	bool foundIn=false, flag=false;
	flag = completeBrLabelSearch(brStack, searchLabel, phiBlockName, condName, foundIn);
	//std::cout<<"flag = "<<flag <<" condName = "<< condName<<std::endl;

	if (flag){	 //condName is already returned in condName if found
		if (!foundIn) { //found in the FalseLabel
			string st;
			st.append("(not ");
			st.append(condName);
			st.append(" )");
			condName = st;
		}
		return true;//Found in either TrueLabel or FalseLabel
	}else {	// ======= Next search iteratively(recursively back-track) using blockOrdering and brData =======
		std::list<struct blockOrdering>::iterator it = blockSequenceStack.begin();
		//it is obvious that it is not found in the block where phi-node exist
		it++;	//going to the next (previous due to stack) block
		for ( ; it != blockSequenceStack.end();it++) {
			string newSearchLabel = (*it).blockBlock; //previous blockName which may not have a conditional br instruction
			flag = searchBrLabel(brStack, newSearchLabel, condName, foundIn);
			if (flag) {	 //condName is already returned in condName if found in TrueLabel
				if (!foundIn) { //found in the FalseLabel
					string st;
					st.append("(not ");
					st.append(condName);
					st.append(" )");
					condName = st;
				}
				return true;//Found in either TrueLabel or FalseLabel
			}
		}//At the end of this the search is guaranteed to be found
	}
	return false;
}

/*Complete Search is done First on the blockName in which br is found and then searches on the True or False Label
 * brStack: is a stack containing the conditional instructions with TrueLabel and FalseLabel
 * searchLabel: the label to be searched
 *
 * Outputs:
 * condName: should be returned either condName or (not condName) depending upon True or False incoming path
 * foundIn: returns True if found in the TrueLabel otherwise False when found in the FalseLabel
 * Returns: true if search found otherwise false
 */
bool completeBrLabelSearch(std::list<struct brData>& brStack, std::string searchBlockName, std::string searchLabel, std::string& condName, bool &foundIn){

	for (std::list<struct brData>::iterator it = brStack.begin(); it != brStack.end(); it++) {
		if (boost::iequals((*it).br_inBlock, searchBlockName)) {
			if (boost::iequals((*it).trueLabel, searchLabel)) {
				condName = (*it).brConditionName;//The first instance that it find return the conditionExpression
				foundIn = true;
				return true;
			} else if (boost::iequals((*it).falseLabel, searchLabel)) {
				condName = (*it).brConditionName;//The first instance that it find return the conditionExpression
				foundIn = false;
				return true;
			}
		}
	}
	return false;
}

/*Search is done only on the True or False Label
 * brStack: is a stack containing the conditional instructions with TrueLabel and FalseLabel
 * searchLabel: the label to be searched
 *
 * Outputs:
 * condName: should be returned either condName or (not condName) depending upon True or False incoming path
 * foundIn: returns True if found in the TrueLabel otherwise False when found in the FalseLabel
 * Returns: true if search found otherwise false
 */
bool searchBrLabel(std::list<struct brData>& brStack, std::string searchLabel, std::string& condName, bool &foundIn){
	for (std::list<struct brData>::iterator it= brStack.begin();it!=brStack.end();it++){
		if (boost::iequals((*it).trueLabel, searchLabel)){
			condName = (*it).brConditionName;//The first instance that it find return the conditionExpression
			foundIn = true;
			return true;
		} else if (boost::iequals((*it).falseLabel, searchLabel)){
			condName = (*it).brConditionName;//The first instance that it find return the conditionExpression
			foundIn = false;
			return true;
		}
	}
	return false;
}


