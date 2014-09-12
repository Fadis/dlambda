#ifndef DLAMBDA_COMPILER_NODE_REMOVE_CONST_CAST_HPP
#define DLAMBDA_COMPILER_NODE_REMOVE_CONST_CAST_HPP

#include <memory>
#include <boost/type_traits/is_same.hpp>
#include <llvm/IR/LLVMContext.h>
#include <dlambda/type.hpp>
#include <dlambda/compiler/ir_builder.hpp>
#include <dlambda/compiler/expression.hpp>

namespace dlambda {
  namespace compiler {
    namespace meta {
      template< typename From, typename To >
      struct is_remove_const_castable :
        public boost::is_same< From, To > {};
    }
    expression remove_const_cast(
      const std::shared_ptr< llvm::LLVMContext > &context,
      const std::shared_ptr< ir_builder_t > &ir_builder,
      const type cast_to,
      const expression &value
    );
  }
}

#endif

