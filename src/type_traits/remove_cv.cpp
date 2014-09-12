#include <boost/mpl/or.hpp>
#include <boost/mpl/not.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/variant/static_visitor.hpp>
#include <dlambda/type.hpp>
#include <dlambda/type_traits/remove_cv.hpp>
#include <dlambda/type_traits/remove_const.hpp>
#include <dlambda/type_traits/remove_volatile.hpp>

namespace dlambda {
  namespace type_traits {
    type remove_cv( const type &type_ ) {
      return remove_volatile( remove_const( type_ ) );
    }
  }
}

