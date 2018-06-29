/*
 * IRssa.h
 *
 *  Created on: 11-May-2018
 *      Author: amit
 */

#ifndef IRssa_H
#define IRssa_H


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
#include "allStackVariables.h"

using namespace llvm;
using namespace std;


class IRssa {

	std::map<std::string, funcDump> functionDump;	//Whole Program as map of function with function-name

	//std::list<std::string> all_insts;//list of program instructions/strings in SSA form
	std::list<std::pair<unsigned int, std::string> > all_insts;//list of instructions/strings in SSA form along with line-no when created

	/*
	 * List of all intermediate variables with the pair<varName, varType> used in the SSA
	 * varName: is the first argument of the pair representing the name of the variable
	 * varType: is the second argument of the pair representing the data type of the variable
	 */
	std::list<std::pair<std::string, std::string> > ssaVariables;
	/*
	 * Original Program Variables:
	 * inputVariables: 	consists of plant variables that is sensed by the controller-program
	 *                 	and the intermediate variables used in the controller-program.
	 * outputVariables: consists of the controller (program) variables that is an input
	 * 					to the plant generated/assigned by the controller.
	 */
	std::list<struct variable> inputVariables;
	std::list<struct variable> outputVariables;

public:
	typedef boost::shared_ptr<IRssa> ptr;

	//void parseInstruction(llvm::Instruction &instruction, allStackVariables::ptr& workingVariable);
	std::list<std::pair<unsigned int, std::string> > getAllInsts();
	/*
	 * Search from reverse an string item in the list and Replace with the given string.
	 * Returns true if replacement is successful, otherwise returns false.
	 */
	bool replaceAllInsts(std::string searchSt, std::string replacingSt);

	void setInst(unsigned int lineNo, std::string st);
//The function getVariables() and setVariables() are interface for SSA variables
	std::list<std::pair<std::string, std::string> > getVariables();
	void setVariable(std::string varName, std::string VarType);

	std::list<variable> getInputVariables();
	void setInputVariable(std::string varName, unsigned int id, std::string VarType);

	std::list<variable> getOutputVariables();
	void setOutputVariable(std::string varName, unsigned int id, std::string VarType);

	/*
	 * Internal utility function for searching and replacing "ite" to "=>" for conditional br instructions
	 * having single True or False block.
	 * The ssaLineNo can be obtained from the input argument item.first and the string as item.second
	 * Returns: True if replacement is successful otherwise False.
	 */
	bool replacePrevious_ite_toImplication(std::pair<unsigned int, std::string> item);


	const std::map<std::string, funcDump>& getFunctionDump() const;
	void setFunctionDump(const std::map<std::string, funcDump>& functionDump);


	static unsigned int ssaLineNo;
};

//DenseMap<MDNode*, unsigned> _mdnMap; //Map for MDNodes.
//void getAllMDNFunc(Function& F);
//void createMetadataSlot(MDNode *N);


#endif //IRssa_H
