#include <boost/mpl/not.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/variant/static_visitor.hpp>
#include <dlambda/type.hpp>
#include <dlambda/type_traits/is_signed.hpp>

namespace dlambda {
  namespace type_traits {
    namespace detail {
      class is_signed : public boost::static_visitor< bool > {
      public:
        template< typename T >
        bool operator()( const T &type_,
          typename boost::enable_if<
            boost::mpl::or_<
              boost::is_same< T, types::int_ >,
              boost::is_same< T, types::enum_ >
            >
          >::type* = 0
        ) const {
          return type_.signed_;
        }
        template< typename T >
        bool operator()( const T &type_,
          typename boost::enable_if<
            boost::mpl::or_<
              boost::is_same< T, types::char_ >,
              boost::is_same< T, types::wchar_t_ >,
              boost::is_same< T, types::char16_t_ >,
              boost::is_same< T, types::char32_t_ >
            >
          >::type* = 0
        ) const {
          if( type_.signed_ )
            return *type_.signed_;
          else
            return false;
        }
        template< typename T >
        bool operator()( const T &,
          typename boost::enable_if<
            boost::mpl::not_<
              boost::mpl::or_<
                boost::mpl::or_<
                  boost::is_same< T, types::int_ >,
                  boost::is_same< T, types::enum_ >,
                  boost::is_same< T, types::char_ >,
                  boost::is_same< T, types::wchar_t_ >,
                  boost::is_same< T, types::char16_t_ >
                >,
                boost::is_same< T, types::char32_t_ >
              >
            >
          >::type* = 0
        ) const {
          return false;
        }
      };
    }
    bool is_signed( const type &type_ ) {
      return apply_visitor( detail::is_signed(), type_ );
    }
  }
}

