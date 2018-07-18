#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Instruction.h>

#include "llvm/Support/raw_ostream.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/Debug.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"

#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include "DataStructures.h"
#include "utilities.h"
#include "coreWrapperSSA.h"
#include "IRssa.h"

using namespace llvm;

namespace {
struct TestPass: public ModulePass {
	IRssa::ptr ir_ssa = IRssa::ptr(new IRssa());
	static char ID;
	TestPass() : ModulePass(ID) { }

	//list of pair<blockName, list-of-Instructions>
		//typedef std::list<std::pair<std::string, std::list<Instruction *> > > funcDump;
		std::map<std::string, funcDump> workingList;

		bool runOnModule(Module &M) {
			//errs() << "In module called: " << M.getName() << "!\n";
			std::string funcName, bkName;
			for (Function &F : M) {	//Found a new Function

				if (isa<Function>(F) && !(F.getName().startswith("llvm."))) {
					funcName = F.getName();
				//	outs() << "\n " << funcName << " : Found a Function !!\n";
				//	outs() << "Function Name = " << F.getName();
					std::pair<std::string, std::list<Instruction *> > funcBlockList;
					std::list<std::pair<std::string, std::list<Instruction *> > > wholeFuncBlocks;
					for (BasicBlock &B : F) {	//Blocks of the Function
						if (isa<BasicBlock>(B)) {
							bkName = B.getName();
							//outs() << "\n BasicBlock: " << bkName << "\n";
						}
						std::list<Instruction *> listInst;

						for (auto &I : B) {
							Instruction *ins;
							ins = &I;
							listInst.push_back(ins);
			//				(*ins).dump();
						}
						funcBlockList.first = bkName;
						funcBlockList.second = listInst;
				//		std::cout << std::endl;
						wholeFuncBlocks.push_back(funcBlockList);
					}

					workingList[funcName] = wholeFuncBlocks;//Mapping of the functions
				//	outs() << "\nI am out of the Function\n";
					//workingList.pu
				}
			}
			/*outs()<< "\n =======Reading all the Function is Over!!! ====\n";

			std::map<std::string, funcDump>::iterator mit;
			for (mit = workingList.begin(); mit != workingList.end(); mit++) {
				outs() << "\n ==== Function name: " << (*mit).first;
				outs() << "\n =====Reading Map!!!============\n";
			}
*/

			ir_ssa->setFunctionDump(workingList);	//required for recursive function call
			//outs() << "\nWorking list of Function set\n";

			funcDump funcData;
			funcData = workingList["controller"];	//Starting from the controller function
			//std::list<std::pair<std::string, std::list<Instruction *> > >::iterator fit;

			outs() << "\nBefore Call to Function convert to SSA\n";
			convertFunctionToSSA(funcData, ir_ssa);


			std::ofstream outFile, outFile2;
			outFile.open("fmsafe_Out.ssa");
			printSSA_toFile(outFile, ir_ssa);
			bool optimization=true;
			if (optimization){
				optIRssa::ptr opt_ir_ssa = optIRssa::ptr(new optIRssa());
				//get a copy of the unoptimized SSA form and its variables
				opt_ir_ssa->setAllInsts(ir_ssa->getAllInsts());	//get a copy of the unoptimized SSA form
				opt_ir_ssa->setInputVariables(ir_ssa->getInputVariables());
				opt_ir_ssa->setIntermediateVariables(ir_ssa->getVariables());
				opt_ir_ssa->setOutputVariables(ir_ssa->getOutputVariables());

				// @@Debug printing SSA
				std::list<std::pair<unsigned int, std::string> > smt = ir_ssa->getAllInsts();
				for (std::list<std::pair<unsigned int, std::string> >::iterator it =
							smt.begin(); it != smt.end(); it++) {
					std::cout <<(*it).first <<")    " << (*it).second << std::endl;
				}

				opt_ir_ssa->optimizeInstCombine();

				outFile2.open("fmsafe_optOut.ssa");
				print_optSSA_toFile(outFile2, opt_ir_ssa);


			}


		return false;
	}

};

}

char TestPass::ID = 0;
static RegisterPass<TestPass> X("testPass", "Testing A Pass");

static void registerTestPass(const PassManagerBuilder &,
		legacy::PassManagerBase &PM) {
	PM.add(new TestPass());
}
static RegisterStandardPasses RegisterMyPass(PassManagerBuilder::EP_ModuleOptimizerEarly, registerTestPass);

static RegisterStandardPasses RegisterMyPass0(PassManagerBuilder::EP_EnabledOnOptLevel0, registerTestPass);

//Automatically enable the pass.
//http://adriansampson.net/blog/clangpass.html

