#ifndef DLAMBDA_COMPILER_NODE_REMOVE_REFERENCE_HPP
#define DLAMBDA_COMPILER_NODE_REMOVE_REFERENCE_HPP

#include <memory>

#include <dlambda/compiler/expression.hpp>
#include <dlambda/compiler/ir_builder.hpp>

namespace dlambda {
  namespace compiler {
    expression remove_reference(
      const std::shared_ptr< llvm::LLVMContext > &context,
      const std::shared_ptr< ir_builder_t > &ir_builder,
      const expression &value
    );
  }
}

#endif

