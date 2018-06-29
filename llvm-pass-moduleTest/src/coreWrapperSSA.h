/*
 * coreWrapperSSA.h
 *
 *  Created on: 12-Jun-2018
 *      Author: amit
 */

#ifndef SRC_COREWRAPPERSSA_H_
#define SRC_COREWRAPPERSSA_H_

#include <list>
#include <string>
#include <iostream>
#include <fstream>

#include "DataStructures.h"
#include "IRssa.h"
#include <llvm/IR/Instruction.h>
#include <llvm/IR/Instructions.h>
#include "allStackVariables.h"

/*
 * Print SSA onto an output file
 */
void printSSA_toFile(std::ofstream& outFile, IRssa::ptr& ir_ssa);



/*
 * Main wrapper function that makes recursive calls to function within function calls (if any).
 */
void convertFunctionToSSA(funcDump& funcData, IRssa::ptr& ir_ssa);


void parseInstruction(llvm::Instruction &instruction, allStackVariables::ptr& workingVariable, IRssa::ptr& ir_ssa);





#endif /* SRC_COREWRAPPERSSA_H_ */
