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
#include <dlambda/type_traits/is_array.hpp>
#include <dlambda/type_traits/is_pointer.hpp>
#include <dlambda/compiler/node/dereference.hpp>
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
      class dereference : public boost::static_visitor< expression > {
      public:
        dereference(
          const std::shared_ptr< llvm::LLVMContext > &context_,
          const std::shared_ptr< ir_builder_t > &ir_builder_,
          const expression &value_
        ) : context( context_ ), ir_builder( ir_builder_ ), value( value_ ) {}
        template< typename ValueType > 
        expression operator()(
          const ValueType &,
          typename boost::enable_if<
            type_traits::meta::is_pointer< ValueType >
          >::type* = 0
        ) const {
          const auto result_type = type_traits::remove_pointer( value.type() );
          const auto llvm_type = get_llvm_type( context, result_type );
          const std::shared_ptr< llvm::LLVMContext > &context_ = context;
          return expression(
            result_type, llvm_type,
            std::shared_ptr< llvm::Value >(
              ir_builder->CreateLoad(
                value.llvm_value().get()
              ),
              [context_]( llvm::Value* ){}
            )
          );
        }
        template< typename ValueType > 
        expression operator()(
          const ValueType &,
          typename boost::enable_if<
            type_traits::meta::is_array< ValueType >
          >::type* = 0
        ) const {
          const auto result_type = type_traits::remove_array( value.type() );
          const auto llvm_type = get_llvm_type( context, result_type );
          const std::shared_ptr< llvm::LLVMContext > &context_ = context;
          return expression(
            result_type, llvm_type,
            std::shared_ptr< llvm::Value >(
              ir_builder->CreateConstInBoundsGEP1_32(
                value.llvm_value().get(), 0u
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
              boost::mpl::or_<
                type_traits::meta::is_pointer< ValueType >,
                type_traits::meta::is_array< ValueType >
              >
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
    expression dereference(
      const std::shared_ptr< llvm::LLVMContext > &context,
      const std::shared_ptr< ir_builder_t > &ir_builder,
      const expression &expr_
    ) {
      const expression expr = remove_reference( context, ir_builder, expr_ );
      return apply_visitor( node::dereference( context, ir_builder, expr ), expr.type() );
    }
  }
}

