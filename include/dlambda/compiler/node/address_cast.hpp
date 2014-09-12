#ifndef DLAMBDA_COMPILER_NODE_ADDRESS_CAST_HPP
#define DLAMBDA_COMPILER_NODE_ADDRESS_CAST_HPP

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
      struct is_address_castable :
        public boost::mpl::or_<
          boost::mpl::and_<
            type_traits::meta::is_promotable< From >,
            type_traits::meta::is_pointer< To >
          >,
          boost::mpl::and_<
            type_traits::meta::is_pointer< From >,
            type_traits::meta::is_promotable< To >
          >
        > {};
    }
    expression address_cast(
      const std::shared_ptr< llvm::LLVMContext > &context,
      const std::shared_ptr< ir_builder_t > &ir_builder,
      const type cast_to,
      const expression &value
    );
  }
}

#endif

