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
#include <dlambda/type_traits/remove_cv.hpp>
#include <dlambda/type_traits/is_const.hpp>
#include <dlambda/exceptions.hpp>
#include <dlambda/compiler/node/type_only_cast.hpp>

namespace dlambda {
  namespace compiler {
    namespace node {
      class type_only_cast : public boost::static_visitor< expression > {
      public:
        type_only_cast(
          const std::shared_ptr< llvm::LLVMContext > &context_,
          const std::shared_ptr< ir_builder_t > &ir_builder_,
          const expression &value_
        ) : context( context_ ), ir_builder( ir_builder_ ), value( value_ ) {}
        template< typename From, typename To >
        expression operator()(
          const From &, const To &to,
          typename boost::enable_if<
            meta::is_type_only_castable< From, To >
          >::type* = 0
        ) const {
          return expression( to, value.llvm_type(), value.llvm_value() );
        }
        template< typename From, typename To >
        expression operator()(
          const From &, const To &,
          typename boost::enable_if<
            boost::mpl::not_< meta::is_type_only_castable< From, To > >
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
    expression type_only_cast(
      const std::shared_ptr< llvm::LLVMContext > &context,
      const std::shared_ptr< ir_builder_t > &ir_builder,
      const type cast_to,
      const expression &value
    ) {
      return apply_visitor( node::type_only_cast( context, ir_builder, value ), value.type(), cast_to );
    }
  }
}

