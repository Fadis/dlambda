#ifndef DLAMBDA_COMPILER_NODE_COMMA_HPP
#define DLAMBDA_COMPILER_NODE_COMMA_HPP

#include <memory>
#include <llvm/IR/Value.h>
#include <llvm/IR/IRBuilder.h>

#include <dlambda/compiler/node/plus.hpp>
#include <dlambda/compiler/node/assign.hpp>
#include <dlambda/compiler/expression.hpp>
#include <dlambda/compiler/ir_builder.hpp>

namespace dlambda {
  namespace compiler {
    expression comma(
      const std::shared_ptr< llvm::LLVMContext > &,
      const std::shared_ptr< ir_builder_t > &,
      const expression &,
      const expression &right
    ) {
      return right;
    }
  }
}

#endif

