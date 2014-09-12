#ifndef DLAMBDA_COMPILER_NODE_TYPE_ONLY_CAST_HPP
#define DLAMBDA_COMPILER_NODE_TYPE_ONLY_CAST_HPP

#include <memory>
#include <boost/mpl/and.hpp>
#include <boost/mpl/or.hpp>
#include <boost/type_traits/is_same.hpp>
#include <llvm/IR/LLVMContext.h>
#include <dlambda/type.hpp>
#include <dlambda/compiler/ir_builder.hpp>
#include <dlambda/compiler/expression.hpp>

namespace dlambda {
  namespace compiler {
    namespace meta {
      template< typename From, typename To >
      struct is_type_only_castable :
        public boost::mpl::and_<
          boost::mpl::or_<
            boost::is_same< From, types::lref< type > >,
            boost::is_same< From, types::rref< type > >,
            boost::is_same< From, types::pointer< type > >
          >,
          boost::mpl::or_<
            boost::is_same< To, types::lref< type > >,
            boost::is_same< To, types::rref< type > >,
            boost::is_same< To, types::pointer< type > >
          >
        > {};
    }
    expression type_only_cast(
      const std::shared_ptr< llvm::LLVMContext > &context,
      const std::shared_ptr< ir_builder_t > &ir_builder,
      const type cast_to,
      const expression &value
    );
  }
}

#endif

