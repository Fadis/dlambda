#include <boost/mpl/or.hpp>
#include <boost/mpl/not.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/variant/static_visitor.hpp>
#include <boost/variant/get.hpp>
#include <dlambda/type.hpp>
#include <dlambda/type_traits/integral_promotion.hpp>
#include <dlambda/type_traits/usual_arithmetic_conversion.hpp>

namespace dlambda {
  namespace type_traits {
    namespace detail {
      class usual_arithmetic_conversion : public boost::static_visitor< type > {
      public:
        type operator()( const types::int_ &left, const types::int_ &right ) const {
          return integral_conversion( left, right );
        }
        type operator()( const types::enum_ &left, const types::enum_ right ) const {
          if( left.name == right.name )
            return types::enum_( false, false, left.length, left.signed_, left.name );
          else throw -1;
        }
        type operator()( const types::bool_ &left, const types::bool_ &right ) const {
          return integral_conversion( left, right );
        }
        type operator()( const types::char_ &left, const types::char_ &right ) const {
          return integral_conversion( left, right );
        }
        type operator()( const types::wchar_t_ &left, const types::wchar_t_ &right ) const {
          return integral_conversion( left, right );
        }
        type operator()( const types::char16_t_ &left, const types::char16_t_ &right ) const {
          return integral_conversion( left, right );
        }
        type operator()( const types::char32_t_ &left, const types::char32_t_ &right ) const {
          return integral_conversion( left, right );
        }
        template< typename Left, typename Right >
        type operator()(
          const Left &left, const Right &right,
          typename boost::enable_if<
            boost::mpl::and_<
              meta::is_promotable< Left >,
              meta::is_promotable< Right >,
              boost::mpl::not_< boost::is_same< Left, Right > >
            >
          >::type* = 0
        ) const {
          return integral_conversion( left, right );
        }
        type operator()(
          const types::float_ &left, const types::float_ &right
        ) const {
          return types::float_( false, false, std::max( left.length, right.length ), false );
        }
        template< typename Other >
        type operator()(
          const types::float_ &left, const Other &,
          typename boost::enable_if<
            boost::mpl::and_<
              boost::mpl::not_< boost::is_same< Other, types::enum_ > >,
              boost::mpl::not_< boost::is_same< Other, types::float_ > >
            >
          >::type* = 0
        ) const {
          return types::float_( false, false, left.length, false );
        }
        type operator()(
          const types::float_ &, const types::enum_ &
        ) const {
          throw -1;
        }
        template< typename Other >
        type operator()(
          const Other &, const types::float_ &right,
          typename boost::enable_if<
            boost::mpl::and_<
              boost::mpl::not_< boost::is_same< Other, types::enum_ > >,
              boost::mpl::not_< boost::is_same< Other, types::float_ > >
            >
          >::type* = 0
        ) const {
          return types::float_( false, false, right.length, false );
        }
        type operator()(
          const types::enum_ &, const types::float_ &
        ) const {
          throw -1;
        }
        template< typename Left, typename Right >
        type operator()(
          const Left &, const Right &,
          typename boost::enable_if<
            boost::mpl::or_<
              boost::mpl::not_<
                meta::is_arithmetic_convertible< Left >
              >,
              boost::mpl::not_<
                meta::is_arithmetic_convertible< Right >
              >
            >
          >::type* = 0
        ) const {
          throw -1;
        }
      private:
        template< typename Left, typename Right >
        type integral_conversion(
          const Left &left, const Right &right
        ) const {
          const auto left_ = boost::get< types::int_ >( dlambda::type_traits::integral_promotion( left ) );
          const auto right_ = boost::get< types::int_ >( dlambda::type_traits::integral_promotion( right ) );
          if( left_.length > right_.length )
            return types::int_( false, false, left_.length, left_.signed_ );
          else if( left_.length < right_.length )
            return types::int_( false, false, right_.length, right_.signed_ );
          else {
            if( !left_.signed_ || !right_.signed_ )
              return types::int_( false, false, left_.length, false );
            else
              return types::int_( false, false, left_.length, true );
          }
        }
      };
    }
    type usual_arithmetic_conversion( const type &left, const type &right ) {
      return apply_visitor( detail::usual_arithmetic_conversion(), left, right );
    }
  }
}

