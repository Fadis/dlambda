#ifndef DLAMBDA_COMPILER_NODE_BITWISE_XOR_HPP
#define DLAMBDA_COMPILER_NODE_BITWISE_XOR_HPP

#include <memory>
#include <llvm/IR/Value.h>
#include <llvm/IR/IRBuilder.h>

#include <dlambda/compiler/expression.hpp>
#include <dlambda/compiler/ir_builder.hpp>

namespace dlambda {
  namespace compiler {
    expression bitwise_xor(
      const std::shared_ptr< llvm::LLVMContext > &context,
      const std::shared_ptr< ir_builder_t > &ir_builder,
      const expression &left,
      const expression &right
    );
  }
}

#endif

