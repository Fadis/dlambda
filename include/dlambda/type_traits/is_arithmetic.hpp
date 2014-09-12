#ifndef DLAMBDA_TYPE_TRAITS_IS_ARITHMETIC_HPP
#define DLAMBDA_TYPE_TRAITS_IS_ARITHMETIC_HPP

#include <dlambda/type.hpp>
#include <dlambda/type_traits/is_integral.hpp>
#include <dlambda/type_traits/is_floating_point.hpp>

namespace dlambda {
  namespace type_traits {
    namespace meta {
      template< typename Type >
      struct is_arithmetic :
        public boost::mpl::or_<
          is_integral< Type >,
          is_floating_point< Type >
        > {};
    }
    bool is_arithmetic( const type& );
  }
}

#endif

