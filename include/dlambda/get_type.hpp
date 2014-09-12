#ifndef DLAMBDA_GET_TYPE_HPP
#define DLAMBDA_GET_TYPE_HPP

#include <array>
#include <vector>
#include <memory>
#include <typeinfo>
#include <boost/array.hpp>
#include <boost/variant/recursive_variant.hpp>
#include <boost/variant/static_visitor.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/type_traits.hpp>
#include <boost/function_types/function_type.hpp>
#include <boost/function_types/function_arity.hpp>
#include <boost/function_types/parameter_types.hpp>
#include <boost/function_types/result_type.hpp>
#include <boost/mpl/is_sequence.hpp>
#include <boost/fusion/include/mpl.hpp>
#include <boost/mpl/pop_back.hpp>
#include <boost/mpl/back.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/not.hpp>
#include <dlambda/type.hpp>
#include <dlambda/utility/is_array.hpp>

namespace dlambda {
  template< typename T >
  type get_type( typename boost::enable_if<
    boost::mpl::and_<
      boost::mpl::and_<
        boost::is_integral< T >,
        boost::mpl::not_< boost::is_same< typename boost::remove_cv< T >::type, char > >,
        boost::mpl::not_< boost::is_same< typename boost::remove_cv< T >::type, signed char > >,
        boost::mpl::not_< boost::is_same< typename boost::remove_cv< T >::type, unsigned char > >,
        boost::mpl::not_< boost::is_same< typename boost::remove_cv< T >::type, wchar_t > >
      >,
      boost::mpl::and_<
        boost::mpl::not_< boost::is_same< typename boost::remove_cv< T >::type, bool > >,
        boost::mpl::not_< boost::is_same< typename boost::remove_cv< T >::type, char32_t > >,
        boost::mpl::not_< boost::is_same< typename boost::remove_cv< T >::type, char16_t > >
      >
    >
  >::type* = 0 ) {
    return types::int_( boost::is_const< T >::value, boost::is_volatile< T >::value, sizeof( T ) * 8u, boost::is_signed< T >::value );
  }
  template< typename T >
  type get_type( typename boost::enable_if< boost::is_same< typename boost::remove_cv< T >::type, void > >::type* = 0 ) {
    return types::void_();
  }
  template< typename T >
  type get_type( typename boost::enable_if< boost::is_same< typename boost::remove_cv< T >::type, bool > >::type* = 0 ) {
    return types::bool_( boost::is_const< T >::value, boost::is_volatile< T >::value );
  }
  template< typename T >
  type get_type( typename boost::enable_if< boost::is_same< typename boost::remove_cv< T >::type, char > >::type* = 0 ) {
    return types::char_( boost::is_const< T >::value, boost::is_volatile< T >::value );
  }
  template< typename T >
  type get_type( typename boost::enable_if< boost::is_same< typename boost::remove_cv< T >::type, signed char > >::type* = 0 ) {
    return types::char_( boost::is_const< T >::value, boost::is_volatile< T >::value, true );
  }
  template< typename T >
  type get_type( typename boost::enable_if< boost::is_same< typename boost::remove_cv< T >::type, unsigned char > >::type* = 0 ) {
    return types::char_( boost::is_const< T >::value, boost::is_volatile< T >::value, false );
  }
  template< typename T >
  type get_type( typename boost::enable_if< boost::is_same< typename boost::remove_cv< T >::type, wchar_t > >::type* = 0 ) {
    return types::wchar_t_( boost::is_const< T >::value, boost::is_volatile< T >::value, boost::is_signed< T >::value );
  }
  template< typename T >
  type get_type( typename boost::enable_if< boost::is_same< typename boost::remove_cv< T >::type, char16_t > >::type* = 0 ) {
    return types::char16_t_( boost::is_const< T >::value, boost::is_volatile< T >::value, boost::is_signed< T >::value );
  }
  template< typename T >
  type get_type( typename boost::enable_if< boost::is_same< typename boost::remove_cv< T >::type, char32_t > >::type* = 0 ) {
    return types::char32_t_( boost::is_const< T >::value, boost::is_volatile< T >::value, boost::is_signed< T >::value );
  }
  template< typename T >
  type get_type( typename boost::enable_if< boost::is_enum< T > >::type* = 0 ) {
    return types::enum_( boost::is_const< T >::value, boost::is_volatile< T >::value, sizeof( T ) * 8u, boost::is_signed< T >::value, typeid( T ).name() );
  }
  template< typename T >
  type get_type( typename boost::enable_if< boost::is_float< T > >::type* = 0 ) {
    return types::float_( boost::is_const< T >::value, boost::is_volatile< T >::value, sizeof( T ) * 8u, false );
  }

  template< typename T >
  type get_type( typename boost::enable_if< utility::is_array< T > >::type* = 0 );
  template< typename T >
  type get_type(
    typename boost::enable_if<
      boost::mpl::is_sequence< T >
    >::type* = 0 );
  
  template< typename T >
  type get_type( typename boost::enable_if< boost::is_pointer< T > >::type* = 0 ) {
    return types::pointer< type >( boost::is_const< T >::value, boost::is_volatile< T >::value, false, get_type< typename boost::remove_pointer< T >::type >() );
  }
  template< typename T >
  type get_type( typename boost::enable_if< boost::mpl::and_< boost::is_reference< T >, boost::mpl::not_< boost::is_rvalue_reference< T > > > >::type* = 0 ) {
    return types::lref< type >( get_type< typename boost::remove_reference< T >::type >() );
  }
  template< typename T >
  type get_type( typename boost::enable_if< boost::mpl::and_< boost::is_reference< T >, boost::is_rvalue_reference< T > > >::type* = 0 ) {
    return types::rref< type >( get_type< typename boost::remove_reference< T >::type >() );
  }
  template< typename T >
  type get_type( typename boost::enable_if< utility::is_array< T > >::type* = 0 ) {
    return types::array< type >( boost::is_const< T >::value, boost::is_volatile< T >::value, utility::get_array_size< T >::value, get_type< typename utility::get_value_type< T >::type >() );
  }

  template< typename T >
  std::vector< type > get_types_in_sequence( typename boost::enable_if< boost::mpl::empty< T > >::type* = 0 ) {
    return std::vector< type >();
  }
  template< typename T >
  std::vector< type > get_types_in_sequence( typename boost::enable_if< boost::mpl::not_< boost::mpl::empty< T > > >::type* = 0 ) {
    auto temp = get_types_in_sequence< typename boost::mpl::pop_back< T >::type >();
    temp.emplace_back( get_type< typename boost::mpl::back< T >::type >() );
    return std::move( temp );
  }
  template< typename T >
  type get_type(
    typename boost::enable_if<
      boost::mpl::is_sequence< T >
    >::type* = 0 ) {
    return types::tuple< type >( boost::is_const< T >::value, boost::is_volatile< T >::value, get_types_in_sequence< T >() );
  }
  template< typename T >
  type get_type(
    typename boost::enable_if<
      typename boost::is_function< T >::type
    >::type* = 0 ) {
    return types::function< type >(
      boost::is_const< T >::value, boost::is_volatile< T >::value,
      false,
      get_type< typename boost::function_types::result_type< T >::type >(),
      get_types_in_sequence< typename boost::function_types::parameter_types< T >::type >()
    );
  }
  template< typename T >
  type get_type(
    typename boost::enable_if<
      boost::is_member_function_pointer< T >
    >::type* = 0
  ) {
    return types::function< type >(
      boost::is_const< T >::value, boost::is_volatile< T >::value,
      false,
      get_type< typename boost::function_types::result_type< T >::type >(),
      get_types_in_sequence< typename boost::function_types::parameter_types< T >::type >()
    );
  }
  template< typename T >
  struct get_this_type {};
  template< typename T, typename U >
  struct get_this_type< T(U::*) > {
    typedef U type; 
  };
  template< typename T >
  struct get_member_type {};
  template< typename T, typename U >
  struct get_member_type< T(U::*) > {
    typedef T type; 
  };
  template< typename T >
  type get_type(
    typename boost::enable_if<
      boost::is_member_pointer< T >
    >::type* = 0
  ) {
    return types::member_pointer< type >(
      get_type< get_this_type< T >::type >(),
      get_type< get_member_type< T >::type >()
    );
  }
  template< typename T >
  type get_type(
    typename boost::enable_if<
      boost::is_same< T, types::any_type >
    >::type* = 0
  ) {
    return types::any_type();
  }
  template< typename T >
  type get_type(
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
    return types::user_defined_type(
        std::string( typeid( typename boost::remove_cv< T >::type ).name() ),
        boost::is_const< T >::value,
        boost::is_volatile< T >::value,
        boost::is_copy_constructible< T >::value,
        boost::is_empty< T >::value,
        boost::has_nothrow_assign< T >::value,
        boost::has_nothrow_copy< T >::value,
        boost::is_nothrow_move_assignable< T >::value,
        boost::is_nothrow_move_constructible< T >::value,
        boost::is_pod< T >::value,
        boost::is_polymorphic< T >::value,
        boost::is_stateless< T >::value,
        boost::is_class< T >::value,
        boost::is_union< T >::value,
        boost::has_trivial_assign< T >::value,
        boost::has_trivial_constructor< T >::value,
        boost::has_trivial_copy< T >::value,
        boost::has_trivial_destructor< T >::value
    );
  }
}

#endif
