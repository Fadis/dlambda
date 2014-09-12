#include <boost/mpl/or.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/has_xxx.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/variant/static_visitor.hpp>
#include <dlambda/type.hpp>
#include <dlambda/type_traits/is_aggregated.hpp>

namespace dlambda {
  namespace type_traits {
    namespace detail {
      class is_aggregated : public boost::static_visitor< bool > {
      public:
        template< typename T >
        bool operator()( const T &,
          typename boost::enable_if<
            meta::is_aggregated< T >
          >::type* = 0
        ) const {
          return true;
        }
        template< typename T >
        bool operator()( const T &,
          typename boost::enable_if<
            boost::mpl::not_<
              meta::is_aggregated< T >
            >
          >::type* = 0
        ) const {
          return false;
        }
      };
    }
    bool is_aggregated( const type &type_ ) {
      return apply_visitor( detail::is_aggregated(), type_ );
    }
  }
}

