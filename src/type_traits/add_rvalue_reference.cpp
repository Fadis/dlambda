#include <boost/mpl/or.hpp>
#include <boost/mpl/not.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/variant/static_visitor.hpp>
#include <dlambda/type.hpp>
#include <dlambda/type_traits/add_rvalue_reference.hpp>

namespace dlambda {
  namespace type_traits {
    namespace detail {
      class add_rvalue_reference : public boost::static_visitor< type > {
      public:
        template< typename T >
        type operator()( const T &type_,
          typename boost::enable_if<
            boost::mpl::or_<
              boost::is_same< T, types::void_ >,
              boost::is_same< T, types::lref< type > >,
              boost::is_same< T, types::rref< type > >,
              boost::is_same< T, types::function< type > >,
              boost::is_same< T, types::member_pointer< type > >
            >
          >::type* = 0
        ) const {
          return type_;
        }
        template< typename T >
        type operator()( const T &type_,
          typename boost::enable_if<
            boost::mpl::not_<
              boost::mpl::or_<
                boost::is_same< T, types::void_ >,
                boost::is_same< T, types::lref< type > >,
                boost::is_same< T, types::rref< type > >,
                boost::is_same< T, types::function< type > >,
                boost::is_same< T, types::member_pointer< type > >
              >
            >
          >::type* = 0
        ) const {
          return types::rref< type >( type_ );
        }
      };
    }
    type add_rvalue_reference( const type &type_ ) {
      return apply_visitor( detail::add_rvalue_reference(), type_ );
    }
  }
}

