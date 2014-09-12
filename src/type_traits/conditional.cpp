#include <boost/mpl/or.hpp>
#include <boost/mpl/not.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/variant/static_visitor.hpp>
#include <dlambda/type.hpp>
#include <dlambda/type_traits/conditional.hpp>

namespace dlambda {
  namespace type_traits {
    type conditional( bool cond, const type &if_true, const type &if_false ) {
      return cond ? if_true : if_false;
    }
  }
}

