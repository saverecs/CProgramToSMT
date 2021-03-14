CL=llvm/build/bin/clang
CXL=llvm/build/bin/clang++
CI=llvm/build/include/
CXI=llvm/build/include/
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
		CC=$(CL) CXX=$(CXL) C_INCLUDE_PATH=$(CI) CPLUS_INCLUDE_PATH=$(CXI) cmake .. && \
		CC=$(CL) CXX=$(CXL) C_INCLUDE_PATH=$(CI) CPLUS_INCLUDE_PATH=$(CXI) make

deps:
	sudo apt-get install -y swig libedit-dev

test:
	$(CL) -O1 -g -Xclang -emit-llvm -c $(PREF)/benchmarks/test.c \
		                            -o $(PREF)/benchmarks/test.bc
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
		       -aa $(PREF)/benchmarks/test.bc \
		       -view-cfg -o $(PREF)/benchmarks/test
	$(LLI) benchmarks/test