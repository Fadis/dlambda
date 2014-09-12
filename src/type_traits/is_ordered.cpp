#include <boost/mpl/or.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/has_xxx.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/variant/static_visitor.hpp>
#include <dlambda/type.hpp>
#include <dlambda/type_traits/is_arithmetic.hpp>
#include <dlambda/type_traits/is_ordered.hpp>
#include <dlambda/type_traits/has_ordered.hpp>

namespace dlambda {
  namespace type_traits {
    namespace detail {
      class is_ordered : public boost::static_visitor< bool > {
      public:
        template< typename T >
        bool operator()( const T &type_,
          typename boost::enable_if<
            has_ordered< T >
          >::type* = 0
        ) const {
          return type_.ordered;
        }
        template< typename T >
        bool operator()( const T &,
          typename boost::enable_if<
            boost::mpl::and_<
              meta::is_arithmetic< T >,
              boost::mpl::not_< has_ordered< T > >
            >
          >::type* = 0
        ) const {
          return true;
        }
        template< typename T >
        bool operator()( const T &,
          typename boost::enable_if<
            boost::mpl::not_<
              boost::mpl::or_<
                meta::is_arithmetic< T >,
                has_ordered< T >
              >
            >
          >::type* = 0
        ) const {
          return false;
        }
      };
    }
    bool is_ordered( const type &type_ ) {
      return apply_visitor( detail::is_ordered(), type_ );
    }
  }
}

