#include <boost/mpl/or.hpp>
#include <boost/mpl/not.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/variant/static_visitor.hpp>
#include <dlambda/type.hpp>
#include <dlambda/type_traits/is_enum.hpp>

namespace dlambda {
  namespace type_traits {
    namespace detail {
      class is_enum : public boost::static_visitor< bool > {
      public:
        bool operator()( const types::enum_& ) const {
          return true;
        }
        template< typename T >
        bool operator()( const T &,
          typename boost::enable_if<
            boost::mpl::not_<
              boost::is_same< T, types::enum_ >
            >
          >::type* = 0
        ) const {
          return false;
        }
      };
    }
    bool is_enum( const type &type_ ) {
      return apply_visitor( detail::is_enum(), type_ );
    }
  }
}

