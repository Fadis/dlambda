#include <array>
#include <vector>
#include <memory>
#include <boost/array.hpp>
#include <boost/variant/recursive_variant.hpp>
#include <boost/variant/static_visitor.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <boost/type_traits/is_float.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/type_traits/is_signed.hpp>
#include <boost/type_traits/is_pointer.hpp>
#include <boost/type_traits/is_array.hpp>
#include <boost/type_traits/remove_pointer.hpp>
#include <boost/mpl/is_sequence.hpp>
#include <boost/fusion/include/mpl.hpp>
#include <boost/mpl/pop_back.hpp>
#include <boost/mpl/back.hpp>
#include <llvm/IR/Type.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/ADT/ArrayRef.h>
#include <llvm/ADT/StringRef.h>
#include <dlambda/type.hpp>
#include <dlambda/get_type.hpp>

namespace dlambda {
  namespace compiler {
    namespace detail {
      class is_reference : public boost::static_visitor< bool > {
      public:
        template< typename T >
        bool operator()( const T& ) const {
          return 
            boost::mpl::or_<
              boost::is_same< T, types::lref< type > >,
              boost::is_same< T, types::rref< type > >
            >::value;
        }
      };
      class get_llvm_type : public boost::static_visitor< std::shared_ptr< llvm::Type > > {
      public:
        get_llvm_type( const std::shared_ptr< llvm::LLVMContext > &context_ ) : context( context_ ) {}
        std::shared_ptr< llvm::Type > operator()( const types::any_type& ) {
          throw -1;
        }
        std::shared_ptr< llvm::Type > operator()( const types::void_& ) {
          std::shared_ptr< llvm::LLVMContext > context_ = context;
          return std::shared_ptr< llvm::Type >( llvm::Type::getVoidTy( *context ), [context_]( llvm::Type* ){} );
        }
        std::shared_ptr< llvm::Type > operator()( const types::bool_ & ) {
          std::shared_ptr< llvm::LLVMContext > context_ = context;
          return std::shared_ptr< llvm::Type >( llvm::Type::getIntNTy( *context, 1u ), [context_]( llvm::Type* ){} );
        }
        std::shared_ptr< llvm::Type > operator()( const types::enum_ & ) {
          std::shared_ptr< llvm::LLVMContext > context_ = context;
          return std::shared_ptr< llvm::Type >( llvm::Type::getIntNTy( *context, 32u ), [context_]( llvm::Type* ){} );
        }
        std::shared_ptr< llvm::Type > operator()( const types::char_& ) {
          std::shared_ptr< llvm::LLVMContext > context_ = context;
          return std::shared_ptr< llvm::Type >( llvm::Type::getIntNTy( *context, 8u ), [context_]( llvm::Type* ){} );
        }
        std::shared_ptr< llvm::Type > operator()( const types::wchar_t_& ) {
          std::shared_ptr< llvm::LLVMContext > context_ = context;
          return std::shared_ptr< llvm::Type >( llvm::Type::getIntNTy( *context, 32u ), [context_]( llvm::Type* ){} );
        }
        std::shared_ptr< llvm::Type > operator()( const types::char16_t_& ) {
          std::shared_ptr< llvm::LLVMContext > context_ = context;
          return std::shared_ptr< llvm::Type >( llvm::Type::getIntNTy( *context, 16u ), [context_]( llvm::Type* ){} );
        }
        std::shared_ptr< llvm::Type > operator()( const types::char32_t_& ) {
          std::shared_ptr< llvm::LLVMContext > context_ = context;
          return std::shared_ptr< llvm::Type >( llvm::Type::getIntNTy( *context, 32u ), [context_]( llvm::Type* ){} );
        }
        std::shared_ptr< llvm::Type > operator()( const types::int_ &type_ ) {
          std::shared_ptr< llvm::LLVMContext > context_ = context;
          return std::shared_ptr< llvm::Type >( llvm::Type::getIntNTy( *context, type_.length ), [context_]( llvm::Type* ){} );
        }
        std::shared_ptr< llvm::Type > operator()( const types::complex< type > &type_ ) {
          std::shared_ptr< llvm::LLVMContext > context_ = context;
          const auto element_type = apply_visitor( *this, type_.base );
          return std::shared_ptr< llvm::Type >( llvm::ArrayType::get( element_type.get(), 2u ), [context_,element_type]( llvm::Type* ){} );
        }
        std::shared_ptr< llvm::Type > operator()( const types::float_ &type_ ) {
          std::shared_ptr< llvm::LLVMContext > context_ = context;
          if( type_.length == 16u )
            return std::shared_ptr< llvm::Type >( llvm::Type::getHalfTy( *context ), [context_]( llvm::Type* ){} );
          else if( type_.length == 32u )
            return std::shared_ptr< llvm::Type >( llvm::Type::getFloatTy( *context ), [context_]( llvm::Type* ){} );
          else if( type_.length == 64u )
            return std::shared_ptr< llvm::Type >( llvm::Type::getDoubleTy( *context ), [context_]( llvm::Type* ){} );
          else if( type_.length == 80u )
            return std::shared_ptr< llvm::Type >( llvm::Type::getX86_FP80Ty( *context ), [context_]( llvm::Type* ){} );
          else if( type_.length == 128u )
            return std::shared_ptr< llvm::Type >( llvm::Type::getFP128Ty( *context ), [context_]( llvm::Type* ){} );
          else
            throw -1;
        }
        std::shared_ptr< llvm::Type > operator()( const types::pointer< type > &type_ ) {
          std::shared_ptr< llvm::LLVMContext > context_ = context;
          const auto element_type = apply_visitor( *this, type_.base );
          return std::shared_ptr< llvm::Type >( llvm::PointerType::getUnqual( element_type.get() ), [context_,element_type]( llvm::Type* ){} );
        }
        std::shared_ptr< llvm::Type > operator()( const types::lref< type > &type_ ) {
          if( apply_visitor( is_reference(), type_.base ) )
            throw -1;
          std::shared_ptr< llvm::LLVMContext > context_ = context;
          const auto element_type = apply_visitor( *this, type_.base );
          return std::shared_ptr< llvm::Type >( llvm::PointerType::getUnqual( element_type.get() ), [context_,element_type]( llvm::Type* ){} );
        }
        std::shared_ptr< llvm::Type > operator()( const types::rref< type > &type_ ) {
          if( apply_visitor( is_reference(), type_.base ) )
            throw -1;
          std::shared_ptr< llvm::LLVMContext > context_ = context;
          const auto element_type = apply_visitor( *this, type_.base );
          return std::shared_ptr< llvm::Type >( llvm::PointerType::getUnqual( element_type.get() ), [context_,element_type]( llvm::Type* ){} );
        }
        std::shared_ptr< llvm::Type > operator()( const types::member_pointer< type > & ) {
          std::shared_ptr< llvm::LLVMContext > context_ = context;
          return std::shared_ptr< llvm::Type >( llvm::Type::getIntNTy( *context, sizeof( void* )*8u ), [context_]( llvm::Type* ){} );
        }
        std::shared_ptr< llvm::Type > operator()( const types::array< type > &type_ ) {
          std::shared_ptr< llvm::LLVMContext > context_ = context;
          const auto element_type = apply_visitor( *this, type_.base );
          return std::shared_ptr< llvm::Type >( llvm::ArrayType::get( element_type.get(), type_.length ), [context_,element_type]( llvm::Type* ){} );
        }
        std::shared_ptr< llvm::Type > operator()( const types::vector< type > &type_ ) {
          std::shared_ptr< llvm::LLVMContext > context_ = context;
          const auto element_type = apply_visitor( *this, type_.base );
          return std::shared_ptr< llvm::Type >( llvm::VectorType::get( element_type.get(), type_.length ), [context_,element_type]( llvm::Type* ){} );
        }
        std::shared_ptr< llvm::Type > operator()( const types::tuple< type > &type_ ) {
          std::shared_ptr< llvm::LLVMContext > context_ = context;
          std::shared_ptr< std::vector< std::shared_ptr< llvm::Type > > > element_types(
            new std::vector< std::shared_ptr< llvm::Type > >( type_.base.size() )
          );
          std::shared_ptr< std::vector< llvm::Type* > > raw_element_types(
            new std::vector< llvm::Type* >( type_.base.size() )
          );
          std::transform(
            type_.base.begin(), type_.base.end(), std::back_inserter( *element_types ),
            [this]( const type &type_ ) {
				          return apply_visitor( *this, type_ );
				        }
          );
          std::transform(
            element_types->begin(), element_types->end(), std::back_inserter( *raw_element_types ),
            [this]( const std::shared_ptr< llvm::Type > &type_ ) {
				          return type_.get();
				        }
          );
          if( type_.name.empty() )
            return std::shared_ptr< llvm::Type >( llvm::StructType::get( *context_, llvm::ArrayRef< llvm::Type* >( *raw_element_types ), type_.packed ), [context_,element_types]( llvm::Type* ){} );
          else {
            std::shared_ptr< std::string > name( new std::string( type_.name ) );
            return std::shared_ptr< llvm::Type >( llvm::StructType::create( *context_, llvm::ArrayRef< llvm::Type* >( *raw_element_types ), llvm::StringRef( *name ), type_.packed ), [context_,element_types,name]( llvm::Type* ){} );
          }
        }
        std::shared_ptr< llvm::Type > operator()( const types::user_defined_type & ) {
          std::shared_ptr< llvm::LLVMContext > context_ = context;
          return std::shared_ptr< llvm::Type >( llvm::PointerType::getUnqual( llvm::Type::getInt8Ty( *context ) ), [context_]( llvm::Type* ){} );
        }
        std::shared_ptr< llvm::Type > operator()( const types::function< type > &type_ ) {
          std::shared_ptr< llvm::LLVMContext > context_ = context;
          std::shared_ptr< std::vector< std::shared_ptr< llvm::Type > > > argument_types(
            new std::vector< std::shared_ptr< llvm::Type > >( type_.args_type.size() )
          );
          std::shared_ptr< std::vector< llvm::Type* > > raw_argument_types(
            new std::vector< llvm::Type* >( type_.args_type.size() )
          );
          std::transform(
            type_.args_type.begin(), type_.args_type.end(), std::back_inserter( *argument_types ),
            [this]( const type &type_ ) {
				          return apply_visitor( *this, type_ );
				        }
          );
          std::transform(
            argument_types->begin(), argument_types->end(), std::back_inserter( *raw_argument_types ),
            [this]( const std::shared_ptr< llvm::Type > &type_ ) {
				          return type_.get();
				        }
          );
          const auto result_type = apply_visitor( *this, type_.result_type );
          if( !raw_argument_types->empty() )
            return std::shared_ptr< llvm::Type >(
              llvm::FunctionType::get(
                result_type.get(),
                llvm::ArrayRef< llvm::Type* >( *raw_argument_types ),
                type_.variable
              ),
              [context_,result_type,argument_types]( llvm::Type* ){}
            );
          else {
            return std::shared_ptr< llvm::Type >(
              llvm::FunctionType::get(
                result_type.get(),
                type_.variable
              ),
              [context_,result_type,argument_types]( llvm::Type* ){}
            );
          }
        }
      private:
        std::shared_ptr< llvm::LLVMContext > context;
      };
    }
    std::shared_ptr< llvm::Type > get_llvm_type(
      const std::shared_ptr< llvm::LLVMContext > &context,
      const type &type_
    ) {
      detail::get_llvm_type visitor( context );
      return apply_visitor( visitor, type_ );
    }
  }
}

