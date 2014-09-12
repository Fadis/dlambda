#ifndef DLAMBDA_COMPILER_IR_BUILDER_HPP
#define DLAMBDA_COMPILER_IR_BUILDER_HPP

#include <llvm/IR/IRBuilder.h>

namespace dlambda {
  namespace compiler {
    typedef llvm::IRBuilder< true, llvm::ConstantFolder, llvm::IRBuilderDefaultInserter< true > > ir_builder_t;
  }
}

#endif

