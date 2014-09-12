#include <boost/mpl/and.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/not.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/range/algorithm/copy.hpp>

#include <dlambda/type.hpp>
#include <dlambda/type_traits/is_lvalue_reference.hpp>
#include <dlambda/type_traits/is_rvalue_reference.hpp>
#include <dlambda/type_traits/remove_reference.hpp>
#include <dlambda/type_traits/is_volatile.hpp>
#include <dlambda/proto/node.hpp>
#include <dlambda/proto/eval.hpp>
#include <dlambda/compiler/expression.hpp>
#include <dlambda/compiler/ir_builder.hpp>
#include <dlambda/compiler/node/remove_reference.hpp>
#include <dlambda/compiler/get_llvm_type.hpp>
#include <llvm/IR/Value.h>

namespace dlambda {
  namespace compiler {
    namespace node {
      class remove_reference : public boost::static_visitor< expression > {
      public:
        remove_reference(
          const std::shared_ptr< llvm::LLVMContext > &context_,
          const std::shared_ptr< ir_builder_t > &ir_builder_,
          const expression &value_
        ) : context( context_ ), ir_builder( ir_builder_ ), value( value_ ) {}
        template< typename Type >
        expression operator()(
          const types::lref< Type > &type
        ) const {
          const auto llvm_type = get_llvm_type( context, type.base );
          const std::shared_ptr< llvm::LLVMContext > &context_ = context;
          return expression(
            type.base, llvm_type,
            std::shared_ptr< llvm::Value >(
              ir_builder->CreateLoad( value.llvm_value().get(), type_traits::is_volatile( type_traits::remove_reference( type ) ) ),
              [context_]( llvm::Value* ){}
            )
          );
        }
        template< typename Type >
        expression operator()(
          const types::rref< Type > &type
        ) const {
          const auto llvm_type = get_llvm_type( context, type.base );
          const std::shared_ptr< llvm::LLVMContext > &context_ = context;
          return expression(
            type.base, llvm_type,
            std::shared_ptr< llvm::Value >(
              ir_builder->CreateLoad( value.llvm_value().get(), type_traits::is_volatile( type_traits::remove_reference( type ) ) ),
              [context_]( llvm::Value* ){}
            )
          );
        }
        template< typename Type >
        expression operator()(
          const Type&,
          typename boost::enable_if<
            boost::mpl::not_<
              boost::mpl::or_<
                type_traits::meta::is_lvalue_reference< Type >,
                type_traits::meta::is_rvalue_reference< Type >
              >
            >
          >::type* = 0
        ) const {
          return value;
        }
      private:
        std::shared_ptr< llvm::LLVMContext > context;
        std::shared_ptr< ir_builder_t > ir_builder;
        expression value;
      };
    }
    expression remove_reference(
      const std::shared_ptr< llvm::LLVMContext > &context,
      const std::shared_ptr< ir_builder_t > &ir_builder,
      const expression &value
    ) {
      return apply_visitor( node::remove_reference( context, ir_builder, value ), value.type() );
    }
  }
}

