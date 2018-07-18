/*
 * optDataStructures.h
 *
 *  Created on: 16-Jul-2018
 *      Author: amit
 */

#ifndef SRC_OPTDATASTRUCTURES_H_
#define SRC_OPTDATASTRUCTURES_H_


#include <iostream>
#include <string>
#include <list>

using namespace std;

//Data Structure of definition of variables with there right-hand side expression. (SSA having = sign)
struct definitionDS{
	unsigned int id; //for now assume the order in which it appears in this structure
	unsigned int lineNo;	//line number in the unoptimized SSA, where this definition exist
	string varName;	//LHS of = sign
	string varType;	//if available
	string varRHSexpression;//The RHS value of the variable
};


#endif /* SRC_OPTDATASTRUCTURES_H_ */
