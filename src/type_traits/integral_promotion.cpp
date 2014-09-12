#include <boost/mpl/or.hpp>
#include <boost/mpl/not.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/variant/static_visitor.hpp>
#include <dlambda/type.hpp>
#include <dlambda/type_traits/integral_promotion.hpp>
#include <dlambda/type_traits/is_integral.hpp>
#include <dlambda/type_traits/is_promotable.hpp>

namespace dlambda {
  namespace type_traits {
    namespace detail {
      class integral_promotion : public boost::static_visitor< type > {
      public:
        type operator()( const types::int_ &type_ ) const {
          return types::int_( type_.const_, type_.volatile_, std::max( sizeof( int ) * 8u, type_.length ), type_.signed_ );
        }
        type operator()( const types::enum_ &type_ ) const {
          return types::int_( type_.const_, type_.volatile_, std::max( sizeof( int ) * 8u, type_.length ), true );
        }
        type operator()( const types::bool_ &type_ ) const {
          return types::int_( type_.const_, type_.volatile_, sizeof( int ) * 8u, false );
        }
        type operator()( const types::char_ &type_ ) const {
          if( type_.signed_ )
            return types::int_( type_.const_, type_.volatile_, sizeof( int ) * 8u, *type_.signed_ );
          else
            return types::int_( type_.const_, type_.volatile_, sizeof( int ) * 8u, false );
        }
        type operator()( const types::wchar_t_ &type_ ) const {
          if( type_.signed_ )
            return types::int_( type_.const_, type_.volatile_, sizeof( int ) * 8u, *type_.signed_ );
          else
            return types::int_( type_.const_, type_.volatile_, sizeof( int ) * 8u, false );
        }
        type operator()( const types::char16_t_ &type_ ) const {
          if( type_.signed_ )
            return types::int_( type_.const_, type_.volatile_, sizeof( int ) * 8u, *type_.signed_ );
          else
            return types::int_( type_.const_, type_.volatile_, sizeof( int ) * 8u, false );
        }
        type operator()( const types::char32_t_ &type_ ) const {
          if( type_.signed_ )
            return types::int_( type_.const_, type_.volatile_, sizeof( int ) * 8u, *type_.signed_ );
          else
            return types::int_( type_.const_, type_.volatile_, sizeof( int ) * 8u, false );
        }
        template< typename T >
        type operator()( const T &type_,
          typename boost::enable_if<
            boost::mpl::not_<
              meta::is_promotable< T >
            >
          >::type* = 0
        ) const {
          return type_;
        }
      };
    }
    type integral_promotion( const type &type_ ) {
      return apply_visitor( detail::integral_promotion(), type_ );
    }
  }
}

