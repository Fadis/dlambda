#include <dlambda/type.hpp>
#include <dlambda/type_traits/is_same.hpp>

namespace dlambda {
  namespace type_traits {
    bool is_same( const type &left, const type &right ) {
      return left == right;
    }
  }
}

