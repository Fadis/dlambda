#ifndef DLAMBDA_PROTO_LITERAL_HPP
#define DLAMBDA_PROTO_LITERAL_HPP

#include <boost/range/algorithm/equal.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/mpl/and.hpp>
#include <boost/utility/enable_if.hpp>
#include <dlambda/type.hpp>
#include <dlambda/value.hpp>
#include <dlambda/get_type.hpp>
#include <dlambda/get_value.hpp>

namespace dlambda {
  namespace proto {
    class literal {
    public:
      literal() : value_type_( get_type< int >() ), value_( get_value( 0 ) ) {}
      template< typename T >
      explicit literal(
        const T &value__,
        typename boost::enable_if<
          boost::mpl::not_< boost::is_same< literal, T > >
        >::type* = 0
      ) : value_type_( get_type< T >() ), value_( get_value< T >( value__ ) ) {}
      template< typename T >
      explicit literal(
        T &value__,
        typename boost::enable_if<
          boost::mpl::and_<
            boost::mpl::not_< boost::is_const< T > >,
            boost::mpl::not_< boost::is_same< literal, T > >
          >
        >::type* = 0
      ) : value_type_( get_type< T& >() ), value_( get_value< T& >( value__ ) ) {}
      literal( const literal & ) = default;
      literal( literal && ) = default;
      literal &operator=( const literal & ) = default;
      literal &operator=( literal && ) = default;
      const dlambda::type &type() const { return value_type_; } 
      const dlambda::value &value() const { return value_; }
    private:
      dlambda::type value_type_;
      dlambda::value value_;
    };
  }
}

#endif


