#ifndef DLAMBDA_GET_VALUE_HPP
#define DLAMBDA_GET_VALUE_HPP

#include <array>
#include <vector>
#include <memory>
#include <typeinfo>
#include <boost/array.hpp>
#include <boost/preprocessor/repeat.hpp>
#include <boost/preprocessor/enum.hpp>
#include <boost/variant/recursive_variant.hpp>
#include <boost/variant/static_visitor.hpp>
#include <boost/range/iterator.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/type_traits.hpp>
#include <boost/function_types/function_type.hpp>
#include <boost/function_types/function_arity.hpp>
#include <boost/function_types/result_type.hpp>
#include <boost/mpl/is_sequence.hpp>
#include <boost/fusion/include/mpl.hpp>
#include <boost/mpl/pop_back.hpp>
#include <boost/mpl/back.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/not.hpp>
#include <dlambda/type.hpp>
#include <dlambda/value.hpp>

namespace dlambda {
  template< typename T >
  value get_value(
    const T &value_,
    typename boost::enable_if<
      boost::mpl::or_<
        boost::is_same< T, bool >,
        boost::is_same< T, float >,
        boost::is_same< T, double >,
        boost::is_same< T, long double >
      >
    >::type* = 0
  ) {
    return value_;
  }
  template< typename T >
  value get_value(
    const T &value_,
    typename boost::enable_if<
      boost::mpl::and_<
        boost::mpl::or_<
          boost::is_integral< T >,
          boost::is_enum< T >
        >,
        boost::mpl::not_<
          boost::is_same< boost::remove_cv< T >, bool >
        >,
        boost::is_signed< T >
      >
    >::type* = 0
  ) {
    return int64_t( value_ );
  }
  template< typename T >
  value get_value(
    const T &value_,
    typename boost::enable_if<
      boost::mpl::and_<
        boost::mpl::or_<
          boost::is_integral< T >,
          boost::is_enum< T >
        >,
        boost::mpl::not_<
          boost::is_same< typename boost::remove_cv< T >::type, bool >
        >,
        boost::mpl::not_<
          boost::is_signed< T >
        >
      >
    >::type* = 0
  ) {
    return uint64_t( value_ );
  }
  template< typename T >
  value get_value(
    const T &value_,
    typename boost::enable_if<
      boost::is_pointer< T >
    >::type* = 0
  ) {
    return static_cast< void* >( value_ );
  }
  template< typename T >
  value get_value(
    const T &value_,
    typename boost::enable_if<
      boost::mpl::or_<
        boost::is_reference< T >,
        boost::is_rvalue_reference< T >
      >
    >::type* = 0
  ) {
    return static_cast< void* >( &value_ );
  }
  template< typename T >
  value get_value( const T &value_, typename boost::enable_if< utility::is_array< T > >::type* = 0 ) {
    std::vector< value > converted;
    std::transform(
      utility::get_begin( value_ ),
      utility::get_end( value_ ),
      std::back_inserter( converted ),
      [&]( const typename boost::range_value< T >::type &elem ) {
        return get_value( elem );
      }
    );
    return converted;
  }
  namespace detail {
    class get_values_in_sequence {
    public:
#define DLAMBDA_GET_VALUE_IN_SEQUENCE_ARGUMENT( z, index, size ) \
  boost::fusion::at_c< BOOST_PP_DEC( BOOST_PP_SUB( size, index ) ) >( values )
#define DLAMBDA_GET_VALUE_IN_SEQUENCE_EXPAND( z, index, unused ) \
      template< typename T > \
      std::vector< value > operator()( \
        const T &values, \
        typename boost::enable_if< \
          boost::mpl::equal_to< \
            typename boost::mpl::size< typename T::types >::type, \
            boost::mpl::size_t< index > \
          > \
        >::type* = 0 \
      ) { \
        return convert( \
          BOOST_PP_ENUM( index, DLAMBDA_GET_VALUE_IN_SEQUENCE_ARGUMENT, index ) \
        ); \
      }
      BOOST_PP_REPEAT( FUSION_MAX_VECTOR_SIZE, DLAMBDA_GET_VALUE_IN_SEQUENCE_EXPAND, )
    private:
      std::vector< value > convert() {
        return std::vector< value >();
      }
      template< typename Head, typename ...Tail >
      std::vector< value > convert( const Head &head, const Tail & ... tail ) {
        return convert( tail... ).emplace_back( get_value( head ) );
      }
    };
  }
  template< typename T >
  value get_value(
    const T &value_,
    typename boost::enable_if<
      boost::mpl::is_sequence< T >
    >::type* = 0 ) {
    return detail::get_values_in_sequence()( value_ );
  }
  template< typename T >
  value get_value(
    const T &value_,
    typename boost::enable_if<
      typename boost::is_function< T >::type
    >::type* = 0 ) {
    return static_cast< void* >( value_ );
  }
  template< typename T >
  value get_value(
    const T &value_,
    typename boost::enable_if<
      boost::is_member_function_pointer< T >
    >::type* = 0
  ) {
    throw -3;
  }
  template< typename T >
  value get_value(
    const T &value_,
    typename boost::enable_if<
      boost::is_member_pointer< T >
    >::type* = 0
  ) {
    throw -3;
  }
  template< typename T >
  value get_value(
    const T &,
    typename boost::enable_if<
      boost::is_same< T, types::void_ >
    >::type* = 0
  ) {
    return value();
  }
  template< typename T >
  value get_value(
    const T &,
    typename boost::enable_if<
      boost::is_same< T, types::any_type >
    >::type* = 0
  ) {
    return value();
  }
  template< typename T >
  value get_value(
    const T &value_,
    typename boost::enable_if<
      boost::mpl::not_<
        boost::mpl::bool_<
          boost::is_function< T >::value ||
          boost::is_member_pointer< T >::value ||
          boost::is_fundamental< T >::value ||
          boost::is_enum< T >::value ||
          utility::is_array< T >::value ||
          boost::is_lvalue_reference< T >::value ||
          boost::is_rvalue_reference< T >::value ||
          boost::is_pointer< T >::value ||
          boost::is_integral< T >::value ||
          boost::is_float< T >::value ||
          boost::mpl::is_sequence< T >::value ||
          boost::is_same< T, types::any_type >::value
        >
      >
    >::type* = 0 ) {
    return static_cast< void* >( value_ );
  }
}

#endif
