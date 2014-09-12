#ifndef DLAMBDA_COMPILER_NODE_MEM_PTR_HPP
#define DLAMBDA_COMPILER_NODE_MEM_PTR_HPP

#include <memory>
#include <llvm/IR/Value.h>
#include <llvm/IR/IRBuilder.h>

#include <dlambda/exceptions.hpp>
#include <dlambda/compiler/expression.hpp>
#include <dlambda/compiler/ir_builder.hpp>

namespace dlambda {
  namespace compiler {
    expression mem_ptr(
      const std::shared_ptr< llvm::LLVMContext > &,
      const std::shared_ptr< ir_builder_t > &,
      const expression &,
      const expression &
    ) {
      throw exceptions::not_implemented();
    }
  }
}

#endif

