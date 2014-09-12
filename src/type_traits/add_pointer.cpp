#include <boost/mpl/or.hpp>
#include <boost/mpl/not.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/variant/static_visitor.hpp>
#include <dlambda/type.hpp>
#include <dlambda/type_traits/add_pointer.hpp>

namespace dlambda {
  namespace type_traits {
    namespace detail {
      class add_pointer : public boost::static_visitor< type > {
      public:
        template< typename T >
        type operator()( const T &type_,
          typename boost::enable_if<
            boost::mpl::or_<
              boost::is_same< T, types::lref< type > >,
              boost::is_same< T, types::rref< type > >
            >
          >::type* = 0
        ) const {
          return types::pointer< type >( false, false, false, type_.base );
        }
        template< typename T >
        type operator()( const T &type_,
          typename boost::enable_if<
            boost::mpl::not_<
              boost::mpl::or_<
                boost::is_same< T, types::lref< type > >,
                boost::is_same< T, types::rref< type > >
              >
            >
          >::type* = 0
        ) const {
          return types::pointer< type >( false, false, false, type_ );
        }
      };
    }
    type add_pointer( const type &type_ ) {
      return apply_visitor( detail::add_pointer(), type_ );
    }
  }
}

