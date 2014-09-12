#include <boost/mpl/or.hpp>
#include <boost/mpl/not.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/variant/static_visitor.hpp>
#include <dlambda/type.hpp>
#include <dlambda/type_traits/extent.hpp>

namespace dlambda {
  namespace type_traits {
    namespace detail {
      class extent : public boost::static_visitor< size_t > {
      public:
        extent( size_t level_ ) : level( level_ ) {}
        template< typename T >
        size_t operator()( const T &type_,
          typename boost::enable_if<
            boost::mpl::or_<
              boost::is_same< T, types::array< type > >,
              boost::is_same< T, types::vector< type > >
            >
          >::type* = 0
        ) const {
          if( level == 0 )
            return type_.length;
          else
            return apply_visitor( extent( level - 1 ), type_.base );
        }
        template< typename T >
        size_t operator()( const T &,
          typename boost::enable_if<
            boost::mpl::not_<
              boost::mpl::or_<
                boost::is_same< T, types::array< type > >,
                boost::is_same< T, types::vector< type > >
              >
            >
          >::type* = 0
        ) const {
          return 0u;
        }
      private:
        size_t level;
      };
    }
    size_t extent( const type &type_, size_t level ) {
      return apply_visitor( detail::extent( level ), type_ );
    }
  }
}

