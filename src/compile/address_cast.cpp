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
#include <dlambda/exceptions.hpp>
#include <dlambda/compiler/node/address_cast.hpp>

namespace dlambda {
  namespace compiler {
    namespace node {
      class address_cast : public boost::static_visitor< expression > {
      public:
        address_cast(
          const std::shared_ptr< llvm::LLVMContext > &context_,
          const std::shared_ptr< ir_builder_t > &ir_builder_,
          const expression &value_
        ) : context( context_ ), ir_builder( ir_builder_ ), value( value_ ) {}
        template< typename From, typename To >
        expression operator()(
          const From &from, const To &to,
          typename boost::enable_if<
            boost::mpl::and_<
              type_traits::meta::is_promotable< From >,
              type_traits::meta::is_pointer< To >
            >
          >::type* = 0
        ) const {
          const auto llvm_type = get_llvm_type( context, to );
          const std::shared_ptr< llvm::LLVMContext > &context_ = context;
          if( from.length >= sizeof( void* ) * 8u ) {
            return expression(
              to, llvm_type,
              std::shared_ptr< llvm::Value >(
                ir_builder->CreateIntToPtr( value.llvm_value().get(), llvm_type.get() ),
                [context_]( llvm::Value* ){}
              )
            );
          }
          else
            throw exceptions::invalid_cast();
        }
        template< typename From, typename To >
        expression operator()(
          const From &, const To &to,
          typename boost::enable_if<
            boost::mpl::and_<
              type_traits::meta::is_pointer< From >,
              type_traits::meta::is_promotable< To >
            >
          >::type* = 0
        ) const {
          if( to.length >= sizeof( void* ) * 8u ) {
            const auto llvm_type = get_llvm_type( context, to );
            const std::shared_ptr< llvm::LLVMContext > &context_ = context;
            return expression(
              to, llvm_type,
              std::shared_ptr< llvm::Value >(
                ir_builder->CreatePtrToInt( value.llvm_value().get(), llvm_type.get() ),
                [context_]( llvm::Value* ){}
              )
            );
          }
          else
            throw exceptions::invalid_cast();
        }
        template< typename From, typename To >
        expression operator()(
          const From &, const To &,
          typename boost::enable_if<
            boost::mpl::not_<
              boost::mpl::or_<
                boost::mpl::and_<
                  type_traits::meta::is_promotable< From >,
                  type_traits::meta::is_pointer< To >
                >,
                boost::mpl::and_<
                  type_traits::meta::is_pointer< From >,
                  type_traits::meta::is_promotable< To >
                >
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
    expression address_cast(
      const std::shared_ptr< llvm::LLVMContext > &context,
      const std::shared_ptr< ir_builder_t > &ir_builder,
      const type cast_to,
      const expression &value
    ) {
      return apply_visitor( node::address_cast( context, ir_builder, value ), value.type(), cast_to );
    }
  }
}

