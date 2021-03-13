LLVM7LOC=~/tools/clang_llvm/clang+llvm-7.0.0-x86_64-linux-gnu-ubuntu-16.04/
# NOTE: You will need to replace LLVM7LOC with the path to your local
#       download of LLVM 7.0.0, which can be found here:
#       https://releases.llvm.org/

CL=$(LLVM7LOC)bin/clang
CXL=$(LLVM7LOC)bin/clang++
CI=$(LLVM7LOC)include/
CXI=$(LLVM7LOC)include/

clean:
	git clean -f -X
	- rm -rf llvm-pass-moduleTest/src/build

build:
	cd llvm-pass-moduleTest/src && \
		mkdir build             && \
		cd build                && \
		CC=$(CL) CXX=$(CXL) C_INCLUDE_PATH=$(CI) CPLUS_INCLUDE_PATH=$(CXI) cmake .. && \
		CC=$(CL) CXX=$(CXL) C_INCLUDE_PATH=$(CI) CPLUS_INCLUDE_PATH=$(CXI) make

deps:
	sudo apt-get install -y clang cmake ninja-build