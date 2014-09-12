#ifndef DLAMBDA_PROTO_OPERATORS_HPP
#define DLAMBDA_PROTO_OPERATORS_HPP

#include <boost/variant.hpp>
#include <dlambda/proto/node/any.hpp>
#include <dlambda/proto/node/terminal.hpp>
#include <dlambda/proto/node/literal.hpp>
#include <dlambda/proto/node/placeholder.hpp>
#include <boost/proto/tags.hpp>
#include <boost/preprocessor.hpp>

#define DLAMBDA_PROTO_OUTSIDE 0
#define DLAMBDA_PROTO_INSIDE  1
#define DLAMBDA_PROTO_WITH_INT 0
#define DLAMBDA_PROTO_WITHOUT_INT 1
#define DLAMBDA_PROTO_OPERATOR_DEFINITION_SIZE 4
#define DLAMBDA_PROTO_OD_ELEM( index, oper ) \
  BOOST_PP_TUPLE_ELEM( DLAMBDA_PROTO_OPERATOR_DEFINITION_SIZE, index, oper )
#define DLAMBDA_PROTO_UNARY_OPERATORS \
  (( unary_plus, +, DLAMBDA_PROTO_OUTSIDE, DLAMBDA_PROTO_WITHOUT_INT )) \
  (( negate, -, DLAMBDA_PROTO_OUTSIDE , DLAMBDA_PROTO_WITHOUT_INT )) \
  (( dereference, *, DLAMBDA_PROTO_OUTSIDE , DLAMBDA_PROTO_WITHOUT_INT )) \
  (( complement, ~, DLAMBDA_PROTO_OUTSIDE , DLAMBDA_PROTO_WITHOUT_INT )) \
  (( address_of, &, DLAMBDA_PROTO_OUTSIDE , DLAMBDA_PROTO_WITHOUT_INT )) \
  (( logical_not, !, DLAMBDA_PROTO_OUTSIDE , DLAMBDA_PROTO_WITHOUT_INT )) \
  (( pre_inc, ++, DLAMBDA_PROTO_OUTSIDE , DLAMBDA_PROTO_WITHOUT_INT )) \
  (( pre_dec, --, DLAMBDA_PROTO_OUTSIDE , DLAMBDA_PROTO_WITHOUT_INT )) \
  (( post_inc, ++, DLAMBDA_PROTO_OUTSIDE , DLAMBDA_PROTO_WITH_INT )) \
  (( post_dec, --, DLAMBDA_PROTO_OUTSIDE , DLAMBDA_PROTO_WITH_INT ))
#define DLAMBDA_PROTO_BINARY_OPERATORS_WITHOUT_SUBSCRIPT \
  (( shift_left, <<, DLAMBDA_PROTO_OUTSIDE , DLAMBDA_PROTO_WITHOUT_INT )) \
  (( shift_right, >>, DLAMBDA_PROTO_OUTSIDE , DLAMBDA_PROTO_WITHOUT_INT )) \
  (( multiplies, *, DLAMBDA_PROTO_OUTSIDE , DLAMBDA_PROTO_WITHOUT_INT )) \
  (( divides, /, DLAMBDA_PROTO_OUTSIDE , DLAMBDA_PROTO_WITHOUT_INT )) \
  (( modulus, %, DLAMBDA_PROTO_OUTSIDE , DLAMBDA_PROTO_WITHOUT_INT )) \
  (( plus, +, DLAMBDA_PROTO_OUTSIDE , DLAMBDA_PROTO_WITHOUT_INT )) \
  (( minus, -, DLAMBDA_PROTO_OUTSIDE , DLAMBDA_PROTO_WITHOUT_INT )) \
  (( less, <, DLAMBDA_PROTO_OUTSIDE , DLAMBDA_PROTO_WITHOUT_INT )) \
  (( greater, >, DLAMBDA_PROTO_OUTSIDE , DLAMBDA_PROTO_WITHOUT_INT )) \
  (( less_equal, <=, DLAMBDA_PROTO_OUTSIDE , DLAMBDA_PROTO_WITHOUT_INT )) \
  (( greater_equal, >=, DLAMBDA_PROTO_OUTSIDE , DLAMBDA_PROTO_WITHOUT_INT )) \
  (( equal_to, ==, DLAMBDA_PROTO_OUTSIDE , DLAMBDA_PROTO_WITHOUT_INT )) \
  (( not_equal_to, !=, DLAMBDA_PROTO_OUTSIDE , DLAMBDA_PROTO_WITHOUT_INT )) \
  (( logical_or, ||, DLAMBDA_PROTO_OUTSIDE , DLAMBDA_PROTO_WITHOUT_INT )) \
  (( logical_and, &&, DLAMBDA_PROTO_OUTSIDE , DLAMBDA_PROTO_WITHOUT_INT )) \
  (( bitwise_or, |, DLAMBDA_PROTO_OUTSIDE , DLAMBDA_PROTO_WITHOUT_INT )) \
  (( bitwise_and, &, DLAMBDA_PROTO_OUTSIDE , DLAMBDA_PROTO_WITHOUT_INT )) \
  (( bitwise_xor, ^, DLAMBDA_PROTO_OUTSIDE , DLAMBDA_PROTO_WITHOUT_INT )) \
  (( mem_ptr, ->*, DLAMBDA_PROTO_OUTSIDE , DLAMBDA_PROTO_WITHOUT_INT )) \
  (( assign, =, DLAMBDA_PROTO_INSIDE , DLAMBDA_PROTO_WITHOUT_INT )) \
  (( shift_left_assign, <<=, DLAMBDA_PROTO_OUTSIDE , DLAMBDA_PROTO_WITHOUT_INT )) \
  (( shift_right_assign, >>=, DLAMBDA_PROTO_OUTSIDE , DLAMBDA_PROTO_WITHOUT_INT )) \
  (( multiplies_assign, *=, DLAMBDA_PROTO_OUTSIDE , DLAMBDA_PROTO_WITHOUT_INT )) \
  (( divides_assign, /=, DLAMBDA_PROTO_OUTSIDE , DLAMBDA_PROTO_WITHOUT_INT )) \
  (( modulus_assign, %=, DLAMBDA_PROTO_OUTSIDE , DLAMBDA_PROTO_WITHOUT_INT )) \
  (( plus_assign, +=, DLAMBDA_PROTO_OUTSIDE , DLAMBDA_PROTO_WITHOUT_INT )) \
  (( minus_assign, -=, DLAMBDA_PROTO_OUTSIDE , DLAMBDA_PROTO_WITHOUT_INT )) \
  (( bitwise_or_assign, |=, DLAMBDA_PROTO_OUTSIDE , DLAMBDA_PROTO_WITHOUT_INT )) \
  (( bitwise_and_assign, &=, DLAMBDA_PROTO_OUTSIDE , DLAMBDA_PROTO_WITHOUT_INT )) \
  (( bitwise_xor_assign, ^=, DLAMBDA_PROTO_OUTSIDE , DLAMBDA_PROTO_WITHOUT_INT ))
#define DLAMBDA_PROTO_BINARY_OPERATORS \
  DLAMBDA_PROTO_BINARY_OPERATORS_WITHOUT_SUBSCRIPT \
  (( subscript, [], DLAMBDA_PROTO_INSIDE , DLAMBDA_PROTO_WITHOUT_INT ))

#define DLAMBDA_PROTO_OPERATOR_TAG( s, data, oper ) \
  DLAMBDA_PROTO_OD_ELEM( 0, oper ) BOOST_PP_COMMA()
#define DLAMBDA_PROTO_OPERATOR_SHOULD_BE_DEFINED_OUTSIDE( s, data, oper ) \
  BOOST_PP_EQUAL( DLAMBDA_PROTO_OD_ELEM( 2, oper ), DLAMBDA_PROTO_OUTSIDE )
#define DLAMBDA_PROTO_OPERATOR_SHOULD_BE_DEFINED_INSIDE( s, data, oper ) \
  BOOST_PP_EQUAL( DLAMBDA_PROTO_OD_ELEM( 2, oper ), DLAMBDA_PROTO_INSIDE )
#define DLAMBDA_PROTO_OPERATOR_SHOULD_BE_DEFINED_WITH_INT( s, data, oper ) \
  BOOST_PP_EQUAL( DLAMBDA_PROTO_OD_ELEM( 3, oper ), DLAMBDA_PROTO_WITH_INT )
#define DLAMBDA_PROTO_OPERATOR_SHOULD_BE_DEFINED_WITHOUT_INT( s, data, oper ) \
  BOOST_PP_EQUAL( DLAMBDA_PROTO_OD_ELEM( 3, oper ), DLAMBDA_PROTO_WITHOUT_INT )
#define DLAMBDA_PROTO_UNARY_OPERATOR_INSIDE( s, data, oper ) \
    node operator DLAMBDA_PROTO_OD_ELEM( 1, oper ) ( \
      BOOST_PP_COMMA_IF( BOOST_PP_EQUAL( DLAMBDA_PROTO_OD_ELEM( 3, oper ), DLAMBDA_PROTO_WITH_INT ) ) \
      BOOST_PP_IF( BOOST_PP_EQUAL( DLAMBDA_PROTO_OD_ELEM( 3, oper ), DLAMBDA_PROTO_WITH_INT ), int, ) \
    ) const { \
      return node( \
        detail::unary_operator< node >( \
          detail::unary_operator< node > :: tag :: DLAMBDA_PROTO_OD_ELEM( 0, oper ) BOOST_PP_COMMA() \
          std::array< node, 1u >{{ *this }} \
        ) \
      ); \
    }

#define DLAMBDA_PROTO_BINARY_OPERATOR_INSIDE( s, data, oper ) \
    template< typename T > \
    node operator DLAMBDA_PROTO_OD_ELEM( 1, oper ) ( const T &right ) const { \
      return node( \
        detail::binary_operator< node >( \
          detail::binary_operator< node > :: tag :: DLAMBDA_PROTO_OD_ELEM( 0, oper ) BOOST_PP_COMMA() std::array< node, 2u >{{ *this BOOST_PP_COMMA() make_node( right ) }} \
        ) \
      ); \
    } \
    template< typename T > \
    node operator DLAMBDA_PROTO_OD_ELEM( 1, oper ) ( node &&right ) const { \
      return node( \
        detail::binary_operator< node >( \
          detail::binary_operator< node > :: tag :: DLAMBDA_PROTO_OD_ELEM( 0, oper ) BOOST_PP_COMMA() std::array< node, 2u >{{ *this BOOST_PP_COMMA() make_node( std::move( right ) ) }} \
        ) \
      ); \
    }

#define DLAMBDA_PROTO_UNARY_OPERATOR_OUTSIDE( s, data, oper ) \
    template< \
      typename ValueType BOOST_PP_COMMA() \
      typename Enable = typename boost::enable_if< \
        boost::mpl::or_< \
          is_node< ValueType > BOOST_PP_COMMA() \
          boost::is_same< ValueType, node > \
        > \
      >::type \
    > \
    node operator DLAMBDA_PROTO_OD_ELEM( 1, oper ) ( \
      const ValueType &value \
      BOOST_PP_COMMA_IF( BOOST_PP_EQUAL( DLAMBDA_PROTO_OD_ELEM( 3, oper ), DLAMBDA_PROTO_WITH_INT ) ) \
      BOOST_PP_IF( BOOST_PP_EQUAL( DLAMBDA_PROTO_OD_ELEM( 3, oper ), DLAMBDA_PROTO_WITH_INT ), int, ) \
    ) { \
      return node( \
        detail :: unary_operator< node > ( \
          detail::unary_operator< node > :: tag :: DLAMBDA_PROTO_OD_ELEM( 0, oper ) BOOST_PP_COMMA() \
          std::array< node, 1u >{{ node::make_node( value ) }} \
        ) \
      ); \
    } \
    template< \
      typename ValueType BOOST_PP_COMMA() \
      typename Enable = typename boost::enable_if< \
        boost::mpl::or_< \
          is_node< ValueType > BOOST_PP_COMMA() \
          boost::is_same< ValueType, node > \
        > \
      >::type \
    > \
    node operator DLAMBDA_PROTO_OD_ELEM( 1, oper ) ( \
      ValueType &&value \
      BOOST_PP_COMMA_IF( BOOST_PP_EQUAL( DLAMBDA_PROTO_OD_ELEM( 3, oper ), DLAMBDA_PROTO_WITH_INT ) ) \
      BOOST_PP_IF( BOOST_PP_EQUAL( DLAMBDA_PROTO_OD_ELEM( 3, oper ), DLAMBDA_PROTO_WITH_INT ), int, ) \
    ) { \
      return node( \
        detail  ::  unary_operator< node >( \
          detail::unary_operator< node > :: tag :: DLAMBDA_PROTO_OD_ELEM( 0, oper ) BOOST_PP_COMMA() \
          std::array< node, 1u >{{ node::make_node( std::move( value ) ) }} \
        ) \
      ); \
    }

#define DLAMBDA_PROTO_BINARY_OPERATOR_OUTSIDE( s, data, oper ) \
    template< \
      typename Left BOOST_PP_COMMA() \
      typename Right BOOST_PP_COMMA() \
      typename Enable = typename boost::enable_if< \
        boost::mpl::or_< \
          is_node< Left > BOOST_PP_COMMA() \
          is_node< Right > BOOST_PP_COMMA() \
          boost::is_same< Left, node > BOOST_PP_COMMA() \
          boost::is_same< Right, node > \
        > \
      >::type \
    > \
    node operator DLAMBDA_PROTO_OD_ELEM( 1, oper ) ( \
      const Left &left BOOST_PP_COMMA() \
      const Right &right \
    ) { \
      return node( \
        detail :: binary_operator < node > ( \
          detail :: binary_operator< node > :: tag :: DLAMBDA_PROTO_OD_ELEM( 0, oper ) BOOST_PP_COMMA() std::array< node, 2u >{{ node::make_node( left ) BOOST_PP_COMMA() node::make_node( right ) }} \
        ) \
      ); \
    } \
    template< \
      typename Left BOOST_PP_COMMA() \
      typename Right BOOST_PP_COMMA() \
      typename Enable = typename boost::enable_if< \
        boost::mpl::or_< \
          is_node< Left > BOOST_PP_COMMA() \
          is_node< Right > BOOST_PP_COMMA() \
          boost::is_same< Left, node > BOOST_PP_COMMA() \
          boost::is_same< Right, node > \
        > \
      >::type \
    > \
    node operator DLAMBDA_PROTO_OD_ELEM( 1, oper ) ( \
      Left &&left BOOST_PP_COMMA() \
      Right &&right \
    ) { \
      return node( \
        detail :: binary_operator< node >( \
          detail :: binary_operator< node > :: tag :: DLAMBDA_PROTO_OD_ELEM( 0, oper ) BOOST_PP_COMMA() std::array< node, 2u >{{ node::make_node( std::move( left ) ) BOOST_PP_COMMA() node::make_node( std::move( right ) ) }} \
        ) \
      ); \
    }

namespace dlambda {
  namespace proto {
    constexpr size_t tag_count = BOOST_PP_SEQ_SIZE( DLAMBDA_PROTO_UNARY_OPERATORS ) + BOOST_PP_SEQ_SIZE( DLAMBDA_PROTO_BINARY_OPERATORS ) + 16;
    enum class tag {
      any = 0,
      terminal,
      literal,
      placeholder,
      BOOST_PP_SEQ_FOR_EACH(
        DLAMBDA_PROTO_OPERATOR_TAG,
        unused,
        DLAMBDA_PROTO_UNARY_OPERATORS
      )
      BOOST_PP_SEQ_FOR_EACH(
        DLAMBDA_PROTO_OPERATOR_TAG,
        unused,
        DLAMBDA_PROTO_BINARY_OPERATORS
      )
      comma,
      ternary,
      function,
      if_else,
      while_,
      for_,
      construct,
      c_style_cast,
      static_cast_,
      const_cast_,
      dynamic_cast_,
      reinterpret_cast_
    };
  }
}
#endif


