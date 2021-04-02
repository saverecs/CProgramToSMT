CL=llvm/build/bin/clang
CXL=llvm/build/bin/clang++
CI=llvm/llvm/include/
CXI=llvm/llvm/include/
OPT=llvm/build/bin/opt
LLI=llvm/build/bin/lli
PREF=llvm-pass-moduleTest

clean:
	git clean -f -X --exclude="\!llvm"
	- rm -rf llvm-pass-moduleTest/src/build

build:
	cd llvm-pass-moduleTest/src && \
		mkdir build             && \
		cd build                && \
		LLVM_DIR=../../../llvm cmake .. && \
		LLVM_DIR=../../../llvm make

deps:
	sudo apt-get install -y swig libedit-dev xdot

test:
	$(CL) -O1 -g -Xclang -emit-llvm -c $(PREF)/benchmarks/thermostat/thermostat.c \
		                            -o $(PREF)/benchmarks/thermostat/thermostat.bc
	$(OPT) -O1 -instnamer \
		       -mem2reg \
		       -simplifycfg \
		       -loops \
		       -lcssa \
		       -loop-simplify \
		       -loop-rotate \
		       -loop-unroll \
		       -unroll-count=3 \
		       -unroll-allow-partial \
		       -load $(PREF)/src/build/libTestPass.so \
		       -aa $(PREF)/benchmarks/thermostat/thermostat.bc \
		       -view-cfg -o $(PREF)/benchmarks/thermostat/thermostat
	
	$(LLI) $(PREF)/benchmarks/thermostat/thermostat