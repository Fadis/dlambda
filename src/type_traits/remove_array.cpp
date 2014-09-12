#include <boost/mpl/or.hpp>
#include <boost/mpl/not.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/variant/static_visitor.hpp>
#include <dlambda/type.hpp>
#include <dlambda/type_traits/is_array.hpp>

namespace dlambda {
  namespace type_traits {
    namespace detail {
      class remove_array : public boost::static_visitor< type > {
      public:
        template< typename T >
        type operator()( const T &type_,
          typename boost::enable_if<
            meta::is_array< T >
          >::type* = 0
        ) const {
          return type_.base;
        }
        template< typename T >
        type operator()( const T &type_,
          typename boost::enable_if<
            boost::mpl::not_<
              meta::is_array< T >
            >
          >::type* = 0
        ) const {
          return type_;
        }
      };
    }
    type remove_array( const type &type_ ) {
      return apply_visitor( detail::remove_array(), type_ );
    }
  }
}

