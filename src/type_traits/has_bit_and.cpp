#include <boost/mpl/or.hpp>
#include <boost/mpl/not.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/variant/static_visitor.hpp>
#include <dlambda/type.hpp>
#include <dlambda/type_traits/has_bit_and.hpp>

namespace dlambda {
  namespace type_traits {
    bool has_bit_and( const type& ) {
      throw -1;
    }
    bool has_bit_and( const type&, const type& ) {
      throw -1;
    }
    bool has_bit_and( const type&, const type&, const type& ) {
      throw -1;
    }
  }
}

