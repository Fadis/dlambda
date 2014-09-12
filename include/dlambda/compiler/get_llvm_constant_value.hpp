#ifndef DLAMBDA_GET_LLVM_CONSTANT_VALUE_HPP
#define DLAMBDA_GET_LLVM_CONSTANT_VALUE_HPP

#include <memory>
#include <boost/array.hpp>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Constants.h>
#include <dlambda/type.hpp>
#include <dlambda/value.hpp>
#include <dlambda/proto/node/literal.hpp>

namespace dlambda {
  namespace compiler {
    std::pair< std::shared_ptr< llvm::Type >, std::shared_ptr< llvm::Constant > > get_llvm_constant_value(
      const std::shared_ptr< llvm::LLVMContext >&,
      const dlambda::proto::literal&
    );
  }
}

#endif

