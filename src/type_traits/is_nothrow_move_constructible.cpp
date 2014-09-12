#include <boost/mpl/or.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/has_xxx.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/variant/static_visitor.hpp>
#include <dlambda/type.hpp>
#include <dlambda/type_traits/is_nothrow_move_constructible.hpp>

namespace dlambda {
  namespace type_traits {
    namespace detail {
      BOOST_MPL_HAS_XXX_TRAIT_DEF(base)
      class is_nothrow_move_constructible : public boost::static_visitor< bool > {
      public:
        bool operator()( const types::user_defined_type &type_ ) const {
          return type_.nothrow_move_constructible;
        }
        bool operator()( const types::tuple< type > &type_ ) const {
          return std::find_if(
            type_.base.begin(), type_.base.end(),
            [&]( const type &elem ) {
              return !apply_visitor( *this, elem );
            }
          ) == type_.base.end();
        }
        template< typename T >
        bool operator()( const T &type_,
          typename boost::enable_if<
            boost::mpl::and_<
              has_base< T >,
              boost::mpl::not_<
                boost::is_same< T, types::tuple< type > >
              >
            >
          >::type* = 0
        ) const {
          return apply_visitor( *this, type_.base );
        }
        template< typename T >
        bool operator()( const T &,
          typename boost::enable_if<
            boost::mpl::and_<
              boost::mpl::not_<
                has_base< T >
              >,
              boost::mpl::not_<
                boost::is_same< T, types::user_defined_type >
              >
            >
          >::type* = 0
        ) const {
          return true;
        }
      };
    }
    bool is_nothrw_move_constructible( const type &type_ ) {
      return apply_visitor( detail::is_nothrow_move_constructible(), type_ );
    }
  }
}

