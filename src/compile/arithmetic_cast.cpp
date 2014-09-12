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
#include <dlambda/exceptions.hpp>
#include <dlambda/compiler/node/arithmetic_cast.hpp>

namespace dlambda {
  namespace compiler {
    namespace node {
      class arithmetic_cast : public boost::static_visitor< expression > {
      public:
        arithmetic_cast(
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
              boost::is_same< To, types::bool_ >,
              boost::mpl::not_< boost::is_same< From, types::bool_ > >
            >
          >::type* = 0
        ) const {
          const auto llvm_type = get_llvm_type( context, to );
          const auto zero_ = proto::literal( 0 );
          const expression zero = expression( zero_.type(), get_llvm_constant_value( context, zero_ ) );
          const std::shared_ptr< llvm::LLVMContext > &context_ = context;
          return expression(
            to, llvm_type,
            std::shared_ptr< llvm::Value >(
              ir_builder->CreateIntCast(
                ir_builder->CreateICmpNE(
                  value.llvm_value().get(),
                  ir_builder->CreateIntCast(
                    zero.llvm_value().get(),
                    get_llvm_type( context, from ).get(), type_traits::is_signed( from )
                  )
                ),
                llvm_type.get(), false
              ),
              [context_]( llvm::Value* ){}
            )
          );
        }
        template< typename From, typename To >
        expression operator()(
          const From &, const To &to,
          typename boost::enable_if<
            boost::mpl::and_<
              boost::is_same< To, types::bool_ >,
              boost::is_same< From, types::bool_ >
            >
          >::type* = 0
        ) const {
          return expression( to, value.llvm_type(), value.llvm_value() );
        }
        template< typename From, typename To >
        expression operator()(
          const From &from, const To &to,
          typename boost::enable_if<
            boost::mpl::and_<
              type_traits::meta::is_floating_point< From >,
              boost::is_same< To, types::bool_ >
            >
          >::type* = 0
        ) const {
          const auto llvm_type = get_llvm_type( context, to );
          const auto zero_ = proto::literal( 0.0f );
          const expression zero = expression( zero_.type(), get_llvm_constant_value( context, zero_ ) );
          const std::shared_ptr< llvm::LLVMContext > &context_ = context;
          return expression(
            to, llvm_type,
            std::shared_ptr< llvm::Value >(
              ir_builder->CreateIntCast(
                ir_builder->CreateICmpNE(
                  value.llvm_value().get(),
                  ir_builder->CreateFPCast(
                    zero.llvm_value().get(),
                    get_llvm_type( context, from ).get()
                  )
                ),
                llvm_type.get(), false
              ),
              [context_]( llvm::Value* ){}
            )
          );
        }
        template< typename From, typename To >
        expression operator()(
          const From &from, const To &to,
          typename boost::enable_if<
            boost::mpl::and_<
              type_traits::meta::is_promotable< From >,
              type_traits::meta::is_floating_point< To >
            >
          >::type* = 0
        ) const {
          const auto llvm_type = get_llvm_type( context, to );
          const std::shared_ptr< llvm::LLVMContext > &context_ = context;
          if( type_traits::is_signed( from ) ) {
            return expression(
              to, llvm_type,
              std::shared_ptr< llvm::Value >(
                ir_builder->CreateSIToFP( value.llvm_value().get(), llvm_type.get() ),
                [context_]( llvm::Value* ){}
              )
            );
          }
          else {
            return expression(
              to, llvm_type,
              std::shared_ptr< llvm::Value >(
                ir_builder->CreateUIToFP( value.llvm_value().get(), llvm_type.get() ),
                [context_]( llvm::Value* ){}
              )
            );
          }
        }
        template< typename From, typename To >
        expression operator()(
          const From &, const To &to,
          typename boost::enable_if<
            boost::mpl::and_<
              type_traits::meta::is_floating_point< From >,
              type_traits::meta::is_promotable< To >,
              boost::mpl::not_< boost::is_same< To, types::bool_ > >
            >
          >::type* = 0
        ) const {
          const auto llvm_type = get_llvm_type( context, to );
          const std::shared_ptr< llvm::LLVMContext > &context_ = context;
          if( type_traits::is_signed( to ) ) {
            return expression(
              to, llvm_type,
              std::shared_ptr< llvm::Value >(
                ir_builder->CreateFPToSI( value.llvm_value().get(), llvm_type.get() ),
                [context_]( llvm::Value* ){}
              )
            );
          }
          else {
            return expression(
              to, llvm_type,
              std::shared_ptr< llvm::Value >(
                ir_builder->CreateFPToUI( value.llvm_value().get(), llvm_type.get() ),
                [context_]( llvm::Value* ){}
              )
            );
          }
        }
        template< typename From, typename To >
        expression operator()(
          const From &from, const To &to,
          typename boost::enable_if<
            boost::mpl::and_<
              type_traits::meta::is_promotable< From >,
              type_traits::meta::is_promotable< To >,
              boost::mpl::not_< boost::is_same< To, types::bool_ > >
            >
          >::type* = 0
        ) const {
          const auto llvm_type = get_llvm_type( context, to );
          const std::shared_ptr< llvm::LLVMContext > &context_ = context;
          return expression(
            to, llvm_type,
            std::shared_ptr< llvm::Value >(
              ir_builder->CreateIntCast( value.llvm_value().get(), llvm_type.get(), type_traits::is_signed( from ) ),
              [context_]( llvm::Value* ){}
            )
          );
        }
        template< typename From, typename To >
        expression operator()(
          const From &from, const To &to,
          typename boost::enable_if<
            boost::mpl::and_<
              type_traits::meta::is_floating_point< From >,
              type_traits::meta::is_floating_point< To >
            >
          >::type* = 0
        ) const {
          const auto llvm_type = get_llvm_type( context, to );
          const std::shared_ptr< llvm::LLVMContext > &context_ = context;
          return expression(
            from, llvm_type,
            std::shared_ptr< llvm::Value >(
              ir_builder->CreateFPCast( value.llvm_value().get(), llvm_type.get() ),
              [context_]( llvm::Value* ){}
            )
          );
        }
        template< typename From, typename To >
        expression operator()(
          const From &, const To &,
          typename boost::enable_if<
            boost::mpl::not_<
              type_traits::meta::is_arithmetic_convertible< From, To >
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
    expression arithmetic_cast(
      const std::shared_ptr< llvm::LLVMContext > &context,
      const std::shared_ptr< ir_builder_t > &ir_builder,
      const type cast_to,
      const expression &value
    ) {
      return apply_visitor( node::arithmetic_cast( context, ir_builder, value ), value.type(), cast_to );
    }
  }
}

