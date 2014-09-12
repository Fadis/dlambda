#ifndef DLAMBDA_TYPE_TRAITS_IS_INTEGRAL_HPP
#define DLAMBDA_TYPE_TRAITS_IS_INTEGRAL_HPP

#include <dlambda/type.hpp>

namespace dlambda {
  namespace type_traits {
    namespace meta {
      template< typename Type >
      struct is_integral :
        public boost::mpl::or_<
          boost::mpl::or_<
            boost::is_same< Type, types::int_ >,
            boost::is_same< Type, types::bool_ >,
            boost::is_same< Type, types::char_ >,
            boost::is_same< Type, types::wchar_t_ >
          >,
          boost::mpl::or_<
            boost::is_same< Type, types::char16_t_ >,
            boost::is_same< Type, types::char32_t_ >
          >
        > {};
    }
    bool is_integral( const type& );
  }
}

#endif

