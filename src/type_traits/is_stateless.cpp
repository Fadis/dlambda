#include <boost/mpl/not.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/variant/static_visitor.hpp>
#include <dlambda/type.hpp>
#include <dlambda/type_traits/is_stateless.hpp>
#include <dlambda/type_traits/has_trivial_constructor.hpp>
#include <dlambda/type_traits/has_trivial_copy.hpp>
#include <dlambda/type_traits/has_trivial_destructor.hpp>
#include <dlambda/type_traits/is_class.hpp>
#include <dlambda/type_traits/is_empty.hpp>

namespace dlambda {
  namespace type_traits {
    bool is_stateless( const type &type_ ) {
      return has_trivial_constructor( type_ ) && has_trivial_copy( type_ ) && has_trivial_destructor( type_ ) && is_class( type_ ) && is_empty( type_ );
    }
  }
}

