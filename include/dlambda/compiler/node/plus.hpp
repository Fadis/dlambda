#ifndef DLAMBDA_COMPILER_NODE_PLUS_HPP
#define DLAMBDA_COMPILER_NODE_PLUS_HPP

#include <memory>
#include <llvm/IR/Value.h>
#include <llvm/IR/IRBuilder.h>

#include <dlambda/compiler/expression.hpp>
#include <dlambda/compiler/ir_builder.hpp>

namespace dlambda {
  namespace compiler {
    expression plus(
      const std::shared_ptr< llvm::LLVMContext > &context,
      const std::shared_ptr< ir_builder_t > &ir_builder,
      const expression &left,
      const expression &right
    );
  }
}

#endif

