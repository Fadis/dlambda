#include <boost/mpl/or.hpp>
#include <boost/mpl/not.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/variant/static_visitor.hpp>
#include <dlambda/type.hpp>
#include <dlambda/type_traits/add_cv.hpp>
#include <dlambda/type_traits/add_const.hpp>
#include <dlambda/type_traits/add_volatile.hpp>

namespace dlambda {
  namespace type_traits {
    type add_cv( const type &type_ ) {
      return add_volatile( add_const( type_ ) );
    }
  }
}

