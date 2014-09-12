#ifndef DLAMBDA_COMPILER_NODE_BITWISE_XOR_ASSIGN_HPP
#define DLAMBDA_COMPILER_NODE_BITWISE_XOR_ASSIGN_HPP

#include <memory>
#include <llvm/IR/Value.h>
#include <llvm/IR/IRBuilder.h>

#include <dlambda/compiler/node/bitwise_xor.hpp>
#include <dlambda/compiler/node/assign.hpp>
#include <dlambda/compiler/expression.hpp>
#include <dlambda/compiler/ir_builder.hpp>

namespace dlambda {
  namespace compiler {
    expression bitwise_xor_assign(
      const std::shared_ptr< llvm::LLVMContext > &context,
      const std::shared_ptr< ir_builder_t > &ir_builder,
      const expression &left,
      const expression &right
    ) {
      return assign( context, ir_builder, left, bitwise_xor( context, ir_builder, left, right ) );
    }
  }
}

#endif

