#ifndef DLAMBDA_TYPE_TRAITS_IS_PROMOTABLE_HPP
#define DLAMBDA_TYPE_TRAITS_IS_PROMOTABLE_HPP

#include <boost/mpl/or.hpp>
#include <boost/type_traits/is_same.hpp>
#include <dlambda/type.hpp>
#include <dlambda/type_traits/is_integral.hpp>

namespace dlambda {
  namespace type_traits {
    namespace meta {
      template< typename Type >
      struct is_promotable :
        public boost::mpl::or_<
          is_integral< Type >,
          boost::is_same< Type, types::enum_ >
        > {};
    }
    bool is_promotable( const type &type_ );
  }
}

#endif

