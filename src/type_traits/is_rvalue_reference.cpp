#include <boost/mpl/or.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/has_xxx.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/variant/static_visitor.hpp>
#include <dlambda/type.hpp>
#include <dlambda/type_traits/is_rvalue_reference.hpp>

namespace dlambda {
  namespace type_traits {
    namespace detail {
      class is_rvalue_reference : public boost::static_visitor< bool > {
      public:
        bool operator()( const types::rref< type >& ) const {
          return true;
        }
        template< typename T >
        bool operator()( const T &,
          typename boost::enable_if<
            boost::mpl::not_<
              boost::is_same< T, types::rref< type > >
            >
          >::type* = 0
        ) const {
          return false;
        }
      };
    }
    bool is_rvalue_reference( const type &type_ ) {
      return apply_visitor( detail::is_rvalue_reference(), type_ );
    }
  }
}

