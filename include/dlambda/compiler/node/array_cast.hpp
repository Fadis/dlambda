#ifndef DLAMBDA_COMPILER_NODE_ARRAY_CAST_HPP
#define DLAMBDA_COMPILER_NODE_ARRAY_CAST_HPP

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
      struct is_array_castable :
        public boost::mpl::and_<
          boost::is_same< From, types::array< type > >,
          boost::is_same< To, types::pointer< type > >
        > {};
    }
    expression array_cast(
      const std::shared_ptr< llvm::LLVMContext > &context,
      const std::shared_ptr< ir_builder_t > &ir_builder,
      const type cast_to,
      const expression &value
    );
  }
}

#endif

