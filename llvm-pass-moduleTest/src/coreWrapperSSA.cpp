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

/*
 * Creating Local object for "allStackVariables" for working as a local memory due to recursive call to this function.
 */

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

	std::cout<<"Number of blocks = "<<F.size() << std::endl;
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
			parseInstruction(*(*it), workingVariable, ir_ssa);
		}
		workingVariable->setPreviousBlockName(bkName);
		workingVariable->setFlagPhiBlock(false);	//leaving the block and before entering the block initially false
	}
	//end of blocks of the function
		//End-Point of the function
}



void parseInstruction(llvm::Instruction &instruction, allStackVariables::ptr& workingVariable, IRssa::ptr& ir_ssa) {
	std::string my_inst = ""; //For every instruction it will start from the empty
	//simpleVariable < simpleVar;
	instruction.dump();

	//if (DbgDeclareInst *dbg = dyn_cast<DbgDeclareInst>(&instruction)) {
	if (CallInst *Inst = dyn_cast<CallInst>(&instruction)) {
		/* Todo: have a look at it to improve */
		if (Function *F = Inst->getCalledFunction()) {
			if (F->getName().startswith("llvm.dbg.value")) {
				//std::cout<< "llvm.dbg.value Found Variable\n";
				//Value *val = dyn_cast<Value>(Inst->getArgOperand(0));
				//outs()<<"Name=" << val->getName() << "\n";
				//std::cout << "Name="<< (string) (Inst->getArgOperand(0)->getName()) << "\n";
				//outs()<<"Name=" << (*Inst->getOperand(0))<<"\n";
				std::string Str;
				raw_string_ostream OS(Str);
				OS << (*Inst->getOperand(0));	//redirecting to string variable
				OS.flush();
				//std::cout<<"Var = "<<Str<<std::endl;
				int pos = Str.find("%");	//searching the % char in the string
				//std::cout<<"% substr/var = "<<Str.substr(pos+1)<<std::endl;
				if (pos > 0){ //otherwise a value creation due to optimization
					ir_ssa->setVariable(Str.substr(pos + 1), "Real"); //Found a variable declaration
					//std::cout<<"Type = Real and Var = "<<Str.substr(pos + 1)<<std::endl;
				}
			} else if (!(F->getName().startswith("llvm."))) { //indicate that it is a function call
				string theFunction = F->getName();
				//Search this in the map and do a recursive: Note:- Take care/make sure variables are local, so that stack logic works
				std::cout<<"========= Found a call to function: "<<theFunction << "==============" <<std::endl;
				funcDump nextFunction;
				std::map<std::string, funcDump> myfuncData = ir_ssa->getFunctionDump(); //Getting the entire functionDump list/map
				nextFunction = myfuncData[theFunction];
				if (nextFunction.size()> 1){
					std::cout<<"Found list of Blocks ("<<nextFunction.size()<<")/Instruction inside the Function!!\n";
					std::cout<<std::endl;	std::cout<<std::endl;

					convertFunctionToSSA(nextFunction, ir_ssa);

				}
				std::cout << std::endl;
			}
		}
	}				//End of function_call of llvm.dbg

	/*Allocate Instruction */
	if (isa<AllocaInst>(&instruction)) {//Will not have effect due to optimization
		AllocaInst *allocaInst = dyn_cast<AllocaInst>(&instruction);
		/*std::string st = "";
		my_inst.append(st);
		st = allocaInst->getName().str(); //verify replacing
		my_inst.append(st);*/

		std::string st = "", varType;
		//my_inst.append(st);
		st = allocaInst->getName(); //Note we get this lable %tmp only on running the pass -instnamer
		//my_inst.append(st);	//Also a new intermediate variable is created as tmp

		//Type *type = allocaInst->getType();	//	NOT correct
		Type *type = allocaInst->getAllocatedType();	//	Correct allocated type

		if (type->isIntegerTy())
			varType = "Int";
		else if (type->isDoubleTy())
			varType = "Real";
		else if (type->isFloatTy())
			varType = "Real";
		else if (type->isStructTy()){
			cout<<"Don't know it is struct type"<<std::endl;
			varType = "Struct";
	//		varType = "Real";
		}else{
			cout<<"Unknown DataType"<<std::endl;
			varType = "Real";//Todo:: For now converting to real
		}
		//std::cout << "\nVariable name = " << st << " and type = "<<varType << std::endl;
		//std::cout << "\nAlignment value  = " << allocaInst->getAlignment() << std::endl;

		ir_ssa->setVariable(st, varType);		  //Found a variable declaration



		//	simpleVar.varDataType = allocaInst->getAllocatedType();
		//simpleVar.varName = allocaInst->getName();
		//	simpleVar.varValue = string(0);
//		std::cout << "\t\tDefining variable = " << my_inst << ";" << std::endl;


	}

	/*STORE Instruction */
	if (isa<StoreInst>(&instruction)) {	//Will not have effect due to optimization
		std::string st = "(= ";
		my_inst.append(st);
		StoreInst *storeInst = dyn_cast<StoreInst>(&instruction);
		st = storeInst->getOperand(1)->getName();
		my_inst.append(st);
		my_inst.append(" ");

		st = getVariable_or_Value(storeInst->getOperand(0));

		//outs()<<"  Operand 1= "<< storeInst->getOperand(1)->getName();
		my_inst.append(st);
		st = " )";
		my_inst.append(st);
//		std::cout << my_inst << std::endl;
	}

	/*LOAD Instruction:  check for new version of the variables */
	if (isa<LoadInst>(&instruction)) {//Will not have effect due to optimization
		LoadInst *loadInst = dyn_cast<LoadInst>(&instruction);
		std::string st = "(= ", varType;
		my_inst.append(st);
		st = loadInst->getName(); //Note we get this lable %tmp only on running the pass -instnamer
		my_inst.append(st);	//Also a new intermediate variable is created as tmp

		if (loadInst->getType()->isIntegerTy())
			varType = "Int";
		else if (loadInst->getType()->isDoubleTy())
			varType = "Real";
		else if (loadInst->getType()->isFloatTy())
			varType = "Real";
		else
			varType = "Real";//Todo:: For now converting to real

		ir_ssa->setVariable(st, varType);		  //Found a variable declaration
		my_inst.append(" ");
		st = loadInst->getOperand(0)->getName();
		my_inst.append(st);
		my_inst.append(" )");
	//	llvm::outs() << my_inst << "\n";
	}

	if (GetElementPtrInst *getElementPtrInst = dyn_cast<GetElementPtrInst>(
			&instruction)) {
		/*
		 * We assume the the controller-program will have inputs as struct.INPUT_VAL and output as struct.RETURN_VAL
		 * To obtain the actual name of the variables of the structure. It is suggested to have a different names for
		 * variables in the program.
		 */
		string varType, varName, structName;
		unsigned int id=0;

		Type *type = getElementPtrInst->getSourceElementType();
		//std::string name = getElementPtrInst->getOperand(0)->getName();
		varName = getElementPtrInst->getName();
	//	varType = "Real";

		StructType *mytype = dyn_cast<StructType>(getElementPtrInst->getType());
		string mystructIndex;
		mystructIndex = getVariable_or_Value(getElementPtrInst->getOperand(2));
		//outs()<<"Index = " <<mystructIndex;
		unsigned int ind = stoi(mystructIndex);
		Type *tt = type->getStructElementType(ind);	//Todo:: Need to verify for Int type
		if (tt->isIntegerTy()) {
			varType = "Int";
		//	outs() << varType << "\n";
		} else if (tt->isDoubleTy()) {
			varType = "Real";
		//	outs() << varType << "\n";
		} else if (tt->isFloatTy()) {
			varType = "Real";
		//	outs() << varType << "\n";
		}

	//	outs() << "\n==OpCodeName(0)'s Name:"<<getElementPtrInst->s<<"==\n";

		/*outs() << "\n====Name:"<<getElementPtrInst->getName()<<"=====";
		outs() << "\n====Operand(0)'s Name:"<<getVariable_or_Value(getElementPtrInst->getOperand(0))<<"=====";
		outs() << "\n====Operand(1)'s Name:"<<getVariable_or_Value(getElementPtrInst->getOperand(1))<<"=====";
		outs() << "\n====Operand(2)'s Name:"<<getVariable_or_Value(getElementPtrInst->getOperand(2))<<"=====";
		outs() << "\n====OpCodeName's Name:"<<getElementPtrInst->getOpcodeName()<<"=====\n";
		outs() << "\n====OpCodeName(0)'s Name:"<<getElementPtrInst->getOpcodeName(0)<<"=====\n";
		outs() << "\n====OpCodeName(1)'s Name:"<<getElementPtrInst->getOpcodeName(1)<<"=====\n";
		outs() << "\n====OpCodeName(2)'s Name:"<<getElementPtrInst->getOpcodeName(2)<<"=====\n";*/

		//if it is a structure , then two operands structname(0) and index(1)
		if (type->getTypeID() == Type::StructTyID) {
			structName = type->getStructName();
			string num = getVariable_or_Value(getElementPtrInst->getOperand(2));
			id = stoi(num);//Index of the element, just used as Id with no sense as ID not used at the moment
			if (boost::iequals(structName, "struct.INPUT_VAL")){	//indicates the INPUT variables
				ir_ssa->setInputVariable(varName, id, varType);
			} else if (boost::iequals(structName, "struct.RETURN_VAL")){	//indicates the OUTPUT variables
				ir_ssa->setOutputVariable(varName, id, varType);
			}
		}
	}

	//Type casting instruction
	if (isa<SIToFPInst>(instruction)) {
		SIToFPInst *siToFPInst = dyn_cast<SIToFPInst>(&instruction);
		std::string st = "(= ", varType;
		my_inst.append(st);
		st = siToFPInst->getName();
		my_inst.append(st);
		my_inst.append(" ");

		//Todo get the type... Need to verify with the program using siToFp in IR
		if (siToFPInst->getType()->isIntegerTy())
			varType = "Int";
		else if (siToFPInst->getType()->isDoubleTy())
			varType = "Real";
		else if (siToFPInst->getType()->isFloatTy())
			varType = "Real";
		else
			varType = "Real";//Todo:: For now converting to real

		ir_ssa->setVariable(st, varType);					//Found a variable declaration
		st = getVariable_or_Value(siToFPInst->getOperand(0));
		my_inst.append(st);
		st = " )";
		my_inst.append(st);
	}

	if (isa<ZExtInst>(instruction)) {

		ZExtInst *zExtInst = dyn_cast<ZExtInst>(&instruction);
		std::string st, varType, convertBoolExp;
//		outs()<<"getVariable_or_Value(zExtInst->getOperand(0)) = "<<getVariable_or_Value(zExtInst->getOperand(0))<<"\n";
//		outs()<<"zExtInst->getName() = "<<zExtInst->getName()<<"\n";
		convertBoolExp.append("(ite ");
		convertBoolExp.append(getVariable_or_Value(zExtInst->getOperand(0)));
		convertBoolExp.append(" (= ");
		convertBoolExp.append(zExtInst->getName());
		convertBoolExp.append(" 1)");

		convertBoolExp.append(" (= ");
		convertBoolExp.append(zExtInst->getName());
		convertBoolExp.append(" 0))");

		//outs()<<"convertBoolExp = "<<convertBoolExp<<"\n";
		my_inst.append(convertBoolExp);
		//Todo get the type... Need to verify with the program using IR
		if (zExtInst->getType()->isIntegerTy())
			varType = "Int";
		else if (zExtInst->getType()->isDoubleTy())
			varType = "Real";
		else if (zExtInst->getType()->isFloatTy())
			varType = "Real";
		else
			varType = "Real";		//Todo:: For now converting to real
		st=zExtInst->getName();//Found a variable declaration
		ir_ssa->setVariable(st, varType);

		/*st = "(= ";
		my_inst.append(st);
		st = zExtInst->getName();
		my_inst.append(st);
		my_inst.append(" ");
		setVariable(st, varType);				//Found a variable declaration
		st = getVariable_or_Value(zExtInst->getOperand(0));
		my_inst.append(st);
		st = ")";
		 my_inst.append(st);*/
	}
	//casting from double/float-point to Unsigned integer
	if (isa<FPToUIInst>(&instruction)) {
		FPToUIInst *fpToUIInst = dyn_cast<FPToUIInst>(&instruction);
		std::string st = "(= ", varType;
		my_inst.append(st);
		st = fpToUIInst->getName();
		my_inst.append(st);
		my_inst.append(" ");


		//Todo get the type... Need to verify with the program using siToFp in IR
		if (fpToUIInst->getDestTy()->isIntegerTy())
			varType = "Int";
		else if (fpToUIInst->getDestTy()->isDoubleTy())
			varType = "Real";
		else if (fpToUIInst->getDestTy()->isFloatTy())
			varType = "Real";
		else {
			cout << "Unknown DataType" << std::endl;
			varType = "Real";		//Todo:: For now converting to real
		}

		ir_ssa->setVariable(st, varType);		//Found a variable declaration
		//std::cout << "\nVariable name = " << st << " and type = "<<varType << std::endl;
		st = getVariable_or_Value(fpToUIInst->getOperand(0));
		my_inst.append(st);
		st = " )";
		my_inst.append(st);
	}
	//casting from Unsigned integer to double/float-point
	if (isa<UIToFPInst>(&instruction)) {
		UIToFPInst *uiToFPInst = dyn_cast<UIToFPInst>(&instruction);
		std::string st, lvar, varType, varTypeSrc;
		lvar = uiToFPInst->getName();
		//outs()<<"uiToFPInst->getSrcTy()->ID = "<<uiToFPInst->getSrcTy()->getTypeID()<<"\n";
		if (uiToFPInst->getSrcTy()->isIntegerTy()) {
			varTypeSrc = "Int";
		} else if (uiToFPInst->getSrcTy()->isHalfTy()) {
			varTypeSrc = "Int";
		} else {
			cout << "Unknown DataType" << std::endl;
			varTypeSrc = "Int";		//Todo:: For now converting to real
		}
		if (uiToFPInst->getDestTy()->isDoubleTy()) {
			varType = "Real";
		} else if (uiToFPInst->getDestTy()->isFloatTy()) {
			varType = "Real";
		} else {
			cout << "Unknown DataType" << std::endl;
			varType = "Real";		//Todo:: For now converting to real
		}
		ir_ssa->setVariable(lvar, varType);		//Found a variable declaration
		//std::cout << "\nVariable = " << lvar << " and type = " << varType<< std::endl;

		string rvar = getVariable_or_Value(uiToFPInst->getOperand(0));
		/*Search op1 in the list of variables if its dataType is Bool then do as below:
		 (ite rvar (lvar = 1.0) (lvar = 0.0))
		 Otherwise just do simple assignment from Integer to Real (this supported in SMT2) */
		if (ir_ssa->findVariable(rvar, varTypeSrc)) {	//if rvar found
			if (boost::iequals(varTypeSrc, "Bool")) {
				my_inst.append("( ite ");
				my_inst.append(rvar);
				my_inst.append(" ( = ");
				my_inst.append(lvar);
				my_inst.append(" 1.0 )");
				my_inst.append(" ( = ");
				my_inst.append(lvar);
				my_inst.append("  0.0 ) )");
			} else { //rvar not found then, it is just Integer casting to Real
				std::string st1;
				my_inst.append("(= ");
				st1 = uiToFPInst->getName();
				my_inst.append(st1);
				my_inst.append(" ");
				my_inst.append(rvar);
				my_inst.append(" )");
			}
		}
	}
//	cout<<"my_inst = " <<my_inst<<"\n";

	if (isa<SelectInst>(&instruction)) {	//Handling only simple i1 type instruction
		/*
		 * Syntax:   <result> = select selty <cond>, <ty> <val1>, <ty> <val2>   ; yields ty
		 * (ite cond (res = val1) (res = val2))
		 */

		SelectInst *selectInst = dyn_cast<SelectInst>(&instruction);
		std::string st = "(ite ", cond, TrueExp, FalseExp, varType;
		cond = getVariable_or_Value(selectInst->getOperand(0));

		ir_ssa->setVariable(cond, "Bool");	//if already declared will be ignored
		TrueExp.append("(= ");
		TrueExp.append(selectInst->getName());

		//Todo get the type... Need to verify with the program using IR
		if (selectInst->getType()->isIntegerTy())
			varType = "Int";
		else if (selectInst->getType()->isDoubleTy())
			varType = "Real";
		else if (selectInst->getType()->isFloatTy())
			varType = "Real";
		else
			varType = "Real";		//Todo:: For now converting to real

		ir_ssa->setVariable(selectInst->getName(), varType);
		TrueExp.append(" ");
		TrueExp.append(getVariable_or_Value(selectInst->getOperand(1)));
		TrueExp.append(" )");

		FalseExp.append("(= ");
		FalseExp.append(selectInst->getName());
		FalseExp.append(" ");
		FalseExp.append(getVariable_or_Value(selectInst->getOperand(2)));
		FalseExp.append(" )");			//totalExpr = st + cond + TrueExp + FalseExp + ")";

		my_inst.append(st);
		my_inst.append(cond);
		my_inst.append(" ");
		my_inst.append(TrueExp);
		my_inst.append(FalseExp);
		my_inst.append(" )");

	}

	/*
	 * Redesign icmp and fcmp instruction, since there can be compound(use of && and ||) conditional statements.
	 * The "instruction.getOpcodeName()" returns the operation name. It returns 'and' or 'or' after two or more successive icmp/fcmp instructions
	 * in which case a single 'ite' smt SSA-code is to be generated for the previous two icmp/fcmp instructions.
	 */
	/*if (CmpInst *cmpInst = dyn_cast<CmpInst>(&instruction)) { //Compare Instruction: are of type ICmpInst and FCmpInst
		//std::cout<<"Found Compare instruction\n";
		struct phiData condition_data;
		if (ICmpInst *IcmpInst = dyn_cast<ICmpInst>(&instruction)) { //integer operations
			std::string st = "", cmp_condtion = "";
			//std::pair<std::string, std::string> condition_data;	//outs()<<"Condition Name="<<IcmpInst->getName()<<"\n";
			condition_data.blockLabel = getCurrentBlockName();
			condition_data.condName = IcmpInst->getName();
			st = "(ite (";	//llvm::outs() << " (ite (> ";
			my_inst.append(st);
			cmp_condtion.append("(");
			cmp_condtion.append(getOperator_Integer(IcmpInst->getPredicate()));
			//cmp_condtion =getOperator_Integer(IcmpInst->getSignedPredicate());
			//std::cout<<"Cmp condition = "<<cmp_condtion<<"\n";
			st = getVariable_or_Value(IcmpInst->getOperand(0));
			cmp_condtion.append(st);
			cmp_condtion.append(" ");
			st = getVariable_or_Value(IcmpInst->getOperand(1));
			//my_inst.append(IcmpInst->getOperand(1)->getName());
			cmp_condtion.append(st);
			cmp_condtion.append(") ");
			my_inst.append(cmp_condtion);
			//std::cout<<"Cmp condition = "<<cmp_condtion<<"\n";
			condition_data.conditionExp = cmp_condtion;
			pushTop(condition_data);//Storing the cmp_conditional at the top of the stack data-structure
		} else if (FCmpInst *FcmpInst = dyn_cast<FCmpInst>(&instruction)) {//Conditional statement on Floating operations
			std::string st = "", cmp_condtion = "";
			//std::pair<std::string, std::string> condition_data;
			condition_data.blockLabel= getCurrentBlockName();
			condition_data.condName = FcmpInst->getName();
			st = "(ite (";	//llvm::outs() << " (ite (> ";
			my_inst.append(st);
			//std::cout << "Condition good" << std::endl;
			cmp_condtion.append("(");
			cmp_condtion.append(getOperator_Float(FcmpInst->getPredicate()));
			//std::cout << "Cmp condition = " << cmp_condtion << "\n";
			st = getVariable_or_Value(FcmpInst->getOperand(0));
			cmp_condtion.append(st);
			cmp_condtion.append(" ");
			st = getVariable_or_Value(FcmpInst->getOperand(1));
			//my_inst.append(IcmpInst->getOperand(1)->getName());
			cmp_condtion.append(st);
			cmp_condtion.append(") ");
			my_inst.append(cmp_condtion);
			//std::cout<<"Cmp condition = "<<cmp_condtion<<"\n";
			condition_data.conditionExp = cmp_condtion;
			pushTop(condition_data);//Storing the cmp_conditional at the top of the stack data-structure
		}
	}*/

	if (CmpInst *cmpInst = dyn_cast<CmpInst>(&instruction)) { //Compare Instruction: are of type ICmpInst and FCmpInst
		struct phiData condition_data;
		std::string st = "", cmp_condtion = "", cmp_str="", varType="Bool", operatorSign;

		if (ICmpInst *IcmpInst = dyn_cast<ICmpInst>(&instruction)) { //integer operations
			condition_data.blockLabel = workingVariable->getCurrentBlockName();
			condition_data.condName = IcmpInst->getName();

			ir_ssa->setVariable(IcmpInst->getName(), varType);

			//cmp_condtion.append("(");
			operatorSign = getOperator_Integer(IcmpInst->getPredicate());
			if (boost::iequals(operatorSign, "!= ")){	//not (= op1 op2) :Note the space in "!= "
				cmp_condtion.append("(not (= ");
				//cmp_condtion.append(operatorSign);//outs()<<"\nFound != comparison\n\n";
				cmp_condtion.append(" ");
				st = getVariable_or_Value(IcmpInst->getOperand(0));
				cmp_condtion.append(st);
				cmp_condtion.append(" ");
				st = getVariable_or_Value(IcmpInst->getOperand(1));
				cmp_condtion.append(st);
				cmp_condtion.append(" ))");
			} else {
				cmp_condtion.append("(");
				cmp_condtion.append(operatorSign);
				cmp_condtion.append(" ");
				st = getVariable_or_Value(IcmpInst->getOperand(0));
				cmp_condtion.append(st);
				cmp_condtion.append(" ");
				st = getVariable_or_Value(IcmpInst->getOperand(1));
				cmp_condtion.append(st);
				cmp_condtion.append(" )");
			}

			cmp_str.append("(= ");
			cmp_str.append(IcmpInst->getName());
			cmp_str.append(" ");
			cmp_str.append(cmp_condtion);
			cmp_str.append(" )");
		//	my_inst.append(cmp_str);
		} else if (FCmpInst *FcmpInst = dyn_cast<FCmpInst>(&instruction)) { //Conditional statement on Floating operations
			//std::string st = "", cmp_condtion = "";
			condition_data.blockLabel = workingVariable->getCurrentBlockName();
			condition_data.condName = FcmpInst->getName();

			ir_ssa->setVariable(FcmpInst->getName(), varType);

			operatorSign = getOperator_Float(FcmpInst->getPredicate());
			if (boost::iequals(operatorSign, "!= ")) {	//not (= op1 op2)
				cmp_condtion.append("(not (= ");
				//cmp_condtion.append(operatorSign);//outs() << "\nFound != comparison in Float\n\n";
				cmp_condtion.append(" ");
				st = getVariable_or_Value(FcmpInst->getOperand(0));
				cmp_condtion.append(st);
				cmp_condtion.append(" ");
				st = getVariable_or_Value(FcmpInst->getOperand(1));
				cmp_condtion.append(st);
				cmp_condtion.append(" ))");
			} else {
				cmp_condtion.append("(");
				cmp_condtion.append(operatorSign);
				cmp_condtion.append(" ");
				st = getVariable_or_Value(FcmpInst->getOperand(0));
				cmp_condtion.append(st);
				cmp_condtion.append(" ");
				st = getVariable_or_Value(FcmpInst->getOperand(1));
				cmp_condtion.append(st);
				cmp_condtion.append(" )");
			}

			cmp_str.append("(= ");
			cmp_str.append(FcmpInst->getName());
			cmp_str.append(" ");
			cmp_str.append(cmp_condtion);
			cmp_str.append(" )");

			//my_inst.append(cmp_str);

			//my_inst.append(cmp_condtion);
			//condition_data.conditionExp = cmp_condtion;
			//pushTop(condition_data);//Storing the cmp_conditional at the top of the stack data-structure
		}
		condition_data.conditionExp = cmp_condtion;
		workingVariable->pushTop(condition_data); //Storing the cmp_conditional at the top of the stack data-structure
		my_inst.append(cmp_str);
	//	llvm::outs()<<"condition: "<<cmp_str<<"\n";
	}

	//if (isa < BranchInst > (&instruction)) {
	if (isa<BranchInst>(&instruction)) {
/*Note:
 * 1) If False branch executes first in sequence, then the condition is negated and open bracket begins.
 * 2) Otherwise no Negation is printed and open bracket begins.
 * 3) If either one of the True/False branch is executed first and the other branch is executed immediately
 *    after it then, it has both if-then-else parts and so use "ite" as smt.
 * 4) After the end of one block with the last instruction as conditional br instruction. As soon as one of the
 *    True/False branch is executed and the next consecutive block executed is not the other branch/block of the conditional br instruction
 *    then, the condition does not have the else-part (i.e., it only has the then-part).
 * 5) Note that the printing of the sequences of '(' or ')' brackets are based on these br labels handled properly using some stack like structure.
 *
 */
		//Todo: handle printing of ite here with appropriate logic of either ite or => (implies)

//		llvm::outs() << "\n=========Branching statement========\n";
		BranchInst *branchInst = dyn_cast<BranchInst>(&instruction);
		struct brData brInfo;
		//llvm::outs() << branchInst->getOperand(0)->getName()<<"\n";
		//llvm::outs() << branchInst->getNumSuccessors()<<"\n";
		unsigned int brNumber=branchInst->getNumSuccessors();

		if (!(branchInst->isConditional())){	//if True has both True and False parts, otherwise on single label
			workingVariable->setPreviousBlockCondition(false);	//not a conditional br instruction
			//outs()<<"Single br label = " << branchInst->getOperand(0)->getName();
			workingVariable->setPrevious_br_label(branchInst->getOperand(0)->getName());
			ir_ssa->ssaLineNo++;
			ir_ssa->setInst(ir_ssa->ssaLineNo, " )");
			return;//Does not have both True and False parts
		} else{
			workingVariable->setPreviousBlockCondition(true);	//is a conditional br instruction
//			llvm::outs() << branchInst->getOperand(0)->getName() << "\n";//branch condition Name
//			llvm::outs() << branchInst->getOperand(1)->getName() << "\n"; // False-label
//			llvm::outs() << branchInst->getOperand(2)->getName()<<"\n";	//True-label
			brInfo.br_inBlock = workingVariable->getCurrentBlockName();
			brInfo.brConditionName = branchInst->getOperand(0)->getName();
			brInfo.falseLabel = branchInst->getOperand(1)->getName();
			brInfo.trueLabel = branchInst->getOperand(2)->getName();

			std::pair<unsigned int, std::string> TrueFalseData;

			TrueFalseData.first = ir_ssa->ssaLineNo + 1; // ssaLineNo+1 is going to be written for this ssa-string
			TrueFalseData.second = brInfo.trueLabel;

			workingVariable->setTrueFalseBlockStack(TrueFalseData);	//pushed TrueLabel in the Working stack

			TrueFalseData.second = brInfo.falseLabel;
			workingVariable->setTrueFalseBlockStack(TrueFalseData);	//pushed FalseLabel in the Working stack

			workingVariable->setBrStack(brInfo);	//USE THIS DATA-structure FOR PHI-NODE processing and NOT THE phiData data-structure

			my_inst.append("(ite "); //1) use if wisely to determine 'ite' or '=>'
									 //2) do some back-tracking to replace 'ite' with '=>'
			my_inst.append(brInfo.brConditionName);//Condition
			my_inst.append(" ");
		//	my_inst.append("(");	//Since it is a conditional instruction, so the start of the True/False block
		}
	}

	/*Binary Operation Instruction: Example add */
	if (isa<BinaryOperator>(instruction)) {
//		llvm::outs() << "Binary Operator";
		BinaryOperator *binaryOperator = cast<BinaryOperator>(&instruction);
		string st = "", varType;

		switch (binaryOperator->getOpcode()) {
		case llvm::Instruction::Add: {
			st = "(= ";
			my_inst.append(st);
			st = binaryOperator->getName();

			//Todo get the type... Need to verify with the program using IR
			if (binaryOperator->getType()->isIntegerTy())
				varType = "Int";
			else if (binaryOperator->getType()->isDoubleTy())
				varType = "Real";
			else if (binaryOperator->getType()->isFloatTy())
				varType = "Real";
			else
				varType = "Real";		//Todo:: For now converting to real
			ir_ssa->setVariable(st, varType);	//Found a variable declaration

			my_inst.append(st);
			my_inst.append(" ");

			Value *operand1 = binaryOperator->getOperand(0);
			Value *operand2 = binaryOperator->getOperand(1);
			st = "(+ ";
			my_inst.append(st);
			st = getVariable_or_Value(operand1);
			my_inst.append(st);
			my_inst.append(" ");

			st = getVariable_or_Value(operand2);
			my_inst.append(st);
			my_inst.append(" )");
			my_inst.append(")");
			break;
		}	//End of Case::Add
		case llvm::Instruction::Sub: {
			st = "(= ";
			my_inst.append(st);
			st = binaryOperator->getName();

			//Todo get the type... Need to verify with the program using IR
			if (binaryOperator->getType()->isIntegerTy())
				varType = "Int";
			else if (binaryOperator->getType()->isDoubleTy())
				varType = "Real";
			else if (binaryOperator->getType()->isFloatTy())
				varType = "Real";
			else
				varType = "Real";		//Todo:: For now converting to real
			ir_ssa->setVariable(st, varType);	//Found a variable declaration

			my_inst.append(st);
			my_inst.append(" ");

			Value *operand1 = binaryOperator->getOperand(0);
			Value *operand2 = binaryOperator->getOperand(1);
			st = "(- ";
			my_inst.append(st);
			st = getVariable_or_Value(operand1);
			my_inst.append(st);
			my_inst.append(" ");

			st = getVariable_or_Value(operand2);
			my_inst.append(st);
			my_inst.append(" )");
			my_inst.append(")");
			break;
		}	//End of Case::Sub
		case llvm::Instruction::Mul: {
			st = "(= ";
			my_inst.append(st);
			st = binaryOperator->getName();

			//Todo get the type... Need to verify with the program using IR
			if (binaryOperator->getType()->isIntegerTy())
				varType = "Int";
			else if (binaryOperator->getType()->isDoubleTy())
				varType = "Real";
			else if (binaryOperator->getType()->isFloatTy())
				varType = "Real";
			else
				varType = "Real";		//Todo:: For now converting to real
			ir_ssa->setVariable(st, varType);	//Found a variable declaration

			my_inst.append(st);
			my_inst.append(" ");

			Value *operand1 = binaryOperator->getOperand(0);
			Value *operand2 = binaryOperator->getOperand(1);
			st = "(* ";
			my_inst.append(st);
			st = getVariable_or_Value(operand1);
			my_inst.append(st);
			my_inst.append(" ");

			st = getVariable_or_Value(operand2);
			my_inst.append(st);
			my_inst.append(" )");
			my_inst.append(")");
			break;
		}	//End of Case::Mul
		case llvm::Instruction::FAdd: {
			st = "(= ";
			my_inst.append(st);
			st = binaryOperator->getName();

			//Todo get the type... Need to verify with the program using IR
			if (binaryOperator->getType()->isIntegerTy())
				varType = "Int";
			else if (binaryOperator->getType()->isDoubleTy())
				varType = "Real";
			else if (binaryOperator->getType()->isFloatTy())
				varType = "Real";
			else
				varType = "Real";		//Todo:: For now converting to real
			ir_ssa->setVariable(st, varType);	//Found a variable declaration

			my_inst.append(st);
			my_inst.append(" ");

			Value *operand1 = binaryOperator->getOperand(0);
			Value *operand2 = binaryOperator->getOperand(1);
			st = "(+ ";
			my_inst.append(st);
			st = getVariable_or_Value(operand1);
			my_inst.append(st);
			my_inst.append(" ");

			st = getVariable_or_Value(operand2);
			my_inst.append(st);
			my_inst.append(" )");
			my_inst.append(")");
			break;
		}	//End of Case::FAdd
		case llvm::Instruction::FSub: {
			st = "(= ";
			my_inst.append(st);
			st = binaryOperator->getName();

			//Todo get the type... Need to verify with the program using IR
			if (binaryOperator->getType()->isIntegerTy())
				varType = "Int";
			else if (binaryOperator->getType()->isDoubleTy())
				varType = "Real";
			else if (binaryOperator->getType()->isFloatTy())
				varType = "Real";
			else
				varType = "Real";		//Todo:: For now converting to real
			ir_ssa->setVariable(st, varType);	//Found a variable declaration

			my_inst.append(st);
			my_inst.append(" ");

			Value *operand1 = binaryOperator->getOperand(0);
			Value *operand2 = binaryOperator->getOperand(1);
			st = "(- ";
			my_inst.append(st);
			st = getVariable_or_Value(operand1);
			my_inst.append(st);
			my_inst.append(" ");

			st = getVariable_or_Value(operand2);
			my_inst.append(st);
			my_inst.append(" )");
			my_inst.append(")");
			break;
		}	//End of Case::FSub
		case llvm::Instruction::FMul: {
			st = "(= ";
			my_inst.append(st);
			st = binaryOperator->getName();

			//Todo get the type... Need to verify with the program using IR
			if (binaryOperator->getType()->isIntegerTy())
				varType = "Int";
			else if (binaryOperator->getType()->isDoubleTy())
				varType = "Real";
			else if (binaryOperator->getType()->isFloatTy())
				varType = "Real";
			else
				varType = "Real";		//Todo:: For now converting to real
			ir_ssa->setVariable(st, varType);	//Found a variable declaration

			my_inst.append(st);
			my_inst.append(" ");

			Value *operand1 = binaryOperator->getOperand(0);
			Value *operand2 = binaryOperator->getOperand(1);
			st = "(* ";
			my_inst.append(st);
			st = getVariable_or_Value(operand1);
			my_inst.append(st);
			my_inst.append(" ");

			st = getVariable_or_Value(operand2);
			my_inst.append(st);
			my_inst.append(" )");
			my_inst.append(")");
			break;
		}	//End of Case::FMul
		case llvm::Instruction::UDiv: {	//unsigned integer division
			st = "(= ";
			my_inst.append(st);
			st = binaryOperator->getName();

			//Todo get the type... Need to verify with the program using IR
			if (binaryOperator->getType()->isIntegerTy())
				varType = "Int";
			else if (binaryOperator->getType()->isDoubleTy())
				varType = "Real";
			else if (binaryOperator->getType()->isFloatTy())
				varType = "Real";
			else
				varType = "Real";		//Todo:: For now converting to real
			ir_ssa->setVariable(st, varType);	//Found a variable declaration

			my_inst.append(st);
			my_inst.append(" ");

			Value *operand1 = binaryOperator->getOperand(0);
			Value *operand2 = binaryOperator->getOperand(1);
			st = "(/ ";
			my_inst.append(st);
			st = getVariable_or_Value(operand1);
			my_inst.append(st);
			my_inst.append(" ");

			st = getVariable_or_Value(operand2);
			my_inst.append(st);
			my_inst.append(" )");
			my_inst.append(")");
			break;
		}	//End of Case::UDiv
		case llvm::Instruction::SDiv: {	//signed integer division
			st = "(= ";
			my_inst.append(st);
			st = binaryOperator->getName();

			//Todo get the type... Need to verify with the program using IR
			if (binaryOperator->getType()->isIntegerTy())
				varType = "Int";
			else if (binaryOperator->getType()->isDoubleTy())
				varType = "Real";
			else if (binaryOperator->getType()->isFloatTy())
				varType = "Real";
			else
				varType = "Real";		//Todo:: For now converting to real
			ir_ssa->setVariable(st, varType);	//Found a variable declaration

			my_inst.append(st);
			my_inst.append(" ");

			Value *operand1 = binaryOperator->getOperand(0);
			Value *operand2 = binaryOperator->getOperand(1);
			st = "(/ ";
			my_inst.append(st);
			st = getVariable_or_Value(operand1);
			my_inst.append(st);
			my_inst.append(" ");

			st = getVariable_or_Value(operand2);
			my_inst.append(st);
			my_inst.append(" )");
			my_inst.append(")");
			break;
		}	//End of Case::SDiv
		case llvm::Instruction::FDiv: {	//float-point division
			st = "(= ";
			my_inst.append(st);
			st = binaryOperator->getName();

			//Todo get the type... Need to verify with the program using IR
			if (binaryOperator->getType()->isIntegerTy())
				varType = "Int";
			else if (binaryOperator->getType()->isDoubleTy())
				varType = "Real";
			else if (binaryOperator->getType()->isFloatTy())
				varType = "Real";
			else
				varType = "Real";		//Todo:: For now converting to real
			ir_ssa->setVariable(st, varType);	//Found a variable declaration

			my_inst.append(st);
			my_inst.append(" ");

			Value *operand1 = binaryOperator->getOperand(0);
			Value *operand2 = binaryOperator->getOperand(1);
			st = "(/ ";
			my_inst.append(st);
			st = getVariable_or_Value(operand1);
			my_inst.append(st);
			my_inst.append(" ");

			st = getVariable_or_Value(operand2);
			my_inst.append(st);
			my_inst.append(" )");
			my_inst.append(")");
			break;
		}	//End of Case::FDiv

		case llvm::Instruction::And: {	//Handle phi-node for compound conditions
			//Indicate that it has found and operator for some comparison operators prior to this. So pop all for this block and do ANDing
			struct phiData phiNode1, phiNode2;
			phiNode1 = workingVariable->getTop();

			workingVariable->setAllCompoundConditions(phiNode1);


			string st = "(and ";
			//			outs()<<"Found stack size="<<getCondStack().size()<<"\n";
			//			outs()<<"phiNode1.condition="<<phiNode1.conditionExp<<"\n";
			//Todo:: re-think of NOT expanding/replacing the and/or compound conditions instead just use
			//condName and ANDing/ORing

			while (!(workingVariable->getCondStack().empty())) {
				phiNode2 = workingVariable->getTop();
				if (boost::iequals(phiNode1.blockLabel, phiNode2.blockLabel)) {
					workingVariable->setAllCompoundConditions(phiNode2);
					phiNode1.condName = binaryOperator->getName(); //replace new label/variable Name

					st.append(phiNode1.conditionExp);
					st.append(phiNode2.conditionExp);
					phiNode1.conditionExp = st;	//replace new expression with ANDing
					st = "";
				} else { //we have popTop but not matching So have to push in
					workingVariable->pushTop(phiNode2); //push back
				}
			}
			phiNode1.conditionExp.append(" )");
			workingVariable->pushTop(phiNode1); //Now replacing the new ANDed expression
			//			outs()<<"And = "<<phiNode1.conditionExp<<"\n";
			//			outs()<<"Found stack size="<<getCondStack().size()<<"\n";

			/*unsigned int totalConditions;
			 totalConditions = binaryOperator->getNumOperands();
			 std::cout<<"totalConditions="<<totalConditions<<std::endl;
			 outs()<<binaryOperator->getOperand(0)->getName()<<"\n";
			 outs()<<binaryOperator->getOperand(1)->getName()<<"\n";*/
			string andConditions = "";

			andConditions.append("(= ");
			andConditions.append(binaryOperator->getName());

			st = binaryOperator->getName();
			varType= "Bool";
			ir_ssa->setVariable(st, varType);	//Found a variable declaration


			andConditions.append(" (and ");
			andConditions.append(binaryOperator->getOperand(0)->getName());
			andConditions.append(" ");
			andConditions.append(binaryOperator->getOperand(1)->getName());
			andConditions.append(" ))");
			my_inst.append(andConditions);
			break;
		}
		case llvm::Instruction::Or: {	//Handle phi-node for compound conditions
			//Indicate that it has found and operator for some comparison operators prior to this. So pop all for this block and do ORing
			//			outs() << "OpcodeName()="<< binaryOperator->getOpcodeName() << "\n";
			//			outs() << "Operand(0)="<< binaryOperator->getOperand(0)->getName() << "\n";
			//			outs() << "Operand(1)="<< binaryOperator->getOperand(1)->getName() << "\n";
			struct phiData phiNode1, phiNode2;
			phiNode1 = workingVariable->getTop();
			workingVariable->setAllCompoundConditions(phiNode1);
			string st = "(or ";
			//			outs()<<"Found stack size="<<getCondStack().size()<<"\n";
			//			outs()<<"phiNode1.condition="<<phiNode1.conditionExp<<"\n";
			while (!(workingVariable->getCondStack().empty())) {
				phiNode2 = workingVariable->getTop();
				if (boost::iequals(phiNode1.blockLabel, phiNode2.blockLabel)) {
					workingVariable->setAllCompoundConditions(phiNode2);
					phiNode1.condName = binaryOperator->getName(); //replace new label/variable Name
					st.append(phiNode1.conditionExp);
					st.append(phiNode2.conditionExp);
					phiNode1.conditionExp = st;	//replace new expression with ANDing
					st = "";
				} else { //we have popTop but not matching So have to push in
					workingVariable->pushTop(phiNode2); //push back
					break;
				}
			}
			phiNode1.conditionExp.append(" )");
			workingVariable->pushTop(phiNode1); //Now replacing the new ANDed expression
			//			outs()<<"OR = "<<phiNode1.conditionExp<<"\n";

			string andConditions = "";

			andConditions.append("(= ");
			andConditions.append(binaryOperator->getName());
			st = binaryOperator->getName();

			varType= "Bool";
			ir_ssa->setVariable(st, varType);	//Found a variable declaration

			andConditions.append(" (or ");
			andConditions.append(binaryOperator->getOperand(0)->getName());
			andConditions.append(" ");
			andConditions.append(binaryOperator->getOperand(1)->getName());
			andConditions.append(" ))");
			my_inst.append(andConditions);
			break;
		}
		case llvm::Instruction::Xor: {//Handle phi-node for compound conditions
			//Note: since dReal does not support xor so, we use [x XOR y = {(x' ^ y) v (x ^ y')}]
			//(or (and (not x) y) (and x (not y)))

			//Indicate that it has found and operator for some comparison operators prior to this. So pop all for this block and do ORing
			struct phiData phiNode1, phiNode2;
			phiNode1 = workingVariable->getTop();
			workingVariable->setAllCompoundConditions(phiNode1);
			string st = "(xor ";
			//	outs()<<"Found stack size="<<getCondStack().size()<<"\n";
			//	outs()<<"phiNode1.condition="<<phiNode1.conditionExp<<"\n";
			while (!(workingVariable->getCondStack().empty())) {
				phiNode2 = workingVariable->getTop();
				if (boost::iequals(phiNode1.blockLabel, phiNode2.blockLabel)) {
					workingVariable->setAllCompoundConditions(phiNode2);
					phiNode1.condName = binaryOperator->getName(); //replace new label/variable Name
					st.append(phiNode1.conditionExp);
					st.append(phiNode2.conditionExp);
					phiNode1.conditionExp = st;	//replace new expression with XORing
					st = "";
				} else { //we have popTop but not matching So have to push in
					workingVariable->pushTop(phiNode2); //push back
					break;
				}
			}
			phiNode1.conditionExp.append(" )");
			workingVariable->pushTop(phiNode1); //Now replacing the new XORed expression
			//outs()<<"XOR = "<<phiNode1.conditionExp<<"\n";
			// z = [x XOR y] = {(x' ^ y) v (x ^ y')}
			//(= z (or (and (not x) y) (and x (not y))))
			string xorConditions = "";
			std::string ops1, ops2;
			xorConditions.append("(= ");
			xorConditions.append(binaryOperator->getName());	//z
			st = binaryOperator->getName();
			varType = "Bool";
			ir_ssa->setVariable(st, varType);	//Found a variable declaration

			Value *op1 = binaryOperator->getOperand(0);
			if (boost::iequals(getVariable_or_Value(op1), "0")) {
				ops1 = " false ";
			} else if (boost::iequals(getVariable_or_Value(op1), "-1")) {
				ops1 = " true ";
			} else {
				ops1 = binaryOperator->getOperand(0)->getName();
			}
			Value *op2 = binaryOperator->getOperand(1);
			if (boost::iequals(getVariable_or_Value(op2), "0")) {
				ops2 = " false ";
			} else if (boost::iequals(getVariable_or_Value(op2), "-1")) {
				ops2 = " true ";
			} else {
				ops2 = binaryOperator->getOperand(1)->getName();
			}
			xorConditions.append(" (or ( and ( not ");	//(or ( and ( not
			xorConditions.append(ops1);		//x
			xorConditions.append(" ) ");
			xorConditions.append(ops2);		//y
			xorConditions.append(" ) ( and ");
			xorConditions.append(ops1);		//x
			xorConditions.append(" ( not ");
			xorConditions.append(ops2);		//y
			xorConditions.append(" ) ) ) )");

			//outs()<<getVariable_or_Value(op1)<<" this is op1\n";

			my_inst.append(xorConditions);
			break;
		}
		case llvm::Instruction::Shl: {
			//Syntax: result = shl x , n // will yields result = x * (2^n): Prefix:  (= result (* x (^ 2 n)))
			//outs()<<"found shl instruction !!!\n";
			st = "(= ";
			my_inst.append(st);
			st = binaryOperator->getName();

			//Todo get the type... Need to verify with the program using IR
			if (binaryOperator->getType()->isIntegerTy())
				varType = "Int";
			else if (binaryOperator->getType()->isDoubleTy())
				varType = "Real";
			else if (binaryOperator->getType()->isFloatTy())
				varType = "Real";
			else
				varType = "Real";		//Todo:: For now converting to real
			ir_ssa->setVariable(st, varType);	//Found a variable declaration

			my_inst.append(st);
			my_inst.append(" ");

			Value *operand1 = binaryOperator->getOperand(0);
			Value *operand2 = binaryOperator->getOperand(1);
			st = "(* ";
			my_inst.append(st);
			st = getVariable_or_Value(operand1);
			my_inst.append(st);
			my_inst.append(" ");

			st = "(^ 2 ";
			my_inst.append(st);

			st = getVariable_or_Value(operand2);
			my_inst.append(st);
			my_inst.append(" )))");
			break;
		}
		}	//End of Switch

	}	//End of BinaryOperator

	if (isa<CallInst>(instruction)) {

	}

	/*
	 * Logic as per my understanding (so far): when phi-node is found
	 *   The working stack data structure is searched for the incoming blockName, if found then get the conditionExp
	 *   and use as the phi-condtion for assignment. If the stack does not contain the blockName then it indicate that
	 *   the incoming block has no conditional instruction thus, only a simple assignment is required in this case.
	 *   Note here the stack is not deleted (as I guess not need in this algorithm)
	 */
	if (isa<PHINode>(instruction)) {
		string st = "", varType;
		PHINode *phiNode = cast<PHINode>(&instruction);
		unsigned numberOfIncomingBlock = phiNode->getNumIncomingValues();
		string currBlockName=workingVariable->getCurrentBlockName();
		std::list<blockOrdering> blockSequence =  workingVariable->getBlockSequence();
		std::list<brData> brInfo = workingVariable->getBrStack();

		for (int i = 0; i < numberOfIncomingBlock; i++) {
			st = phiNode->getName();//New variable/label name

			if (phiNode->getType()->isIntegerTy())
				varType = "Int";
			else if (phiNode->getType()->isDoubleTy())
				varType = "Real";
			else if (phiNode->getType()->isFloatTy())
				varType = "Real";
			else
				varType = "Real";		//Todo:: For now converting to real
			ir_ssa->setVariable(st, varType);

			string cond = "", incomingBlockLabel;
			incomingBlockLabel = phiNode->getIncomingBlock(i)->getName();
			bool found;// = searchBlock(getCondStack(), incomingBlockLabel, cond);//return cond varName

			found = searchPhiCondition(blockSequence, brInfo, currBlockName, incomingBlockLabel, cond); //Correct implementation

			if (found){ //Need to print implication (=> (condition)(assignment/true-part)
				my_inst.append("(=> ");
				my_inst.append(cond);
				my_inst.append(" (= ");
				my_inst.append(st);
				my_inst.append(" ");
				my_inst.append(getVariable_or_Value(phiNode->getOperand(i)));
				my_inst.append(" ))");
			}//At the moment I do not see other possible condition
			/*else{	//Not found indicate just need to have simple assignment
				my_inst.append(" (= ");
				my_inst.append(st);
				my_inst.append(" ");
				my_inst.append(getVariable_or_Value(phiNode->getOperand(i)));
				my_inst.append(")");
			}*/

			if (i!=(numberOfIncomingBlock-1))
				my_inst.append("\n");
		}
	}

	if (my_inst.compare("") != 0){
		ir_ssa->ssaLineNo++;
		ir_ssa->setInst(ir_ssa->ssaLineNo, my_inst);
		//all_insts.push_back(my_inst);
	}
}

