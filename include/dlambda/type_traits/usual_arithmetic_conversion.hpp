#ifndef DLAMBDA_TYPE_TRAITS_USUAL_ARITHMETIC_CONVERSION_HPP
#define DLAMBDA_TYPE_TRAITS_USUAL_ARITHMETIC_CONVERSION_HPP

#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/or.hpp>
#include <dlambda/type.hpp>
#include <dlambda/type_traits/is_promotable.hpp>

namespace dlambda {
  namespace type_traits {
    namespace meta {
      struct undefined {};
      template< typename Left, typename Right = undefined >
      struct is_arithmetic_convertible :
        public boost::mpl::and_<
          is_arithmetic_convertible< Left >,
          is_arithmetic_convertible< Right >
        > {};
      template< typename Type >
      struct is_arithmetic_convertible< Type, undefined > :
        public boost::mpl::or_<
          is_promotable< Type >,
          boost::is_same< Type, types::float_ >
        > {};
    }
    type usual_arithmetic_conversion( const type &left, const type &right );
  }
}

#endif

