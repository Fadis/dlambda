#ifndef DLAMBDA_COMPILER_USUAL_ARITHMETIC_CONVERSION_HPP
#define DLAMBDA_COMPILER_USUAL_ARITHMETIC_CONVERSION_HPP

#include <utility>
#include <llvm/IR/LLVMContext.h>
#include <dlambda/type.hpp>
#include <dlambda/compiler/ir_builder.hpp>
#include <dlambda/compiler/expression.hpp>

namespace dlambda {
  namespace compiler {
    std::pair< expression, expression > usual_arithmetic_conversion(
      const std::shared_ptr< llvm::LLVMContext > &context,
      const std::shared_ptr< ir_builder_t > &ir_builder,
      const expression &left, const expression &right
    );
  }
}

#endif

