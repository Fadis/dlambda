#ifndef DLAMBDA_COMPILER_NODE_REINTERPRET_CAST_HPP
#define DLAMBDA_COMPILER_NODE_REINTERPRET_CAST_HPP

#include <memory>
#include <llvm/IR/LLVMContext.h>
#include <dlambda/type.hpp>
#include <dlambda/compiler/ir_builder.hpp>
#include <dlambda/compiler/expression.hpp>

namespace dlambda {
  namespace compiler {
    expression reinterpret_cast_(
      const std::shared_ptr< llvm::LLVMContext > &context,
      const std::shared_ptr< ir_builder_t > &ir_builder,
      const type cast_to,
      const expression &value
    );
  }
}

#endif

