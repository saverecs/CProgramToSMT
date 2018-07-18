/*
 * optIRssa.h
 *
 *  Created on: 16-Jul-2018
 *      Author: amit
 */

#ifndef SRC_OPTIRSSA_H_
#define SRC_OPTIRSSA_H_

#include <fstream>
#include <string>
#include <iostream>
#include <list>

#include <boost/algorithm/string.hpp> //string comparison
#include <boost/tokenizer.hpp>
#include <boost/foreach.hpp>
#include <boost/config.hpp>
#include <boost/lexical_cast.hpp>

#include <boost/shared_ptr.hpp>

//#include "DataStructures.h"
#include "utilities.h"
#include "optDataStructures.h"

using namespace std;

class optIRssa{

//Data for before optimization process
	//std::list<std::string> all_insts;//list of program instructions/strings in SSA form
	std::list<std::pair<unsigned int, std::string> > all_insts;//list of instructions/strings in SSA form along with line-no when created
	/*
	 * List of all intermediate variables with the pair<varName, varType> used in the SSA
	 * varName: is the first argument of the pair representing the name of the variable
	 * varType: is the second argument of the pair representing the data type of the variable
	 */
	std::list<std::pair<std::string, std::string> > intermediateVariables;
	/*
	 * Original Program Variables:
	 * inputVariables: 	consists of plant variables that is sensed by the controller-program
	 *                 	and the intermediate variables used in the controller-program.
	 * outputVariables: consists of the controller (program) variables that is an input
	 * 					to the plant generated/assigned by the controller.
	 */
	std::list<struct variable> inputVariables;
	std::list<struct variable> outputVariables;


//Data for/after optimization process
	std::list<definitionDS> DDS;	//Definition Data Structure
	std::list<std::pair<unsigned int, std::string> > SSA;	//single static assignment (optimized after all processing is over).

public:
	typedef boost::shared_ptr<optIRssa> ptr;

	void optimizeInstCombine();
	void createDefinitionDataStructure(std::list< std::pair<unsigned int, std::string> > SSA_data);
	void optimizeProcess();
	/*
	 * Search the string mytoken in DDS if found return true, otherwise false
	 */
	bool isDefinitionExist(std::string mytoken, std::string& varRHSexpression, unsigned int& definition_lineNo);

	void modified_DDS_atLine(unsigned int foundAtLineNo, std::string varNameFound, std::string varRHSexpression);
	void delete_DDS_atLine(unsigned int foundAtLineNo);

	void delete_variable(std::string varName);

	const std::list<std::pair<unsigned int, std::string> >& getAllInsts() const;
	void setAllInsts(
			const std::list<std::pair<unsigned int, std::string> >& allInsts);
	const std::list<definitionDS>& getDds() const;
	void setDds(const std::list<definitionDS>& dds);
	const std::list<struct variable>& getInputVariables() const;
	void setInputVariables(const std::list<struct variable>& inputVariables);
	const std::list<std::pair<std::string, std::string> >& getIntermediateVariables() const;
	void setIntermediateVariables(
			const std::list<std::pair<std::string, std::string> >& intermediateVariables);
	const std::list<struct variable>& getOutputVariables() const;
	void setOutputVariables(const std::list<struct variable>& outputVariables);
	const std::list<std::pair<unsigned int, std::string> >& getSsa() const;
	void setSsa(const std::list<std::pair<unsigned int, std::string> >& ssa);
};


#endif /* SRC_OPTIRSSA_H_ */
