#include <boost/mpl/not.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/variant/static_visitor.hpp>
#include <dlambda/type.hpp>
#include <dlambda/type_traits/is_signed.hpp>

namespace dlambda {
  namespace type_traits {
    namespace detail {
      class is_union : public boost::static_visitor< bool > {
      public:
        bool operator()( const types::user_defined_type &type_ ) const {
          return type_.union_;
        }
        template< typename T >
        bool operator()( const T &,
          typename boost::enable_if<
            boost::mpl::not_<
              boost::is_same< T, types::user_defined_type >
            >
          >::type* = 0
        ) const {
          return false;
        }
      };
    }
    bool is_union( const type &type_ ) {
      return apply_visitor( detail::is_union(), type_ );
    }
  }
}

