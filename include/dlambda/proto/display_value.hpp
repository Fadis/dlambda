#ifndef DLAMBDA_PROTO_DISPLAY_VALUE_HPP
#define DLAMBDA_PROTO_DISPLAY_VALUE_HPP

#include <vector>
#include <string>
#include <boost/range/algorithm/copy.hpp>
#include <boost/variant/static_visitor.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <boost/type_traits/is_float.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/type_traits/is_signed.hpp>
#include <boost/type_traits/is_unsigned.hpp>
#include <boost/type_traits/is_pointer.hpp>
#include <boost/type_traits/is_array.hpp>
#include <boost/spirit/include/karma.hpp>
#include <dlambda/type.hpp>
#include <dlambda/value.hpp>
#include <dlambda/type_traits/is_promotable.hpp>
#include <dlambda/exceptions.hpp>

namespace dlambda {
  namespace proto {
    namespace detail {
      template< typename OutputIterator >
      class display_value : public boost::static_visitor< void > {
      public:
        display_value( OutputIterator &out_ ) : out( out_ ) {}
        template< typename ValueType >
        void operator()( const types::any_type&, const ValueType& ) {
          boost::copy( std::string( "any" ), out );
        }
        template< typename ValueType >
        void operator()( const types::void_&, const ValueType& ) {
          boost::copy( std::string( "void" ), out );
        }
        template< typename T, typename ValueType >
        void operator()(
          const T&, bool value_,
          typename boost::enable_if<
            boost::is_same< T, types::bool_ >
          >::type* = 0
        ) {
          namespace karma = boost::spirit::karma;
          karma::generate( out, karma::bool_, value_ );
        }
        template< typename T, typename ValueType >
        void operator()(
          const T &, const ValueType &value_,
          typename boost::enable_if<
            boost::mpl::and_<
              type_traits::meta::is_promotable< T >,
              boost::is_integral< ValueType >,
              boost::mpl::not_< boost::is_same< ValueType, bool > >,
              boost::mpl::not_< boost::is_same< T, types::bool_ > >,
              boost::is_signed< ValueType >
            >
          >::type* = 0
        ) {
          namespace karma = boost::spirit::karma;
          karma::generate( out, karma::long_long, value_ );
        }
        template< typename T, typename ValueType >
        void operator()(
          const T &, const ValueType &value_,
          typename boost::enable_if<
            boost::mpl::and_<
              type_traits::meta::is_promotable< T >,
              boost::is_integral< ValueType >,
              boost::mpl::not_< boost::is_same< ValueType, bool > >,
              boost::mpl::not_< boost::is_same< T, types::bool_ > >,
              boost::is_unsigned< ValueType >
            >
          >::type* = 0
        ) {
          namespace karma = boost::spirit::karma;
          karma::generate( out, karma::ulong_long, value_ );
        }
        template< typename T, typename ValueType >
        void operator()(
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
            throw exceptions::invalid_value();
          apply( type.base, value[ 0 ] );
          boost::copy( std::string( "+" ), out );
          apply( type.base, value[ 1 ] );
          boost::copy( std::string( "i" ), out );
        }
        template< typename T, typename ValueType >
        void operator()(
          const T &, const ValueType &value,
          typename boost::enable_if<
            boost::mpl::and_<
              boost::is_same< T, types::float_ >,
              boost::is_same< ValueType, float >
            >
          >::type* = 0
        ) {
          namespace karma = boost::spirit::karma;
          karma::generate( out, karma::float_, value );
        }
        template< typename T, typename ValueType >
        void operator()(
          const T &, const ValueType &value,
          typename boost::enable_if<
            boost::mpl::and_<
              boost::is_same< T, types::float_ >,
              boost::is_same< ValueType, double >
            >
          >::type* = 0
        ) {
          namespace karma = boost::spirit::karma;
          karma::generate( out, karma::double_, value );
        }
        template< typename T, typename ValueType >
        void operator()(
          const T &, const ValueType &value,
          typename boost::enable_if<
            boost::mpl::and_<
              boost::is_same< T, types::float_ >,
              boost::is_same< ValueType, long double >
            >
          >::type* = 0
        ) {
          namespace karma = boost::spirit::karma;
          karma::generate( out, karma::long_double, value );
        }
        template< typename T, typename ValueType >
        void operator()(
          const T &, ValueType value,
          typename boost::enable_if<
            boost::mpl::and_<
              boost::is_same< T, types::pointer< dlambda::type > >,
              boost::is_same< ValueType, void* >
            >
          >::type* = 0
        ) {
          namespace karma = boost::spirit::karma;
          static const karma::uint_generator< uintptr_t, 16 > hex64;
          karma::generate( out, "0x" << hex64, uintptr_t( value ) );
        }
        template< typename T, typename ValueType >
        void operator()(
          const T &, ValueType value,
          typename boost::enable_if<
            boost::mpl::and_<
              boost::is_same< T, types::lref< dlambda::type > >,
              boost::is_same< ValueType, void* >
            >
          >::type* = 0
        ) {
          namespace karma = boost::spirit::karma;
          static const karma::uint_generator< uintptr_t, 16 > hex64;
          karma::generate( out, "0x" << hex64, uintptr_t( value ) );
        }
        template< typename T, typename ValueType >
        void operator()(
          const T &, ValueType value,
          typename boost::enable_if<
            boost::mpl::and_<
              boost::is_same< T, types::rref< dlambda::type > >,
              boost::is_same< ValueType, void* >
            >
          >::type* = 0
        ) {
          namespace karma = boost::spirit::karma;
          static const karma::uint_generator< uintptr_t, 16 > hex64;
          karma::generate( out, "0x" << hex64, uintptr_t( value ) );
        }
        template< typename T, typename ValueType >
        void operator()(
          const T &, ValueType,
          typename boost::enable_if<
            boost::mpl::and_<
              boost::is_same< T, types::member_pointer< dlambda::type > >,
              boost::is_same< ValueType, boost::any >
            >
          >::type* = 0
        ) {
          boost::copy( std::string( "member_pointer" ), out );
        }
        template< typename T, typename ValueType >
        void operator()(
          const T &type, const ValueType &value,
          typename boost::enable_if<
            boost::mpl::and_<
              boost::is_same< T, types::array< dlambda::type > >,
              boost::is_same< ValueType, std::vector< dlambda::value > >
            >
          >::type* = 0
        ) {
          if( type.length != value.size() )
            throw exceptions::invalid_value();
          boost::copy( std::string( "[" ), out );
          for( size_t index = 0; index != value.size(); ++index ) {
            apply( type.base, value[ index ] );
            if( index != value.size() - 1 )
              boost::copy( std::string( "," ), out );
          }
          boost::copy( std::string( "]" ), out );
        }
        template< typename T, typename ValueType >
        void operator()(
          const T &type, const ValueType &value,
          typename boost::enable_if<
            boost::mpl::and_<
              boost::is_same< T, types::vector< dlambda::type > >,
              boost::is_same< ValueType, std::vector< dlambda::value > >
            >
          >::type* = 0
        ) {
          if( type.length != value.size() )
            throw exceptions::invalid_value();
          boost::copy( std::string( "<" ), out );
          for( size_t index = 0; index != value.size(); ++index ) {
            apply( type.base, value[ index ] );
            if( index != value.size() - 1 )
              boost::copy( std::string( "," ), out );
          }
          boost::copy( std::string( ">" ), out );
        }
        template< typename T, typename ValueType >
        void operator()(
          const T &type, const ValueType &value,
          typename boost::enable_if<
            boost::mpl::and_<
              boost::is_same< T, types::tuple< dlambda::type > >,
              boost::is_same< ValueType, std::vector< dlambda::value > >
            >
          >::type* = 0
        ) {
          if( type.base.size() != value.size() )
            throw exceptions::invalid_value();
          boost::copy( std::string( "{" ), out );
          for( size_t index = 0; index != value.size(); ++index ) {
            apply( type.base[ index ], value[ index ] );
            if( index != value.size() - 1 )
              boost::copy( std::string( "," ), out );
          }
          boost::copy( std::string( "}" ), out );
        }
        template< typename T, typename ValueType >
        void operator()(
          const T &type, const ValueType &,
          typename boost::enable_if<
            boost::mpl::and_<
              boost::is_same< T, types::user_defined_type >,
              boost::is_same< ValueType, void* >
            >
          >::type* = 0
        ) {
          boost::copy( type.name, out );
        }
        template< typename T, typename ValueType >
        void operator()(
          const T &, const ValueType &value,
          typename boost::enable_if<
            boost::mpl::and_<
              boost::is_same< T, types::function< dlambda::type > >,
              boost::is_same< ValueType, void* >
            >
          >::type* = 0
        ) {
          namespace karma = boost::spirit::karma;
          static const karma::uint_generator< uintptr_t, 16 > hex64;
          karma::generate( out, "0x" << hex64, uintptr_t( value ) );
        }
        void operator()( const dlambda::type &, const dlambda::value & ) {
          throw exceptions::invalid_value();
        }
      private:
        void apply(
          const dlambda::type &type, const dlambda::value &value
        ) {
          return apply_visitor( *this, type, value );
        }
        OutputIterator &out;
      };
    }
    template< typename OutputIterator >
    void display_value(
      OutputIterator &out,
      const proto::literal &value
    ) {
      detail::display_value< OutputIterator > visitor( out );
      apply_visitor( visitor, value.type(), value.value() );
    }
  }
}

#endif

