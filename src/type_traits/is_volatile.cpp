#include <boost/mpl/or.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/has_xxx.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/variant/static_visitor.hpp>
#include <dlambda/type.hpp>
#include <dlambda/type_traits/is_volatile.hpp>
#include <dlambda/type_traits/has_volatile.hpp>

namespace dlambda {
  namespace type_traits {
    namespace detail {
      class is_volatile : public boost::static_visitor< bool > {
      public:
        template< typename T >
        bool operator()( const T &type_,
          typename boost::enable_if<
            has_volatile_< T >
          >::type* = 0
        ) const {
          return type_.volatile_;
        }
        template< typename T >
        bool operator()( const T &,
          typename boost::enable_if<
            boost::mpl::not_<
              has_volatile_< T >
            >
          >::type* = 0
        ) const {
          return false;
        }
      };
    }
    bool is_volatile( const type &type_ ) {
      return apply_visitor( detail::is_volatile(), type_ );
    }
  }
}

