#include <boost/mpl/or.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/has_xxx.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/variant/static_visitor.hpp>
#include <dlambda/type.hpp>
#include <dlambda/type_traits/decay.hpp>
#include <dlambda/type_traits/remove_reference.hpp>

namespace dlambda {
  namespace type_traits {
    namespace detail {
      class decay : public boost::static_visitor< type > {
      public:
        type operator()( const types::array< type > &type_ ) const {
          return types::pointer< type >( type_.const_, type_.volatile_, type_.base );
        }
        type operator()( const types::function< type > &type_ ) const {
          return types::pointer< type >( false, false, type_ );
        }
        template< typename T >
        type operator()( const T &type_,
          typename boost::enable_if<
            boost::mpl::not_<
              boost::is_same< T, types::array< type > >,
              boost::is_same< T, types::function< type > >
            >
          >::type* = 0
        ) const {
          return type_;
        }
      };
    }
    type decay( const type &type_ ) {
      return apply_visitor( detail::decay(), remove_reference( type_ ) );
    }
  }
}

