#include <boost/variant.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/not.hpp>
#include <boost/utility/enable_if.hpp>
#include <dlambda/type_traits/match.hpp>
#include <dlambda/type_traits/is_same.hpp>

namespace dlambda {
  namespace type_traits {
    namespace detail {
      struct match : boost::static_visitor< bool > {
        template< typename Left, typename Right >
        bool operator()(
          const Left&, const Right&,
          typename boost::enable_if<
            boost::mpl::or_<
              boost::is_same< Left, any_type >,
              boost::is_same< Right, any_type >
            >
          >::type* = 0
        ) const {
          return true;
        }
        template< typename Left, typename Right >
        bool operator()(
          const Left &left, const Right &right,
          typename boost::enable_if<
            boost::mpl::not_<
              boost::mpl::or_<
                boost::is_same< Left, any_type >,
                boost::is_same< Right, any_type >
              >
            >
          >::type* = 0
        ) const {
          return is_same( left, right );
        }
      };
    }
    bool match( const type &left, const type &right ) {
      return apply_visitor( detail::match(), left, right );
    }
  }
}


