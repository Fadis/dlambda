#include <boost/mpl/or.hpp>
#include <boost/mpl/not.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/variant/static_visitor.hpp>
#include <dlambda/type.hpp>
#include <dlambda/type_traits/add_const.hpp>
#include <dlambda/type_traits/has_const.hpp>

namespace dlambda {
  namespace type_traits {
    namespace detail {
      class add_const : public boost::static_visitor< type > {
      public:
        template< typename T >
        type operator()( const T &type_,
          typename boost::enable_if<
            has_const_< T >
          >::type* = 0
        ) const {
          T temp = type_;
          temp.const_ = true;
          return temp;
        }
        template< typename T >
        type operator()( const T &type_,
          typename boost::enable_if<
            boost::mpl::not_<
              has_const_< T >
            >
          >::type* = 0
        ) const {
          return type_;
        }
      };
    }
    type add_const( const type &type_ ) {
      return apply_visitor( detail::add_const(), type_ );
    }
  }
}

