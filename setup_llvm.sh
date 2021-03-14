LLVM_VERSION=7.0.0
# wget "https://github.com/llvm/llvm-project/archive/llvmorg-${LLVM_VERSION}.tar.gz"
tar zxf llvmorg-${LLVM_VERSION}.tar.gz
# rm llvmorg-${LLVM_VERSION}.tar.gz
mv llvm-project-llvmorg-${LLVM_VERSION} llvm
cd llvm
mkdir build
cd build
cmake ../llvm \
    -G Ninja \
    -DLLVM_ENABLE_PROJECTS="clang;clang-tools-extra;lld;lldb" \
    -DCMAKE_BUILD_TYPE=Debug
ninja