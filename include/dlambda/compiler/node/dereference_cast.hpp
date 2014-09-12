#ifndef DLAMBDA_COMPILER_NODE_ARITHMETIC_CAST_HPP
#define DLAMBDA_COMPILER_NODE_ARITHMETIC_CAST_HPP

#include <boost/mpl/and.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/not.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <llvm/IR/Value.h>
#include <llvm/IR/IRBuilder.h>

#include <dlambda/type.hpp>
#include <dlambda/proto/node.hpp>
#include <dlambda/proto/eval.hpp>
#include <dlambda/compiler/get_llvm_type.hpp>
#include <dlambda/compiler/get_llvm_constant_value.hpp>
#include <dlambda/type_traits/integral_promotion.hpp>
#include <dlambda/type_traits/usual_arithmetic_conversion.hpp>
#include <dlambda/type_traits/is_pointer.hpp>
#include <dlambda/type_traits/is_signed.hpp>
#include <dlambda/type_traits/is_arithmetic.hpp>
#include <dlambda/type_traits/is_floating_point.hpp>
#include <dlambda/exceptions.hpp>

namespace dlambda {
  namespace compiler {
    namespace node {
      template< typename MasterCast >
      class dereference_cast : public boost::static_visitor< expression > {
      public:
        dereference_cast(
          const std::shared_ptr< llvm::LLVMContext > &context_,
          const std::shared_ptr< ir_builder_t > &ir_builder_,
          const expression &value_
        ) : context( context_ ), ir_builder( ir_builder_ ), value( value_ ) {}
        template< typename From, typename To >
        expression operator()(
          const From &from, const To &to,
          typename boost::enable_if<
            boost::mpl::or_<
              type_traits::meta::is_same< From, types::lref< type > >,
              type_traits::meta::is_same< From, types::rref< type > >
            >
          >::type* = 0
        ) const {
          const expression loaded_value = remove_reference( context, ir_builder, value );
          return apply_visitor( MasterCast( context_, ir_builder_, loaded_value ), loaded_value.type(), to );
        }
        template< typename From, typename To >
        expression operator()(
          const From &, const To &,
          typename boost::enable_if<
            boost::mpl::not_<
              boost::mpl::or_<
                type_traits::meta::is_same< From, types::lref< type > >,
                type_traits::meta::is_same< From, types::rref< type > >
              >
            >
          >::type* = 0
        ) const {
          throw exceptions::invalid_cast();
        }
      private:
        std::shared_ptr< llvm::LLVMContext > context;
        std::shared_ptr< ir_builder_t > ir_builder;
        expression value;
      };
    }
    template< typename MasterCast >
    expression dereference_cast(
      const std::shared_ptr< llvm::LLVMContext > &context,
      const std::shared_ptr< ir_builder_t > &ir_builder,
      const type cast_to,
      const expression &value
    ) {
      return apply_visitor( node::dereference_cast< MasterCast >( context, ir_builder, value ), value.type(), cast_to );
    }
  }
}

#endif

