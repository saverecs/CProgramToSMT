/*
 * coreWrapperSSA.cpp
 *
 *  Created on: 12-Jun-2018
 *      Author: amit
 */

#include "coreWrapperSSA.h"



typedef std::list<std::pair<std::string, std::list<Instruction *> > > funcDump;


void printSSA_toFile(std::ofstream& outFile, IRssa::ptr& ir_ssa){

	/* At this point, the controller program have been read and converted into an SSA form with prefix notation
	 * as required in the SMT solver (in our case dReal) for one unrolling depth, i.e., k=0;
	 * Now we dump the output into a file so that our FMSafe project can use it for hybrid analysis.
	 * The format of the output file is:
	 * First line contain three(3) numbers separated by a space.
	 * 		The first number is the size of the total intermediate variables used in the entire SSA process.
	 * 		The second number is the size of the total input variables present in the controller program which is passed
	 * 			as input arguments with the reference structure data type named "INPUT_VAL".
	 * 		The third number is the size of the total output variables present in the controller program which is passed
	 * 			as output arguments with the reference structure data type named "RETURN_VAL".		 *
	 *
	 * Next contains the detailed/values described above. The syntax of these values are "DataType  VariableName".
	 * 			The order of occurrence of these variables are "intermediate variables", "input variables" followed
	 * 			by the "output variables".
	 *
	 * Following the variables name and their data type are the lines comprising the actual SSA statements.
	 *		Note I did not printed the size of the SSA statements since, the size of the list smt.size() may not
	 *		reflect the correct values, because some statements may have more than one lines due to the phi-node process
	 */



	//std::list<std::string> vars = ir_ssa->getVariables();
	std::list<std::pair<std::string, std::string> > vars = ir_ssa->getVariables();
	std::list<variable> inVars = ir_ssa->getInputVariables(), outVars= ir_ssa->getOutputVariables();
	std::list<std::pair<unsigned int, std::string> > smt = ir_ssa->getAllInsts();

	outFile << vars.size() << " " << inVars.size() << " " << outVars.size() << std::endl;

	//outFile <<"Intermediat SSA Variables" << std::endl;
	for (std::list<std::pair<std::string, std::string> >::iterator it = vars.begin();
			it != vars.end(); it++) {
		outFile << (*it).second << " " <<(*it).first<< std::endl;
//			std::cout << (*it).second << " " <<(*it).first<< std::endl;
	}
	//outFile << std::endl;	//Inserting a blank Line
//		outs() << "\n";

	//outFile <<"Controller's Input Variables" << std::endl;
	for (std::list<variable>::iterator it = inVars.begin();
			it != inVars.end(); it++) {
		outFile << (*it).varType << " " << (*it).varName << std::endl;
//			outs() << (*it).varType << " " << (*it).varName << "\n";
	}
	//outFile << std::endl;	//Inserting a blank Line
//		outs() << "\n";

	//outFile <<"Controller's Output Variables" << std::endl;
	for (std::list<variable>::iterator it = outVars.begin();
			it != outVars.end(); it++) {
		outFile << (*it).varType << " " << (*it).varName << std::endl;
//			outs() << (*it).varType << " " << (*it).varName << "\n";
	}
	//outFile << std::endl;	//Inserting a blank Line
//		outs() << "\n";

	for (std::list<std::pair<unsigned int, std::string> >::iterator it =
			smt.begin(); it != smt.end(); it++) {
		outFile << (*it).second << std::endl;
//			std::cout << (*it).first << "    " << (*it).second << std::endl;
	}

	outFile.close();
}



/*
 * Main wrapper function that makes recursive calls to function within function calls (if any).
 */
void convertFunctionToSSA(funcDump& F, IRssa::ptr& ir_ssa) {
	std::cout<<"Inside converFunction To SSA" << std::endl;

//Creating Local object for "allStackVariables" for working in local memory due to recursive call to this function

	allStackVariables::ptr workingVariable = allStackVariables::ptr(new allStackVariables());
	//IRssa::ptr ir_ssa = IRssa::ptr(new IRssa());
	//std::cout<<"After creation of workingVariable" << std::endl;

	//outs() << "\nInside  :" << F.getName() << "()\n";
	unsigned int bkNumber = 0;

	//Todo: I can also count the different types of operands
	//See https://sites.google.com/site/amitgurung777/clang-and-llvm-practise-stuff?pli=1
	std::string st1 = "";

	workingVariable->setFlagPhiBlock(false); //Initially false

	workingVariable->setPreviousBlockCondition(false); //Initialization

	std::cout<<"Number of blocks"<<F.size() << std::endl;
	for (funcDump::iterator bit = F.begin(); bit != F.end(); bit++){	//BasicBlock &B : F) {

		std::pair<std::string, std::list<Instruction *> > blockB;
		//BasicBlock &B = (*bit);
		blockB = *bit;

		outs() << "\nBasic Block Name: " << blockB.first << "\n";
		std::string bkName = "", st = "";
		bkName = blockB.first;
		workingVariable->setCurrentBlockName(bkName);
		struct blockOrdering bkSequence;
		bkNumber++;
		bkSequence.blockNumber = bkNumber;	//Entry block starts with ID=1
		bkSequence.blockBlock = bkName;
		workingVariable->setBlockSequence(bkSequence);//std::cout << "===My Block Name=" << bkName << "\n";
		//B.dump();

		/*
		 * Get the top of the element from TrueFalseBlock and search for bkName and delete matching items.
		 * For each deleted items insert a closing ")" brackets
		 */
		//if (!(ir_ssa->getTrueFalseBlockStack().empty())){	//Empty stack indicate the entry block case
		if (!(boost::iequals(bkName, "entry"))) { //Only for entry block we skip bracketing rules
			if (workingVariable->isPreviousBlockConditional()) { //if true then just search and delete the
														// first occurrence of blockName from the Stack
				// ************** Logic for !(condition) *********************
				std::list<brData> br_stack;
				std::list<brData>::iterator itStack, tt;
				br_stack = workingVariable->getBrStack();
				itStack = br_stack.begin();	//Getting the top of the stack i.e., the previous conditional instruction
				//std::cout << "Stack False-Label = " << (*itStack).falseLabel<<"!!!!!!!!!!\n";
				if (boost::iequals(bkName, (*itStack).falseLabel)) {
					//Find the previous smt string which is a "(ite cond", so replace it with "(ite (not cond)" (can use for for error-trapping)
					//Logic: find out the first space in it and replace it
					std::list<std::pair<unsigned int, std::string> >::iterator stIt =
							ir_ssa->getAllInsts().end();
					stIt--; //Last record before end pointer
					std::string st = (*stIt).second, new_st = "", cond; //	std::cout << "Last string smt = " << (*stIt);
					unsigned int ssaNumber = (*stIt).first;
					//typedef boost::tokenizer<boost::char_separator <char> > tokenizer;
					//std::string s = "(ite or.cond5";
					//tokenizer tok {s};
					boost::char_separator<char> sep(" "); //, and " " a single space is the separator
					boost::tokenizer < boost::char_separator<char>
							> tok(st, sep);
					boost::tokenizer<boost::char_separator<char> >::iterator tokPtr;
					tokPtr = tok.begin();
					//						BOOST_FOREACH (const string& t, tok) {
					//							std::cout<<"t = "<<t<<std::endl;
					//						}
					new_st.append((*tokPtr)); //First token i.e., "(ite"
					new_st.append(" (not ");
					tokPtr++;
					new_st.append((*tokPtr)); //Second token i.e., "cond"
					new_st.append(" )");

					if (!(ir_ssa->replaceAllInsts(st, new_st)))	//we can also pass ssaNumber for valid replacement
						std::cout
								<< "False Block executed first not operator replacement UnSuccessful!!\n";
					//std::cout << "string smt after modification= " << new_st << std::endl;
					//(*stIt) = new_st;	//replacing with the modified smt string

				}
				// ************** END Logic for !(condition) ******** Note first this block is must ******

				// ************** Logic for start of the 2nd block of br instruction (True/False) *********************

				std::list<std::pair<unsigned int, std::string> > stackList;
				std::list<std::pair<unsigned int, std::string> >::iterator it;

				//std::list<std::string> stackList;
				//std::list<std::string>::iterator it;
				stackList = workingVariable->getTrueFalseBlockStack();
				for (it = stackList.begin(); it != stackList.end(); it++) {
					if (boost::iequals((*it).second, bkName)) {
						/*std::pair<unsigned int, std::string> TrueFalseData;
						 TrueFalseData.first = (*it).first;
						 TrueFalseData.second = (*it).second;*/

						workingVariable->deleteTrueFalseBlockStack(it);//Found the first matching position

						IRssa::ssaLineNo++;	//static increment
						ir_ssa->setInst(IRssa::ssaLineNo, "(and ");	//Putting 'and' for compound instructions support for single inst also

						//ir_ssa->setInst("(");
						break;//Found the first matching position
					}
				}
				// ************** END Logic for start of True/False block *********************

			} else {//previous block had a single ending label. So, search and delete stack data and for each
				// deleted item insert a ")" closing brackets
				if (!(boost::iequals(bkName, workingVariable->getPrevious_br_label()))) { //This means the else block is executed
					/*It could be the 2nd branch of the if instruction, so the label can be in the Stack at the top.
					 The top element or the current blockName will NOT be equal to the previousBlock's br label. */
					std::list<std::pair<unsigned int, std::string> > stackList;
					std::list<std::pair<unsigned int, std::string> >::iterator it;
					stackList = workingVariable->getTrueFalseBlockStack();
					for (it = stackList.begin(); it != stackList.end(); it++) {
						if (boost::iequals((*it).second, bkName)) {
							workingVariable->deleteTrueFalseBlockStack(it); //Found the first matching position (usually/will be at top)

							IRssa::ssaLineNo++;	//static increment
							ir_ssa->setInst(IRssa::ssaLineNo, "(and ");	//Putting 'and' for compound instructions support for single inst also

							//ir_ssa->setInst("(");
							break;//Found the first matching position
						}
					}
				} else if (boost::iequals(bkName,
						workingVariable->getPrevious_br_label())) {
					//Such blocks can be from 1) the sequential block and 2) a conditional br instruction's true or false branch.
					//In such case of 2) this blockName will be found in the stackList below, so we if find it then we can replace "ite" with "=>"
					std::list<std::pair<unsigned int, std::string> > stackList;
					std::list<std::pair<unsigned int, std::string> > tempDeletingData;//creating to keep all found pair for deleting
					std::list<std::pair<unsigned int, std::string> >::iterator it;
					stackList = workingVariable->getTrueFalseBlockStack();
					bool Found = false;
					int count = 0;
					for (it = stackList.begin(); it != stackList.end(); it++) { //For each deleted element

						if (boost::iequals((*it).second, bkName)) {
							//	ir_ssa->deleteTrueFalseBlockStack(it);//Found a matching position
							IRssa::ssaLineNo++;	//static increment
							ir_ssa->setInst(IRssa::ssaLineNo, " )");

							//ir_ssa->setInst(")");	//condition part closing
							Found = true;
							count++;
							tempDeletingData.push_back((*it));

							ir_ssa->replacePrevious_ite_toImplication((*it));//replace for this (*it).first = all_insts.ssaLineNo
						}
					}
					//std::cout << "\nCOUNT=" << count << std::endl;
					if (count == 0) {//Not Found. Like the case of Last block.
						IRssa::ssaLineNo++;	//static increment
						ir_ssa->setInst(IRssa::ssaLineNo, " )");

						//ir_ssa->setInst(")");//ite closing in both case Found or NOT Found
					} else if (Found && count == 1) {
						for (std::list<std::pair<unsigned int, std::string> >::iterator it =
								tempDeletingData.begin();
								it != tempDeletingData.end(); it++) {
							workingVariable->deleteAllTrueFalseBlockStack((*it));//Delete all elements with bkName
						}

						//ir_ssa->deleteAllTrueFalseBlockStack(bkName);//Delete all elements with bkName
					} else if (count > 1) {
						for (std::list<std::pair<unsigned int, std::string> >::iterator it =
								tempDeletingData.begin();
								it != tempDeletingData.end(); it++) {
							workingVariable->deleteAllTrueFalseBlockStack((*it));//Delete all elements with bkName
						}
						//ir_ssa->deleteAllTrueFalseBlockStack(bkName);//Delete all elements with bkName
						for (int i = 1; i < count; i++) {//One less closing for ite

							IRssa::ssaLineNo++;	//static increment
							ir_ssa->setInst(IRssa::ssaLineNo, " )");

							//ir_ssa->setInst(")");	//ite closing
						}
					}
				}
			}
		}
		std::list<Instruction *> BInsts;
		BInsts = blockB.second;
		for (std::list<Instruction *>::iterator it = BInsts.begin(); it	!= BInsts.end(); it++){ //Instruction &I : B) {
			// it.dump();
			ir_ssa->parseInstruction(*(*it), workingVariable);
		}
		workingVariable->setPreviousBlockName(bkName);
		workingVariable->setFlagPhiBlock(false);	//leaving the block and before entering the block initially false
	}
	//end of blocks of the function
		//End-Point of the function
}
