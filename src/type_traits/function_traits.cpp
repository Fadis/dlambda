#include <boost/mpl/or.hpp>
#include <boost/mpl/not.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/variant/static_visitor.hpp>
#include <dlambda/type.hpp>
#include <dlambda/type_traits/function_traits.hpp>
#include <dlambda/exceptions.hpp>

namespace dlambda {
  namespace type_traits {
    namespace detail {
      class function_traits : public boost::static_visitor< types::function< type > > {
      public:
        types::function< type > operator()( const types::function< type > &type_ ) const {
          return type_;
        }
        template< typename T >
        types::function< type > operator()( const T &,
          typename boost::enable_if<
            boost::mpl::not_<
              boost::is_same< T, types::function< type > >
            >
          >::type* = 0
        ) const {
          throw exceptions::unexpected_type();
        }
      };
    }
    types::function< type > function_traits( const type &type_ ) {
      return apply_visitor( detail::function_traits(), type_ );
    }
  }
}

