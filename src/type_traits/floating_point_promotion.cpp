#include <boost/mpl/or.hpp>
#include <boost/mpl/not.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/variant/static_visitor.hpp>
#include <dlambda/type.hpp>
#include <dlambda/type_traits/floating_point_promotion.hpp>

namespace dlambda {
  namespace type_traits {
    namespace detail {
      class floating_point_promotion : public boost::static_visitor< type > {
      public:
        type operator()( const types::float_ &type_
        ) const {
          return types::float_( type_.const_, type_.volatile_, 64u, type_.ordered );
        }
        template< typename T >
        type operator()( const T &type_,
          typename boost::enable_if<
            boost::mpl::not_<
              boost::is_same< T, types::float_ >
            >
          >::type* = 0
        ) const {
          return type_;
        }
      };
    }
    type floating_point_promotion( const type &type_ ) {
      return apply_visitor( detail::floating_point_promotion(), type_ );
    }
  }
}


