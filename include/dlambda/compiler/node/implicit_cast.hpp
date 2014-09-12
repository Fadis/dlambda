#ifndef DLAMBDA_COMPILER_NODE_IMPLICIT_CAST_HPP
#define DLAMBDA_COMPILER_NODE_IMPLICIT_CAST_HPP

#include <memory>
#include <boost/mpl/and.hpp>
#include <boost/mpl/or.hpp>
#include <llvm/IR/LLVMContext.h>
#include <dlambda/type.hpp>
#include <dlambda/compiler/ir_builder.hpp>
#include <dlambda/compiler/expression.hpp>

#include <dlambda/type_traits/integral_promotion.hpp>
#include <dlambda/type_traits/is_promotable.hpp>
#include <dlambda/type_traits/is_pointer.hpp>
#include <dlambda/type_traits/is_signed.hpp>
#include <dlambda/type_traits/is_arithmetic.hpp>
#include <dlambda/type_traits/is_floating_point.hpp>

namespace dlambda {
  namespace compiler {
    namespace meta {
      template< typename From, typename To >
      struct is_implicitly_castable :
        public boost::mpl::or_<
          boost::mpl::and_<
            type_traits::meta::is_promotable< From >,
            type_traits::meta::is_floating_point< To >
          >,
          boost::mpl::and_<
            type_traits::meta::is_floating_point< From >,
            type_traits::meta::is_promotable< To >
          >,
          boost::mpl::and_<
            type_traits::meta::is_promotable< From >,
            type_traits::meta::is_promotable< To >
          >,
          boost::mpl::and_<
            type_traits::meta::is_pointer< From >,
            type_traits::meta::is_pointer< To >
          >,
          boost::mpl::and_<
            type_traits::meta::is_floating_point< From >,
            type_traits::meta::is_floating_point< To >
          >
        > {};
    }
    expression implicit_cast(
      const std::shared_ptr< llvm::LLVMContext > &context,
      const std::shared_ptr< ir_builder_t > &ir_builder,
      const type cast_to,
      const expression &value
    );
  }
}

#endif

