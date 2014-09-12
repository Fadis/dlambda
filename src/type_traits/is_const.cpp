#include <boost/mpl/or.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/has_xxx.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/variant/static_visitor.hpp>
#include <dlambda/type.hpp>
#include <dlambda/type_traits/is_const.hpp>
#include <dlambda/type_traits/has_const.hpp>

namespace dlambda {
  namespace type_traits {
    namespace detail {
      class is_const : public boost::static_visitor< bool > {
      public:
        template< typename T >
        bool operator()( const T &type_,
          typename boost::enable_if<
            has_const_< T >
          >::type* = 0
        ) const {
          return type_.const_;
        }
        template< typename T >
        bool operator()( const T &,
          typename boost::enable_if<
            boost::mpl::not_<
              has_const_< T >
            >
          >::type* = 0
        ) const {
          return false;
        }
      };
    }
    bool is_const( const type &type_ ) {
      return apply_visitor( detail::is_const(), type_ );
    }
  }
}

