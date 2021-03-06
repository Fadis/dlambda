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
#include <dlambda/type_traits/integral_promotion.hpp>
#include <dlambda/type_traits/usual_arithmetic_conversion.hpp>
#include <dlambda/type_traits/is_pointer.hpp>
#include <dlambda/type_traits/is_arithmetic.hpp>
#include <dlambda/type_traits/is_signed.hpp>
#include <dlambda/type_traits/remove_cv.hpp>
#include <dlambda/compiler/node/bitwise_xor.hpp>
#include <dlambda/compiler/expression.hpp>
#include <dlambda/compiler/get_llvm_type.hpp>
#include <dlambda/compiler/ir_builder.hpp>
#include <dlambda/compiler/node/remove_reference.hpp>
#include <dlambda/compiler/node/implicit_cast.hpp>
#include <dlambda/compiler/usual_arithmetic_conversion.hpp>
#include <dlambda/exceptions.hpp>

namespace dlambda {
  namespace compiler {
    namespace node {
      class bitwise_xor : public boost::static_visitor< expression > {
      public:
        bitwise_xor(
          const std::shared_ptr< llvm::LLVMContext > &context_,
          const std::shared_ptr< ir_builder_t > &ir_builder_,
          const expression &left_, const expression &right_
        ) : context( context_ ), ir_builder( ir_builder_ ), left( left_ ), right( right_ ) {}
        template< typename Left, typename Right > 
        expression operator()(
          const Left &, const Right &,
          typename boost::enable_if<
            boost::mpl::and_<
              type_traits::meta::is_promotable< Left >,
              type_traits::meta::is_promotable< Right >
            >
          >::type* = 0
        ) const {
          const auto converted = usual_arithmetic_conversion( context, ir_builder, left, right );
          const std::shared_ptr< llvm::LLVMContext > &context_ = context;
          return expression(
            converted.first.type(), converted.first.llvm_type(),
            std::shared_ptr< llvm::Value >(
              ir_builder->CreateXor(
                converted.first.llvm_value().get(),
                converted.second.llvm_value().get()
              ),
              [context_]( llvm::Value* ){}
            )
          );
        }
        template< typename Left, typename Right >
        expression operator()(
          const Left &, const Right &,
          typename boost::enable_if<
            boost::mpl::not_<
              boost::mpl::and_<
                type_traits::meta::is_promotable< Left >,
                type_traits::meta::is_promotable< Right >
              >
            >
          >::type* = 0
        ) const {
          throw exceptions::invalid_expression();
        }
      private:
        std::shared_ptr< llvm::LLVMContext > context;
        std::shared_ptr< ir_builder_t > ir_builder;
        expression left;
        expression right;
      };
    }
    expression bitwise_xor(
      const std::shared_ptr< llvm::LLVMContext > &context,
      const std::shared_ptr< ir_builder_t > &ir_builder,
      const expression &left_expr,
      const expression &right_expr
    ) {
      const expression left = remove_reference( context, ir_builder, left_expr );
      const expression right = remove_reference( context, ir_builder, right_expr );
      return apply_visitor( node::bitwise_xor( context, ir_builder, left, right ), left.type(), right.type() );
    }
  }
}

