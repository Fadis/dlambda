#include <boost/mpl/or.hpp>
#include <boost/mpl/not.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/variant/static_visitor.hpp>
#include <dlambda/type.hpp>
#include <dlambda/type_traits/is_copy_constructible.hpp>

namespace dlambda {
  namespace type_traits {
    namespace detail {
      class is_copy_constructible : public boost::static_visitor< bool > {
      public:
        bool operator()( const types::user_defined_type &type_ ) const {
          return type_.copy_constructible;
        }
        bool operator()( const types::function< type >& ) const {
          return false;
        }
        template< typename T >
        bool operator()( const T &,
          typename boost::enable_if<
            boost::mpl::not_<
              boost::mpl::or_<
                boost::is_same< T, types::function< type > >,
                boost::is_same< T, types::user_defined_type >
              >
            >
          >::type* = 0
        ) const {
          return true;
        }
      };
    }
    bool is_copy_constructible( const type &type_ ) {
      return apply_visitor( detail::is_copy_constructible(), type_ );
    }
  }
}

