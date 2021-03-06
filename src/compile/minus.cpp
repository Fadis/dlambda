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
#include <dlambda/compiler/node/minus.hpp>
#include <dlambda/compiler/expression.hpp>
#include <dlambda/compiler/get_llvm_type.hpp>
#include <dlambda/compiler/ir_builder.hpp>
#include <dlambda/compiler/node/remove_reference.hpp>
#include <dlambda/compiler/node/implicit_cast.hpp>
#include <dlambda/exceptions.hpp>

namespace dlambda {
  namespace compiler {
    namespace node {
      class minus : public boost::static_visitor< expression > {
      public:
        minus(
          const std::shared_ptr< llvm::LLVMContext > &context_,
          const std::shared_ptr< ir_builder_t > &ir_builder_,
          const expression &left_, const expression &right_
        ) : context( context_ ), ir_builder( ir_builder_ ), left( left_ ), right( right_ ) {}
        template< typename Left, typename Right > 
        expression operator()(
          const Left &, const Right &,
          typename boost::enable_if<
            boost::mpl::and_<
              type_traits::meta::is_arithmetic_convertible< Left >,
              type_traits::meta::is_arithmetic_convertible< Right >
            >
          >::type* = 0
        ) const {
          return generate_arithmetic_minus();
        }
        template< typename Left, typename Right > 
        expression operator()(
          const Left &, const Right &,
          typename boost::enable_if<
            boost::mpl::and_<
              type_traits::meta::is_pointer< Left >,
              type_traits::meta::is_arithmetic_convertible< Right >
            >
          >::type* = 0
        ) const {
          return generate_pointer_minus();
        }
        template< typename Left, typename Right >
        expression operator()(
          const Left &, const Right &,
          typename boost::enable_if<
            boost::mpl::not_<
              boost::mpl::or_<
                boost::mpl::and_<
                  type_traits::meta::is_arithmetic_convertible< Left >,
                  type_traits::meta::is_arithmetic_convertible< Right >
                >,
                boost::mpl::and_<
                  type_traits::meta::is_pointer< Left >,
                  type_traits::meta::is_arithmetic_convertible< Right >
                >
              >
            >
          >::type* = 0
        ) const {
          throw exceptions::invalid_expression();
        }
      private:
        expression generate_arithmetic_minus() const;
        expression generate_pointer_minus() const;
        std::shared_ptr< llvm::LLVMContext > context;
        std::shared_ptr< ir_builder_t > ir_builder;
        expression left;
        expression right;
      };
      expression minus::generate_arithmetic_minus() const {
        const auto result_type = type_traits::usual_arithmetic_conversion( left.type(), right.type() );
        const expression converted_left = implicit_cast( context, ir_builder, result_type, left );
        const expression converted_right = implicit_cast( context, ir_builder, result_type, right );
        const auto llvm_type = get_llvm_type( context, result_type );
        const std::shared_ptr< llvm::LLVMContext > &context_ = context;
        if( type_traits::is_floating_point( result_type ) ) {
          return expression(
            result_type, llvm_type,
            std::shared_ptr< llvm::Value >(
              ir_builder->CreateFSub(
                converted_left.llvm_value().get(),
                converted_right.llvm_value().get()
              ),
              [context_]( llvm::Value* ){}
            )
          );
        }
        else {
          return expression(
            result_type, llvm_type,
            std::shared_ptr< llvm::Value >(
              ir_builder->CreateSub(
                converted_left.llvm_value().get(),
                converted_right.llvm_value().get()
              ),
              [context_]( llvm::Value* ){}
            )
          );
        }
      }
      expression minus::generate_pointer_minus() const {
        const expression converted_index = implicit_cast( context, ir_builder, type_traits::integral_promotion( right.type() ), right );
        const auto pointer_type = type_traits::remove_cv( left.type() );
        const auto llvm_type = get_llvm_type( context, pointer_type );
        const std::shared_ptr< llvm::LLVMContext > &context_ = context;
        return expression(
          pointer_type, llvm_type,
          std::shared_ptr< llvm::Value >(
            ir_builder->CreateInBoundsGEP(
              left.llvm_value().get(),
              ir_builder->CreateNeg(
                converted_index.llvm_value().get()
              )
            ),
            [context_]( llvm::Value* ){}
          )
        );
      }
    }
    expression minus(
      const std::shared_ptr< llvm::LLVMContext > &context,
      const std::shared_ptr< ir_builder_t > &ir_builder,
      const expression &left_expr,
      const expression &right_expr
    ) {
      const expression left = remove_reference( context, ir_builder, left_expr );
      const expression right = remove_reference( context, ir_builder, right_expr );
      return apply_visitor( node::minus( context, ir_builder, left, right ), left.type(), right.type() );
    }
  }
}

