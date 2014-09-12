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
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/IRBuilder.h>
#include <dlambda/type.hpp>
#include <dlambda/value.hpp>
#include <dlambda/compiler/get_llvm_type.hpp>
#include <dlambda/compiler/get_llvm_constant_value.hpp>

namespace dlambda {
  namespace compiler {
    namespace detail {
      class get_llvm_constant_value : public boost::static_visitor< std::shared_ptr< llvm::Constant > > {
      public:
        get_llvm_constant_value(
          const std::shared_ptr< llvm::LLVMContext > &context_,
          const std::shared_ptr< llvm::Type > &llvm_type_
        ) : context( context_ ), llvm_type( llvm_type_ ) {}
        template< typename ValueType >
        std::shared_ptr< llvm::Constant > operator()( const types::any_type&, const ValueType& ) {
          throw -1;
        }
        template< typename ValueType >
        std::shared_ptr< llvm::Constant > operator()( const types::void_&, const ValueType& ) {
          throw -1;
        }
        template< typename T, typename ValueType >
        std::shared_ptr< llvm::Constant > operator()(
          const T&, bool value_,
          typename boost::enable_if<
            boost::is_same< T, types::bool_ >
          >::type* = 0
        ) {
          std::shared_ptr< llvm::LLVMContext > context_ = context;
          if( value_ )
            return std::shared_ptr< llvm::Constant >( llvm::ConstantInt::getTrue( *context ), [context_]( llvm::ConstantInt* ){} );
          else
            return std::shared_ptr< llvm::Constant >( llvm::ConstantInt::getFalse( *context ), [context_]( llvm::Constant* ){} );
        }
        template< typename T, typename ValueType >
        std::shared_ptr< llvm::Constant > operator()(
          const T &type_, ValueType value_,
          typename boost::enable_if<
            boost::mpl::and_<
              boost::mpl::or_<
                boost::is_same< T, types::int_ >,
                boost::is_same< T, types::enum_ >
              >,
              boost::is_integral< ValueType >,
              boost::mpl::not_<
                boost::is_same< typename boost::remove_cv< ValueType >::type, bool >
              >
            >
          >::type* = 0
        ) {
          std::shared_ptr< llvm::LLVMContext > context_ = context;
          if( type_.signed_ ) {
            return std::shared_ptr< llvm::Constant >( llvm::ConstantInt::getSigned( llvm_type.get(), value_ ), [context_]( llvm::Constant* ){} );
          }
          else {
            return std::shared_ptr< llvm::Constant >( llvm::ConstantInt::get( llvm_type.get(), value_, type_.signed_ ), [context_]( llvm::Constant* ){} );
          }
        }
        template< typename T, typename ValueType >
        std::shared_ptr< llvm::Constant > operator()(
          const T &type_, ValueType value_, 
          typename boost::enable_if<
            boost::mpl::and_<
              boost::mpl::or_<
                boost::is_same< T, types::char_ >,
                boost::is_same< T, types::wchar_t_ >,
                boost::is_same< T, types::char16_t_ >,
                boost::is_same< T, types::char32_t_ >
              >,
              boost::is_integral< ValueType >,
              boost::mpl::not_<
                boost::is_same< typename boost::remove_cv< ValueType >::type, bool >
              >
            >
          >::type* = 0
        ) {
          std::shared_ptr< llvm::LLVMContext > context_ = context;
          const bool signed_ = type_.signed_ ? *type_.signed_ : boost::is_signed< char >::value;
          if( signed_ ) {
            return std::shared_ptr< llvm::Constant >( llvm::ConstantInt::getSigned( llvm_type.get(), value_ ), [context_]( llvm::Constant* ){} );
          }
          else { 
            return std::shared_ptr< llvm::Constant >( llvm::ConstantInt::get( llvm_type.get(), value_, false ), [context_]( llvm::Constant* ){} );
          }
        }
        template< typename T, typename ValueType >
        std::shared_ptr< llvm::Constant > operator()(
          const T &type,
          const ValueType &value,
          typename boost::enable_if<
            boost::mpl::and_<
              boost::is_same< T, types::complex< dlambda::type > >,
              boost::is_same< ValueType, std::vector< dlambda::value > >
            >
          >::type* = 0
        ) {
          if( value.size() != 2u )
            throw -4;
          const llvm::ArrayRef< llvm::Constant* > ref(
            std::vector< llvm::Constant* >{
              convert( type.base, value[ 0 ] ).get(),
              convert( type.base, value[ 1 ] ).get()
            }
          );
          std::shared_ptr< llvm::LLVMContext > context_ = context;
          return std::shared_ptr< llvm::Constant >(
            llvm::ConstantStruct::get(
              llvm::ConstantStruct::getTypeForElements( *context, ref, false ),
              ref
            ),
            [context_]( llvm::Constant* ){}
          );
        }
        template< typename T, typename ValueType >
        std::shared_ptr< llvm::Constant > operator()(
          const T &type, ValueType value,
          typename boost::enable_if<
            boost::mpl::and_<
              boost::is_same< T, types::float_ >,
              boost::is_same< ValueType, float >
            >
          >::type* = 0
        ) {
          std::shared_ptr< llvm::LLVMContext > context_ = context;
          return std::shared_ptr< llvm::Constant >(
            llvm::ConstantFP::get( get_llvm_type( context, type ).get(), value ),
            [context_]( llvm::Constant* ){}
          );
        }
        template< typename T, typename ValueType >
        std::shared_ptr< llvm::Constant > operator()(
          const T &type, ValueType value,
          typename boost::enable_if<
            boost::mpl::and_<
              boost::is_same< T, types::float_ >,
              boost::is_same< ValueType, double >
            >
          >::type* = 0
        ) {
          std::shared_ptr< llvm::LLVMContext > context_ = context;
          return std::shared_ptr< llvm::Constant >(
            llvm::ConstantFP::get( get_llvm_type( context, type ).get(), value ),
            [context_]( llvm::Constant* ){}
          );
        }
        template< typename T, typename ValueType >
        std::shared_ptr< llvm::Constant > operator()(
          const T &type, ValueType value,
          typename boost::enable_if<
            boost::mpl::and_<
              boost::is_same< T, types::float_ >,
              boost::is_same< ValueType, long double >
            >
          >::type* = 0
        ) {
          std::shared_ptr< llvm::LLVMContext > context_ = context;
          // FIXME: long double will be casted to double
          return std::shared_ptr< llvm::Constant >(
            llvm::ConstantFP::get( get_llvm_type( context, type ).get(), value ),
            [context_]( llvm::Constant* ){}
          );
        }
        template< typename T, typename ValueType >
        std::shared_ptr< llvm::Constant > operator()(
          const T &, ValueType value,
          typename boost::enable_if<
            boost::mpl::and_<
              boost::is_same< T, types::pointer< dlambda::type > >,
              boost::is_same< ValueType, void* >
            >
          >::type* = 0
        ) {
          std::shared_ptr< llvm::LLVMContext > context_ = context;
          return std::shared_ptr< llvm::Constant >(
            llvm::ConstantInt::get( get_llvm_type( context, get_type< std::intptr_t >() ).get(), uint64_t( std::intptr_t( value ) ), false ),
            [context_]( llvm::Constant* ){}
          );
        }
        template< typename T, typename ValueType >
        std::shared_ptr< llvm::Constant > operator()(
          const T &, ValueType value,
          typename boost::enable_if<
            boost::mpl::and_<
              boost::is_same< T, types::lref< dlambda::type > >,
              boost::is_same< ValueType, void* >
            >
          >::type* = 0
        ) {
          std::shared_ptr< llvm::LLVMContext > context_ = context;
          return std::shared_ptr< llvm::Constant >(
            llvm::ConstantInt::get( get_llvm_type( context, get_type< std::intptr_t >() ).get(), uint64_t( std::intptr_t( value ) ), false ),
            [context_]( llvm::Constant* ){}
          );
        }
        template< typename T, typename ValueType >
        std::shared_ptr< llvm::Constant > operator()(
          const T &, ValueType value,
          typename boost::enable_if<
            boost::mpl::and_<
              boost::is_same< T, types::rref< dlambda::type > >,
              boost::is_same< ValueType, void* >
            >
          >::type* = 0
        ) {
          std::shared_ptr< llvm::LLVMContext > context_ = context;
          return std::shared_ptr< llvm::Constant >(
            llvm::ConstantInt::get( get_llvm_type( context, get_type< std::intptr_t >() ).get(), uint64_t( std::intptr_t( value ) ), false ),
            [context_]( llvm::Constant* ){}
          );
        }
        template< typename T, typename ValueType >
        std::shared_ptr< llvm::Constant > operator()(
          const T &, ValueType value,
          typename boost::enable_if<
            boost::mpl::and_<
              boost::is_same< T, types::member_pointer< dlambda::type > >,
              boost::is_same< ValueType, boost::any >
            >
          >::type* = 0
        ) {
          std::shared_ptr< llvm::LLVMContext > context_ = context;
          return std::shared_ptr< llvm::Constant >(
            llvm::ConstantInt::get( get_llvm_type( context, get_type< std::intptr_t >() ).get(), std::intptr_t( &value ), false ),
            [context_]( llvm::Constant* ){}
          );
        }
        template< typename T, typename ValueType >
        std::shared_ptr< llvm::Constant > operator()(
          const T &type, const ValueType &value,
          typename boost::enable_if<
            boost::mpl::and_<
              boost::is_same< T, types::array< dlambda::type > >,
              boost::is_same< ValueType, std::vector< dlambda::value > >
            >
          >::type* = 0
        ) {
          if( type.length != value.size() )
            throw -5;
          std::vector< llvm::Constant* > elements;
          elements.reserve( value.size() );
          std::transform(
            value.begin(), value.end(),
            std::back_inserter( elements ),
            [&]( const dlambda::value &value ) {
              return convert( type.base, value ).get();
            }
          );
          const llvm::ArrayRef< llvm::Constant* > ref( elements );
          std::shared_ptr< llvm::LLVMContext > context_ = context;
          return std::shared_ptr< llvm::Constant >(
            llvm::ConstantArray::get( llvm::ArrayType::get( get_llvm_type( context, type.base ).get(), elements.size() ), ref ),
            [context_]( llvm::Constant* ){}
          );
        }
        template< typename T, typename ValueType >
        std::shared_ptr< llvm::Constant > operator()(
          const T &type, const ValueType &value,
          typename boost::enable_if<
            boost::mpl::and_<
              boost::is_same< T, types::vector< dlambda::type > >,
              boost::is_same< ValueType, std::vector< dlambda::value > >
            >
          >::type* = 0
        ) {
          if( type.length != value.size() )
            throw -5;
          std::vector< llvm::Constant* > elements;
          elements.reserve( value.size() );
          std::transform(
            value.begin(), value.end(),
            std::back_inserter( elements ),
            [&]( const dlambda::value &value ) {
              return convert( type.base, value ).get();
            }
          );
          const llvm::ArrayRef< llvm::Constant* > ref( elements );
          std::shared_ptr< llvm::LLVMContext > context_ = context;
          return std::shared_ptr< llvm::Constant >(
            llvm::ConstantVector::get( ref ),
            [context_]( llvm::Constant* ){}
          );
        }
        template< typename T, typename ValueType >
        std::shared_ptr< llvm::Constant > operator()(
          const T &type, const ValueType &value,
          typename boost::enable_if<
            boost::mpl::and_<
              boost::is_same< T, types::tuple< dlambda::type > >,
              boost::is_same< ValueType, std::vector< dlambda::value > >
            >
          >::type* = 0
        ) {
          if( type.base.size() != value.size() )
            throw -5;
          std::vector< llvm::Constant* > elements;
          elements.reserve( value.size() );
          for( size_t index = 0; index != value.size(); ++index )
            elements.emplace_back( convert( type.base[ index ], value[ index ] ).get() );
          const llvm::ArrayRef< llvm::Constant* > ref( elements );
          std::shared_ptr< llvm::LLVMContext > context_ = context;
          return std::shared_ptr< llvm::Constant >(
            llvm::ConstantStruct::get(
              llvm::ConstantStruct::getTypeForElements( *context, ref, false ),
              ref
            ),
            [context_]( llvm::Constant* ){}
          );
        }
        template< typename T, typename ValueType >
        std::shared_ptr< llvm::Constant > operator()(
          const T &, const ValueType &value,
          typename boost::enable_if<
            boost::mpl::and_<
              boost::is_same< T, types::user_defined_type >,
              boost::is_same< ValueType, void* >
            >
          >::type* = 0
        ) {
          std::shared_ptr< llvm::LLVMContext > context_ = context;
          return std::shared_ptr< llvm::Constant >(
            llvm::ConstantInt::get( get_llvm_type( context, get_type< std::intptr_t >() ).get(), std::intptr_t( &value ), false ),
            [context_]( llvm::Constant* ){}
          );
        }
        template< typename T, typename ValueType >
        std::shared_ptr< llvm::Constant > operator()(
          const T &, const ValueType &value,
          typename boost::enable_if<
            boost::mpl::and_<
              boost::is_same< T, types::function< dlambda::type > >,
              boost::is_same< ValueType, void* >
            >
          >::type* = 0
        ) {
          std::shared_ptr< llvm::LLVMContext > context_ = context;
          return std::shared_ptr< llvm::Constant >(
            llvm::ConstantInt::get( get_llvm_type( context, get_type< std::intptr_t >() ).get(), std::intptr_t( &value ), false ),
            [context_]( llvm::Constant* ){}
          );
        }
        std::shared_ptr< llvm::Constant > operator()( const dlambda::type &type, const dlambda::value &value ) {
          std::cout << type.which() << ' ' << value.which() << std::endl;
          throw -3;
        }
      private:
        std::shared_ptr< llvm::Constant > convert(
          const dlambda::type &type, const dlambda::value &value
        ) const {
          const auto type_ = get_llvm_type( context, type );
          detail::get_llvm_constant_value constant_generator( context, type_ );
          return apply_visitor( constant_generator, type, value );
        }
        std::shared_ptr< llvm::LLVMContext > context;
        std::shared_ptr< llvm::Type > llvm_type;
      };
    }
    std::pair< std::shared_ptr< llvm::Type >, std::shared_ptr< llvm::Constant > > get_llvm_constant_value(
      const std::shared_ptr< llvm::LLVMContext > &context,
      const proto::literal &value
    ) {
      const auto type_ = get_llvm_type( context, value.type() );
      detail::get_llvm_constant_value constant_generator( context, type_ );
      return std::make_pair( type_, apply_visitor( constant_generator, value.type(), value.value() ) );
    }
  }
}

