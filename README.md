# llvm-pass-controllerProgram
********************************************************************************************
Indian Institute of Technology Kharagpur, India,
Formal Methods Lab,
Project Title: "FMSAFE: A Networked Centre for Formal Methods in Validation and Certification Procedures for Safety Critical ICT Systems"

Funded by IMPRINT, a MHRD supported Pan-IIT + IISc joint initiative for addressing major science and engineering challenges that are relevant in the national context.


Copyright (c) 2018 Formal Methods Lab, Indian Institute of Technology Kharagpur, India.

Developer by:
               Amit Gurung
********************************************************************************************


It is developed using clang/LLVM version 7.0.0.
This pass is written in order to generate a file named "fmsafe_Out.ssa" so that the FMSafe project can use it for analysis of the input controller program.

How to build: 
	
	To obtain "libTestPass.so" the controller program pass. Download the source code of the project "llvm-pass-moduleTest" (note this project is written in clang-llvm using LLVM version 7.0.0. The project takes a C/C++ program (we call this program the controller-program) emits the LLVM's IR and transforms it into a high-level SSA form which the project FMSafe can understand).  
If the Makefile of the project "llvm-pass-moduleTest" does not exists, then, use the cmake command in the sequence shown below:

    $ cd llvm-pass-moduleTest
    $ mkdir build
    $ cd build
    $ cmake ..
    $ make
    $ cd ..
 
If the Makefile is generated then just use the make command

	$ cd llvm-pass-moduleTest
	$ make


How to Run:

	$ clang -O1 -g -Xclang -emit-llvm -c benchmarks/someProgram.c -o benchmarks/test.bc
	$ opt -O1 -instnamer -mem2reg -simplifycfg -loops -lcssa -loop-simplify -loop-rotate -loop-unroll -unroll-count=3 -unroll-allow-partial -load lib/libTestPass.so -ssa benchmarks/test.bc -view-cfg  -o benchmarks/test
	$ lli benchmarks/test		//if we want to execute the binary file 

Here the folder benchmarks and lib is assumed to contain the program file "someProgram.c" and the "libTestPass.so" library pass. The output of this libTestPass.so pass invocation is an output file name "fmsafe_Out.ssa". The format and details contained in the file are described below.


Format of the Output file:

 * The output of execution of "libTestPass.so" pass is dumped into a file so that our FMSafe project can use it for analysis of the input controller program (someProgram.c above).
 * The format of the output file is:
 * 	First line contain three(3) numbers separated by a space.
 * 	The first number is the size of the total intermediate variables used in the entire SSA process.
 * 	The second number is the size of the total input variables present in the controller program which is passed
 * 		as input arguments with the reference structure data type named "INPUT_VAL".
 * 	The third number is the size of the total output variables present in the controller program which is passed
 * 		as output arguments with the reference structure data type named "RETURN_VAL".
 
 * Next contains the detailed/values described above. The syntax is:  "DataType  VariableName".
 * 	The order of occurrence of these variables are "intermediate variables", "input variables" followed
 * 		by the "output variables".
 
 * Following the variables name and their data type, the rest of the lines are the actual SSA statements.




