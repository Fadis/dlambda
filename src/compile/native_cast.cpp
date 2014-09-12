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
#include <dlambda/type_traits/is_const.hpp>
#include <dlambda/type_traits/remove_const.hpp>
#include <dlambda/type_traits/is_signed.hpp>
#include <dlambda/type_traits/is_arithmetic.hpp>
#include <dlambda/type_traits/is_floating_point.hpp>
#include <dlambda/exceptions.hpp>
#include <dlambda/compiler/node/native_cast.hpp>

namespace dlambda {
  namespace compiler {
    namespace node {
      class native_cast : public boost::static_visitor< expression > {
      public:
        native_cast(
          const std::shared_ptr< llvm::LLVMContext > &context_,
          const std::shared_ptr< ir_builder_t > &ir_builder_,
          const expression &value_
        ) : context( context_ ), ir_builder( ir_builder_ ), value( value_ ) {}
        template< typename T >
        expression operator()(
          const T &from, const T &to,
          typename boost::enable_if<
            boost::mpl::or_<
              boost::is_same< T, types::pointer< type > >,
              boost::is_same< T, types::lref< type > >,
              boost::is_same< T, types::rref< type > >
            >
          >::type* = 0
        ) const {
          if(
            !type_traits::is_const( from ) ||
            ( type_traits::is_const( from ) && type_traits::is_const( to ) )
          ) {
            if( type_traits::remove_const( from ) == type_traits::remove_const( to ) )
              return expression( to, value.llvm_type(), value.llvm_value() );
          }
          throw exceptions::invalid_cast();
        }
        template< typename T >
        expression operator()(
          const T &from, const T &to,
          typename boost::enable_if<
            boost::mpl::not_<
              boost::mpl::or_<
                boost::is_same< T, types::pointer< type > >,
                boost::is_same< T, types::lref< type > >,
                boost::is_same< T, types::rref< type > >
              >
            >
          >::type* = 0
        ) const {
          if( type_traits::remove_const( from ) == type_traits::remove_const( to ) )
            return expression( to, value.llvm_type(), value.llvm_value() );
          else
            throw exceptions::invalid_cast();
        }
        template< typename From, typename To >
        expression operator()(
          const From &, const To &,
          typename boost::enable_if<
            boost::mpl::not_< boost::is_same< From, To > >
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
    expression native_cast(
      const std::shared_ptr< llvm::LLVMContext > &context,
      const std::shared_ptr< ir_builder_t > &ir_builder,
      const type cast_to,
      const expression &value
    ) {
      return apply_visitor( node::native_cast( context, ir_builder, value ), value.type(), cast_to );
    }
  }
}

