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
#include <dlambda/type_traits/is_promotable.hpp>
#include <dlambda/type_traits/integral_promotion.hpp>
#include <dlambda/type_traits/is_floating_point.hpp>
#include <dlambda/compiler/node/negate.hpp>
#include <dlambda/compiler/expression.hpp>
#include <dlambda/compiler/get_llvm_type.hpp>
#include <dlambda/compiler/ir_builder.hpp>
#include <dlambda/compiler/get_llvm_constant_value.hpp>
#include <dlambda/compiler/node/remove_reference.hpp>
#include <dlambda/compiler/node/implicit_cast.hpp>
#include <dlambda/compiler/usual_arithmetic_conversion.hpp>
#include <dlambda/exceptions.hpp>

namespace dlambda {
  namespace compiler {
    namespace node {
      class complement : public boost::static_visitor< expression > {
      public:
        complement(
          const std::shared_ptr< llvm::LLVMContext > &context_,
          const std::shared_ptr< ir_builder_t > &ir_builder_,
          const expression &value_
        ) : context( context_ ), ir_builder( ir_builder_ ), value( value_ ) {}
        template< typename ValueType > 
        expression operator()(
          const ValueType &,
          typename boost::enable_if<
            type_traits::meta::is_promotable< ValueType >
          >::type* = 0
        ) const {
          const auto result_type = type_traits::integral_promotion( value.type() );
          const expression converted_value = implicit_cast( context, ir_builder, result_type, value );
          const auto zero_ = proto::literal( 0 );
          const expression zero = implicit_cast(
            context, ir_builder, result_type,
            expression( zero_.type(), get_llvm_constant_value( context, zero_ ) )
          );
          const auto one_ = proto::literal( 1 );
          const expression one = implicit_cast(
            context, ir_builder, result_type,
            expression( one_.type(), get_llvm_constant_value( context, one_ ) )
          );
          const auto llvm_type = get_llvm_type( context, result_type );
          const std::shared_ptr< llvm::LLVMContext > &context_ = context;
          return expression(
            result_type, llvm_type,
            std::shared_ptr< llvm::Value >(
              ir_builder->CreateXor(
                converted_value.llvm_value().get(),
                ir_builder->CreateSub( zero.llvm_value().get(), one.llvm_value().get() )
              ),
              [context_]( llvm::Value* ){}
            )
          );
        }
        template< typename ValueType > 
        expression operator()(
          const ValueType &,
          typename boost::enable_if<
            boost::mpl::not_<
              type_traits::meta::is_promotable< ValueType >
            >
          >::type* = 0
        ) const {
          throw exceptions::invalid_expression();
        }
      private:
        std::shared_ptr< llvm::LLVMContext > context;
        std::shared_ptr< ir_builder_t > ir_builder;
        expression value;
      };
    }
    expression complement(
      const std::shared_ptr< llvm::LLVMContext > &context,
      const std::shared_ptr< ir_builder_t > &ir_builder,
      const expression &expr_
    ) {
      const expression expr = remove_reference( context, ir_builder, expr_ );
      return apply_visitor( node::complement( context, ir_builder, expr ), expr.type() );
    }
  }
}

