/*
 * optIRssa.cpp
 *
 *  Created on: 17-Jul-2018
 *      Author: amit
 */

#include <optIRssa.h>




void optIRssa::optimizeInstCombine(){
	createDefinitionDataStructure(all_insts);	//DDS created



	optimizeProcess();

}

//DDS
void optIRssa::createDefinitionDataStructure(std::list< std::pair<unsigned int, std::string> > SSA_data){
	//std::list<std::pair<unsigned int, std::string> > all_OldSSA;
	//all_OldSSA = this->all_insts;

	DDS.clear();	//initially empty

	for (std::list< std::pair<unsigned int, std::string> >::iterator it=SSA_data.begin(); it != SSA_data.end();it++){
		string ssa_str = (*it).second;
		//std::cout<<"Each OLD SSA = "<<ssa_str<<std::endl;
		definitionDS defDS;

		boost::char_separator<char> sep(" "); //, and " " a single space is the separator
		boost::tokenizer< boost::char_separator<char> > tok(ssa_str, sep);

		boost::tokenizer< boost::char_separator<char> >::iterator tokPtr;
		tokPtr = tok.begin();
		string OpenBracketEqual;
		OpenBracketEqual = (*tokPtr); //First token
		// ===== 1st token possibilities are "(=", "(=>", "(and", ")" "(ite" ==========
		if (boost::iequals(OpenBracketEqual,"(=")){
			tokPtr++;	//get the next token which is the variable for searchReplace operation
			defDS.lineNo = (*it).first;
			defDS.varName = (*tokPtr);	//The variable name for storing in DDS
			//std::cout<<"varName = " << defDS.varName <<std::endl;
			// ===== Now find the position of varName in the OldSSA string and get substr upto last bracket (excluding it) ======
			unsigned int varlen = defDS.varName.length();
			unsigned int varPosition;	//position of the variable in the OldSSA string
			varPosition = ssa_str.find(defDS.varName);
			//std::cout<<"varName length = " << defDS.varName.length() <<" position = "<<varPosition <<std::endl;
			//remaining string
			unsigned int start_pos = varPosition + varlen;	//varPosition gives the sarting char position
			//std::cout<<"substr start_pos = " << start_pos <<std::endl;
			string rest_str = ssa_str.substr(start_pos);//the 3rd argument is default indicates the rest of the strings including last closing bracket
			//std::cout<<"rest_string = " << rest_str <<std::endl;
			string rest_str2 = rest_str.substr(0,rest_str.length()-1);//excluding last closing bracket
			//std::cout<<"RHS expression = " << rest_str2 <<std::endl;
			defDS.varRHSexpression=rest_str2;	//string without the last closing bracket
			DDS.push_back(defDS);	//Maintaining the Definition Data Structure
		}

		if (boost::iequals(OpenBracketEqual,"(ite")){
			tokPtr++;	//getting the conditional variable and do the searchReplace operation
			std::cout << "Found ite SSA"<<std::endl;
		}

		if (boost::iequals(OpenBracketEqual,"(=>")){
			std::cout << "\t\tPossible Found conversion of phi-node. So multiple line of (=> may follow..."<<std::endl;
			std::cout << "\t\tNeed to think on further optimization on phi-node...."<<std::endl;
		}
	}
	//Delete the last element from DDS because that will be the first processing element in SSA
		std::list<definitionDS>::iterator last_dds = DDS.end();
		last_dds--;
		//std::cout <<"Last DDS variable ="<< (*last_dds).varName<<std::endl;
		DDS.pop_back();	//remove the last element
	/*
		std::list<definitionDS>::iterator last_dds2 = DDS.end();
		last_dds2--;
		std::cout <<"Now Last DDS variable is ="<< (*last_dds2).varName<<std::endl;
	*/

		std::cout<<"Displaying DDS\n";
		for (std::list<definitionDS>::iterator it = DDS.begin();it!= DDS.end();it++){
			std::cout<< (*it).varName <<" = "<<(*it).varRHSexpression<<std::endl;
		}



}

void optIRssa::optimizeProcess(){
	bool flagVarDefinitionFound=true;

	while (flagVarDefinitionFound){	//terminate when replacement logic is not possible to apply anymore
		flagVarDefinitionFound = false;	//initialize to false
		std::string varRHSexpression, varNameFound;
		unsigned int foundAtLineNo, definition_lineNo;
		// **==== iterating from the end and find out variable having definition and replaceable for optimization ====**
		for (std::list< std::pair<unsigned int, std::string> >::reverse_iterator rit = SSA.rbegin(); rit != SSA.rend(); ++rit){

			string ssa_str = (*rit).second;
			//Possible ssa_str are (especially first two tokens):
			//"(= varName", "(=> varName", "(= (not condName", "(ite condName", " )", "(=> (not varName"
			//"(= varName (>"

			boost::char_separator<char> sep(" "); //, and " " a single space is the separator
			boost::tokenizer<boost::char_separator<char> > tok(ssa_str, sep);
			boost::tokenizer<boost::char_separator<char> >::iterator tokPtr;
			tokPtr = tok.begin();
			bool starting_token=true;

			while (tokPtr != tok.end()){	//searching in the string of ssa_str
				string mytoken;
				mytoken = (*tokPtr); //First token
			//	std::cout<<"mytoken="<<mytoken<<std::endl;
				// === 1st token possibilities are "(=", "(=>", "(and", ")", "(ite" ===

				if (boost::iequals(mytoken, "(=")) {
				// if "(=" found at the start, then ssa_str will have varName followed by RHSexpression,
				// so skip this
					if (starting_token){
						tokPtr++;	//this will give varName,
						starting_token = false;
					}
				}
				if (boost::iequals(mytoken, ")")) {
					//We may not require to do anything the last tokPtr++; will do the job
				}
				if (boost::iequals(mytoken, "(and")) {
					//We may not require to do anything the last tokPtr++; will do the job
				}

				//match if this mytoken has definition in DDS. If so get the RHSexpression for replacement

				flagVarDefinitionFound = isDefinitionExist(mytoken, varRHSexpression,definition_lineNo);
				if(flagVarDefinitionFound){
					varNameFound = mytoken;
					foundAtLineNo = (*rit).first;	//this will help to directly start Replacement Logic directly from this SSA
					std::cout<<"Found token at SSALineNo="<<foundAtLineNo<<std::endl;
					std::cout<<"SSA = "<<ssa_str<<std::endl;

					break;	//exit from the while-loop for string search
				}
				starting_token = false;	//as soon as the first token of the ssa_str is traversed
				tokPtr++;	//THIS does the search for next token
			}//End of While for each SSA string
			if (flagVarDefinitionFound){
				break;	//exit from iterating the next SSA
			}
		}//iterating the next SSA for search variable definition

		//when found apply Replacement Logic using varRHSexpression as replacing expression for the found varName
		if (flagVarDefinitionFound){
			std::cout<<"===========Found token Apply Replacement Logic!!==========="<<std::endl;
			bool updateDone=false;

			//std::list< std::pair<unsigned int, std::string> >::reverse_iterator rit = SSA.rbegin();
			auto rit = SSA.end();	//https://stackoverflow.com/questions/1830158/how-to-call-erase-with-a-reverse-iterator
			//while (rit != SSA.rend()){
			while (rit != SSA.begin()){	//Todo need to verify if the begin/first element get access
				rit--;	//decrement or increment from reverse. initially done to reach last element from .end()
				unsigned int lineNo = (*rit).first;//lineNo on which replacement to be applied, needed to reference DDS.lineNo
				string ssa_str = (*rit).second;	//string on which replacement to be applied
				std::cout <<"ssa_str is ="<<ssa_str<< std::endl;
				if (definition_lineNo==lineNo){	//if while-loop reached till definition_lineNo then delete this element from the list of DDS and SSA and STOP

					std::cout << "Found Starting point of definition:" << varNameFound << std::endl;
					//delete SSA and update DDS using SSA
					rit = SSA.erase(rit);	//definition in SSA deleted
					createDefinitionDataStructure(SSA);	//creating DDS using the current modified SSA

					break;
				}

				//Now find the varNameFound get its position and do a careful selective replace
				boost::char_separator<char> sep(" "," "); //, and " " a single space is the separator and wanted to also display it to count the number of spaces
				boost::tokenizer<boost::char_separator<char> > tok(ssa_str, sep);
				boost::tokenizer<boost::char_separator<char> >::iterator tokPtr;
				tokPtr = tok.begin();
				unsigned int searchStrLength=0, tokenLength;
				while (tokPtr != tok.end()){	//searching in the string of ssa_str
					string mytoken;
					mytoken = (*tokPtr);
					//std::cout<<"token="<<mytoken<<std::endl;
					tokenLength =mytoken.length()+1;	//assuming only 1 space
					searchStrLength = searchStrLength + tokenLength;
					//std::cout<<"searchStrLength="<<searchStrLength<<std::endl;
					updateDone = false;
					if (boost::iequals(mytoken, varNameFound)){
		/*
		 * std::string str="this is a test string.";
		  str.replace(9,5,str2);          // "this is an example string." (1)
		  9 is the starting position (0-based indexing)
		  5 is the number of characters to be replaced by str2
		  Also Note:
		  size_t find (const string& str, size_t pos = 0) const;
		  When pos is specified, the search only includes characters at or after position pos, ignoring any possible occurrences that include characters before pos.
		 */
						searchStrLength = searchStrLength - tokenLength;
						std::cout<<"Found matching token="<<mytoken<<"  searchStrLength="<<searchStrLength<<std::endl;
						unsigned int varPosition, varlength;	//position of the variable in the OldSSA string
						varPosition = ssa_str.find(mytoken, searchStrLength);
						std::cout<<"start_pos = " << varPosition <<std::endl;
						varlength = mytoken.length();
						string modified_ssa = ssa_str;
						modified_ssa.replace(varPosition,varlength,varRHSexpression);

						/*
						 * modified_DDS_atLine(foundAtLineNo, varNameFound, varRHSexpression): is not correct implementation
						 * the definition in DDS should also be searched and replaced in the same way as SSA as above
						 *Alternative approach is, Whenever SSA is updated/modified
						 *	use this updated SSA and create DDS as before
						 */

						createDefinitionDataStructure(SSA);	//creating DDS using the current modified SSA

						std::cout<<"modified_ssa = " << modified_ssa <<std::endl;
						(*rit).second = modified_ssa;
						updateDone = true;
						searchStrLength =0;	//reset the length
					}
					if (!updateDone)	//if not updateDone, then increase tokPtr for next token
						tokPtr++;			//otherwise search further the ssa_str
					else
						break;	//need to search on this same modified ssa_str for more occurance
				}

				if (updateDone){	//need to search on this same modified ssa_str for more occurance
					rit++;	//so that we access the same ssa_str
				}

				//rit++;	//increment from reverse
			}
		//	break;	//when Replacement Logic correct remove this break
		}
	}//End of While
}

void optIRssa::modified_DDS_atLine(unsigned int foundAtLineNo, std::string varNameFound, std::string varRHSexpression){

	for (std::list<definitionDS>::iterator it= DDS.begin(); it != DDS.end(); it++){
		if (foundAtLineNo == (*it).lineNo){
			string modified_varRHSexp = (*it).varRHSexpression;
			boost::replace_all(modified_varRHSexp, varNameFound, varRHSexpression);
			//replace_all() replaces all occurrence at once// todo:: verify _all and use replace_first if required
			std::cout<<"modified_varRHSexp = " << modified_varRHSexp <<std::endl;
			(*it).varRHSexpression = modified_varRHSexp;	//replace the modified DDS
			break;
		}
	}
}

void optIRssa::delete_DDS_atLine(unsigned int foundAtLineNo){
	for (std::list<definitionDS>::iterator it= DDS.begin(); it != DDS.end(); it++){
		if (foundAtLineNo == (*it).lineNo){
			string modified_varRHSexp = (*it).varRHSexpression;
			std::cout<<"Deleting Definition DDS.varName="<<(*it).varName<<std::endl;
			it = DDS.erase(it);	//https://stackoverflow.com/questions/596162/can-you-remove-elements-from-a-stdlist-while-iterating-through-it
			break;
		}
	}
}

bool optIRssa::isDefinitionExist(std::string mytoken, std::string& varRHSexpression, unsigned int& definition_lineNo){

	for (std::list<definitionDS>::iterator it= DDS.begin(); it != DDS.end(); it++){
		if (boost::iequals(mytoken, (*it).varName)){
			varRHSexpression = (*it).varRHSexpression;
			definition_lineNo = (*it).lineNo;
			return true;
		}
	}
	return false;
}

const std::list<std::pair<unsigned int, std::string> >& optIRssa::getAllInsts() const {
	return all_insts;
}

void optIRssa::setAllInsts(
		const std::list<std::pair<unsigned int, std::string> >& allInsts) {
	all_insts = allInsts;
	setSsa(all_insts);	//also copy to SSA
}

const std::list<definitionDS>& optIRssa::getDds() const {
	return DDS;
}

void optIRssa::setDds(const std::list<definitionDS>& dds) {
	DDS = dds;
}

const std::list<struct variable>& optIRssa::getInputVariables() const {
	return inputVariables;
}

void optIRssa::setInputVariables(
		const std::list<struct variable>& inputVariables) {
	this->inputVariables = inputVariables;
}

const std::list<std::pair<std::string, std::string> >& optIRssa::getIntermediateVariables() const {
	return intermediateVariables;
}

void optIRssa::setIntermediateVariables(
		const std::list<std::pair<std::string, std::string> >& intermediateVariables) {
	this->intermediateVariables = intermediateVariables;
}

const std::list<struct variable>& optIRssa::getOutputVariables() const {
	return outputVariables;
}

void optIRssa::setOutputVariables(
		const std::list<struct variable>& outputVariables) {
	this->outputVariables = outputVariables;
}

const std::list<std::pair<unsigned int, std::string> >& optIRssa::getSsa() const {
	return SSA;
}

void optIRssa::setSsa(
		const std::list<std::pair<unsigned int, std::string> >& ssa) {
	SSA = ssa;
}







/*
//Now find the varNameFound get its position and do a careful selective replace
std::cout<<"Found the same SSA="<<foundAtLineNo<<std::endl;
boost::char_separator<char> sep(" "); //, and " " a single space is the separator
boost::tokenizer<boost::char_separator<char> > tok(ssa_str, sep);
boost::tokenizer<boost::char_separator<char> >::iterator tokPtr;
tokPtr = tok.begin();

while (tokPtr != tok.end()){	//searching in the string of ssa_str
	string mytoken;
	mytoken = (*tokPtr);

	if (boost::iequals(mytoken, varNameFound)){
		std::cout<<"Found matching token="<<mytoken<<std::endl;
		unsigned int varPosition;	//position of the variable in the OldSSA string
		//varPosition = ssa_str.find(mytoken);
		//std::cout<<"start_pos = " << varPosition <<std::endl;
		string modified_ssa = ssa_str;

		boost::replace_first(modified_ssa, mytoken, varRHSexpression);
		//replace_all() replaces all occurrence at once// todo:: verify _all and use replace_first if required

		std::cout<<"modified_ssa = " << modified_ssa <<std::endl;

		//Todo found the variable now have to replace it and update SSA

		(*rit).second = modified_ssa;


	}

	tokPtr++;
}
*/


/*
 * 	std::size_t foundVarName;
	foundVarName = ssa_str.find(varNameFound);
	if (foundVarName != std::string::npos){	//indicate found
		string ssa_str = (*rit).second;
		string modified_ssa = ssa_str;
		boost::replace_all(modified_ssa, varNameFound, varRHSexpression);
		//replace_all() replaces all occurrence at once// todo:: verify _all and use replace_first if required
		std::cout << "modified_ssa = " << modified_ssa << std::endl;
		(*rit).second = modified_ssa;	//replace or update the SSA
		modified_DDS_atLine(foundAtLineNo, varNameFound, varRHSexpression);//modify DDS for varNameFound with varRHSexpression
		updateDone = true;
	}
 */
