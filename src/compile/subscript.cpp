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
#include <dlambda/type_traits/remove_reference.hpp>
#include <dlambda/type_traits/remove_pointer.hpp>
#include <dlambda/type_traits/remove_array.hpp>
#include <dlambda/type_traits/add_lvalue_reference.hpp>
#include <dlambda/type_traits/is_array.hpp>
#include <dlambda/type_traits/is_reference.hpp>
#include <dlambda/type_traits/is_pointer.hpp>
#include <dlambda/compiler/node/subscript.hpp>
#include <dlambda/compiler/expression.hpp>
#include <dlambda/compiler/get_llvm_type.hpp>
#include <dlambda/compiler/ir_builder.hpp>
#include <dlambda/compiler/node/remove_reference.hpp>
#include <dlambda/compiler/node/implicit_cast.hpp>
#include <dlambda/compiler/usual_arithmetic_conversion.hpp>
#include <dlambda/compiler/integral_promotion.hpp>
#include <dlambda/exceptions.hpp>

namespace dlambda {
  namespace compiler {
    namespace node {
      class subscript : public boost::static_visitor< expression > {
      public:
        subscript(
          const std::shared_ptr< llvm::LLVMContext > &context_,
          const std::shared_ptr< ir_builder_t > &ir_builder_,
          const expression &left_, const expression &right_
        ) : context( context_ ), ir_builder( ir_builder_ ), left( left_ ), right( right_ ) {}
        template< typename LeftType, typename RightType > 
        expression operator()(
          const LeftType &, const RightType &,
          typename boost::enable_if<
            boost::mpl::and_<
              type_traits::meta::is_pointer< LeftType >,
              type_traits::meta::is_promotable< RightType >
            >
          >::type* = 0
        ) const {
          const auto converted_right = integral_promotion( context, ir_builder, right );
          const auto result_type = type_traits::add_lvalue_reference( type_traits::remove_pointer( left.type() ) );
          const auto llvm_type = get_llvm_type( context, result_type );
          const std::shared_ptr< llvm::LLVMContext > &context_ = context;
          return expression(
            result_type, llvm_type,
            std::shared_ptr< llvm::Value >(
              ir_builder->CreateInBoundsGEP(
                left.llvm_value().get(),
                converted_right.llvm_value().get()
              ),
              [context_]( llvm::Value* ){}
            )
          );
        }
        template< typename LeftType, typename RightType > 
        expression operator()(
          const LeftType &left_type, const RightType &,
          typename boost::enable_if<
            boost::mpl::and_<
              type_traits::meta::is_reference< LeftType >,
              type_traits::meta::is_promotable< RightType >
            >
          >::type* = 0
        ) const {
          const auto converted_right = integral_promotion( context, ir_builder, right );
          if( type_traits::is_pointer( left_type.base ) ) {
            const expression left_deref = remove_reference( context, ir_builder, left );
            const auto result_type = type_traits::add_lvalue_reference( type_traits::remove_pointer( left_deref.type() ) );
            const auto llvm_type = get_llvm_type( context, result_type );
            const std::shared_ptr< llvm::LLVMContext > &context_ = context;
            return expression(
              result_type, llvm_type,
              std::shared_ptr< llvm::Value >(
                ir_builder->CreateInBoundsGEP(
                  left_deref.llvm_value().get(),
                  converted_right.llvm_value().get()
                ),
                [context_]( llvm::Value* ){}
              )
            );
          }
          else if( type_traits::is_array( left_type.base ) ) {
            const auto result_type = type_traits::add_lvalue_reference( type_traits::remove_array( left_type.base ) );
            const auto llvm_type = get_llvm_type( context, result_type );
            const std::shared_ptr< llvm::LLVMContext > &context_ = context;
            return expression(
              result_type, llvm_type,
              std::shared_ptr< llvm::Value >(
                ir_builder->CreateInBoundsGEP(
                  left.llvm_value().get(),
                  converted_right.llvm_value().get()
                ),
                [context_]( llvm::Value* ){}
              )
            );
          }
          else throw exceptions::invalid_expression();
        }
        template< typename LeftType, typename RightType > 
        expression operator()(
          const LeftType &, const RightType &,
          typename boost::enable_if<
            boost::mpl::and_<
              type_traits::meta::is_array< LeftType >,
              type_traits::meta::is_promotable< RightType >
            >
          >::type* = 0
        ) const {
          const auto converted_right = integral_promotion( context, ir_builder, right );
          const auto result_type = type_traits::add_lvalue_reference( type_traits::remove_array( left.type() ) );
          const auto llvm_type = get_llvm_type( context, result_type );
          const std::shared_ptr< llvm::LLVMContext > &context_ = context;
          const auto addr = ir_builder->CreateAlloca( left.llvm_type().get() );
          ir_builder->CreateStore( left.llvm_value().get(), addr );
          return expression(
            result_type, llvm_type,
            std::shared_ptr< llvm::Value >(
              ir_builder->CreateInBoundsGEP(
                addr,
                converted_right.llvm_value().get()
              ),
              [context_]( llvm::Value* ){}
            )
          );
        }
        template< typename LeftType, typename RightType > 
        expression operator()(
          const LeftType &, const RightType&,
          typename boost::enable_if<
            boost::mpl::not_<
              boost::mpl::or_<
                boost::mpl::and_<
                  type_traits::meta::is_pointer< LeftType >,
                  type_traits::meta::is_promotable< RightType >
                >,
                boost::mpl::and_<
                  type_traits::meta::is_reference< LeftType >,
                  type_traits::meta::is_promotable< RightType >
                >,
                boost::mpl::and_<
                  type_traits::meta::is_array< LeftType >,
                  type_traits::meta::is_promotable< RightType >
                >
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
    expression subscript(
      const std::shared_ptr< llvm::LLVMContext > &context,
      const std::shared_ptr< ir_builder_t > &ir_builder,
      const expression &left, const expression &right_
    ) {
      const expression right = remove_reference( context, ir_builder, right_ );
      return apply_visitor( node::subscript( context, ir_builder, left, right ), left.type(), right.type() );
    }
  }
}

