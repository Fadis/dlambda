#ifndef DLAMBDA_GET_LLVM_FUNCTION_TYPE_HPP
#define DLAMBDA_GET_LLVM_FUNCTION_TYPE_HPP

#include <memory>
#include <llvm/IR/Type.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Constant.h>
#include <dlambda/type.hpp>

namespace dlambda {
  namespace compiler {
    std::shared_ptr< llvm::FunctionType > get_llvm_function_type(
      const std::shared_ptr< llvm::LLVMContext > &context,
      const type &type_
    );
  }
}

#endif

