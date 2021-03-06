#include <boost/mpl/or.hpp>
#include <boost/mpl/not.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/variant/static_visitor.hpp>
#include <dlambda/type.hpp>
#include <dlambda/type_traits/is_function.hpp>
#include <dlambda/type_traits/is_member_object_pointer.hpp>

namespace dlambda {
  namespace type_traits {
    namespace detail {
      class is_member_object_pointer : public boost::static_visitor< bool > {
      public:
        bool operator()( const types::member_pointer< type > &type_ ) const {
          return !is_function( type_.value_type );
        }
        template< typename T >
        bool operator()( const T &,
          typename boost::enable_if<
            boost::mpl::not_<
              boost::is_same< T, types::member_pointer< type > >
            >
          >::type* = 0
        ) const {
          return false;
        }
      };
    }
    bool is_member_object_pointer( const type &type_ ) {
      return apply_visitor( detail::is_member_object_pointer(), type_ );
    }
  }
}

