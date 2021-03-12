# Outputs

The output of execution of the `libTestPass.so` pass is dumped into a file that can use it for analysis of an input controller program (namely one of the C programs from the benchmarks folder).

The format of the output file is, line by line:

1. Three numbers separated by ` `s.
	* The first number is the size of the total intermediate variables used in the entire SSA process.
	* The second number is the size of the total input variables present in the controller program (which is passed as an input argument with the reference structure data type named `INPUT_VAL`).
 	* The third number is the size of the total output variables present in the controller program (which is passed as an output argument with the reference structure data type named `RETURN_VAL`).
2. A list of values, detailed in the form `DataType VariableName`.
	* The order of occurrence of these variables are (1) intermediate variables, (2) input variables, then (3) output variables.
3. Following the variables (name and data-type pairs), the rest of the lines are the actual SSA statements.