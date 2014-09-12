#ifndef DLAMBDA_COMPILER_NODE_ADDRESS_TO_POINTER_HPP
#define DLAMBDA_COMPILER_NODE_ADDRESS_TO_POINTER_HPP

#include <memory>

#include <dlambda/compiler/expression.hpp>
#include <dlambda/compiler/ir_builder.hpp>

namespace dlambda {
  namespace compiler {
    expression address_to_pointer(
      const std::shared_ptr< llvm::LLVMContext > &context,
      const std::shared_ptr< ir_builder_t > &ir_builder,
      const expression &value
    );
  }
}

#endif

