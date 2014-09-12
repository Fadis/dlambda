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
#include <dlambda/compiler/node/modulus.hpp>
#include <dlambda/compiler/expression.hpp>
#include <dlambda/compiler/get_llvm_type.hpp>
#include <dlambda/compiler/ir_builder.hpp>
#include <dlambda/compiler/usual_arithmetic_conversion.hpp>
#include <dlambda/compiler/node/remove_reference.hpp>
#include <dlambda/compiler/node/implicit_cast.hpp>

namespace dlambda {
  namespace compiler {
    namespace node {
      class modulus : public boost::static_visitor< expression > {
      public:
        modulus(
          const std::shared_ptr< llvm::LLVMContext > &context_,
          const std::shared_ptr< ir_builder_t > &ir_builder_,
          const expression &left_, const expression &right_
        ) : context( context_ ), ir_builder( ir_builder_ ), left( left_ ), right( right_ ) {}
        template< typename Left, typename Right > 
        expression operator()(
          const Left &, const Right &,
          typename boost::enable_if<
            type_traits::meta::is_arithmetic_convertible< Left, Right >
          >::type* = 0
        ) const {
          const auto converted = usual_arithmetic_conversion( context, ir_builder, left, right );
          const std::shared_ptr< llvm::LLVMContext > &context_ = context;
          if( type_traits::is_floating_point( converted.first.type() ) ) {
            return expression(
              converted.first.type(), converted.first.llvm_type(),
              std::shared_ptr< llvm::Value >(
                ir_builder->CreateFRem(
                  converted.first.llvm_value().get(),
                  converted.second.llvm_value().get()
                ),
                [context_]( llvm::Value* ){}
              )
            );
          }
          else if( type_traits::is_signed( converted.first.type() ) ) {
            return expression(
              converted.first.type(), converted.first.llvm_type(),
              std::shared_ptr< llvm::Value >(
                ir_builder->CreateSRem(
                  converted.first.llvm_value().get(),
                  converted.second.llvm_value().get()
                ),
                [context_]( llvm::Value* ){}
              )
            );
          }
          else {
            return expression(
              converted.first.type(), converted.first.llvm_type(),
              std::shared_ptr< llvm::Value >(
                ir_builder->CreateURem(
                  converted.first.llvm_value().get(),
                  converted.second.llvm_value().get()
                ),
                [context_]( llvm::Value* ){}
              )
            );
          }
        }
        template< typename Left, typename Right >
        expression operator()(
          const Left &, const Right &,
          typename boost::enable_if<
            boost::mpl::not_<
              type_traits::meta::is_arithmetic_convertible< Left, Right >
            >
          >::type* = 0
        ) const {
          throw -1;
        }
      private:
        std::shared_ptr< llvm::LLVMContext > context;
        std::shared_ptr< ir_builder_t > ir_builder;
        expression left;
        expression right;
      };
    }
    expression modulus(
      const std::shared_ptr< llvm::LLVMContext > &context,
      const std::shared_ptr< ir_builder_t > &ir_builder,
      const expression &left_expr,
      const expression &right_expr
    ) {
      const expression left = remove_reference( context, ir_builder, left_expr );
      const expression right = remove_reference( context, ir_builder, right_expr );
      return apply_visitor( node::modulus( context, ir_builder, left, right ), left.type(), right.type() );
    }
  }
}

