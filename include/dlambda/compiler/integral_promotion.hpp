#ifndef DLAMBDA_COMPILER_INTEGRAL_PROMOTION_HPP
#define DLAMBDA_COMPILER_INTEGRAL_PROMOTION_HPP

#include <utility>
#include <llvm/IR/LLVMContext.h>
#include <dlambda/type.hpp>
#include <dlambda/compiler/ir_builder.hpp>
#include <dlambda/compiler/expression.hpp>

namespace dlambda {
  namespace compiler {
    expression integral_promotion(
      const std::shared_ptr< llvm::LLVMContext > &context,
      const std::shared_ptr< ir_builder_t > &ir_builder,
      const expression &value
    );
  }
}

#endif

