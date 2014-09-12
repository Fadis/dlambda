#ifndef DLAMBDA_UTILITY_IS_ARRAY_HPP
#define DLAMBDA_UTILITY_IS_ARRAY_HPP

#include <array>
#include <boost/array.hpp>
#include <boost/type_traits/is_array.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/size_t.hpp>
#include <boost/range/iterator.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <dlambda/type.hpp>

namespace dlambda {
  namespace utility {
    template< typename T >
    struct is_array : public boost::is_array< T > {};
    template< typename T, size_t size >
    struct is_array< std::array< T, size > > : boost::mpl::bool_< true > {};
    template< typename T, size_t size >
    struct is_array< boost::array< T, size > > : boost::mpl::bool_< true > {};
    template< typename T >
    struct get_value_type {};
    template< typename T, size_t size >
    struct get_value_type< std::array< T, size > > {
      typedef T type;
    };
    template< typename T, size_t size >
    struct get_value_type< boost::array< T, size > > {
      typedef T type;
    };
    template< typename T, size_t size >
    struct get_value_type< T[size] > {
      typedef T type;
    };
    template< typename T >
    struct get_array_size {};
    template< typename T, size_t size >
    struct get_array_size< std::array< T, size > > :
      public boost::mpl::size_t< size > {};
    template< typename T, size_t size >
    struct get_array_size< boost::array< T, size > > :
      public boost::mpl::size_t< size > {};
    template< typename T, size_t size >
    struct get_array_size< T[size] > :
      public boost::mpl::size_t< size > {};
    namespace detail {
      template< typename T >
      struct get_begin {
        typedef typename boost::range_iterator< T >::type result_type;
        result_type operator()( T &range ) const {
          return boost::begin( range );
        }
      };
      template< typename T, size_t size >
      struct get_begin< T[ size ] > {
        typedef T* result_type;
        result_type operator()( T &range ) const {
          return range;
        }
      };
      template< typename T >
      struct get_end {
        typedef typename boost::range_iterator< T >::type result_type;
        result_type operator()( T &range ) const {
          return boost::end( range );
        }
      };
      template< typename T, size_t size >
      struct get_end< T[ size ] > {
        typedef T* result_type;
        result_type operator()( T &range ) const {
          return range + size;
        }
      };
    }
    template< typename T >
    typename detail::get_begin< T >::result_type get_begin( T &range ) {
      return detail::get_begin< T >()( range );
    }
    template< typename T >
    typename detail::get_end< T >::result_type get_end( T &range ) {
      return detail::get_end< T >()( range );
    }
  }
}

#endif

