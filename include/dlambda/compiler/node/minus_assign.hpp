#ifndef DLAMBDA_COMPILER_NODE_MINUS_ASSIGN_HPP
#define DLAMBDA_COMPILER_NODE_MINUS_ASSIGN_HPP

#include <memory>
#include <llvm/IR/Value.h>
#include <llvm/IR/IRBuilder.h>

#include <dlambda/compiler/node/minus.hpp>
#include <dlambda/compiler/node/assign.hpp>
#include <dlambda/compiler/expression.hpp>
#include <dlambda/compiler/ir_builder.hpp>

namespace dlambda {
  namespace compiler {
    expression minus_assign(
      const std::shared_ptr< llvm::LLVMContext > &context,
      const std::shared_ptr< ir_builder_t > &ir_builder,
      const expression &left,
      const expression &right
    ) {
      return assign( context, ir_builder, left, minus( context, ir_builder, left, right ) );
    }
  }
}

#endif

