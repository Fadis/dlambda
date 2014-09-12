#ifndef DLAMBDA_PROTO_NODE_GET_TOP_OPERATOR_TAG_HPP
#define DLAMBDA_PROTO_NODE_GET_TOP_OPERATOR_TAG_HPP

#include <boost/variant.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/not.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/preprocessor.hpp>

#include <dlambda/proto/operators.hpp>
#include <dlambda/proto/node.hpp>
#include <dlambda/proto/node/make_node.hpp>

namespace dlambda {
  namespace proto {
    struct invalid_operator {};
    namespace detail {
#define DLAMBDA_PROTO_OPERATOR_UNARY_LOCAL_TAG_TO_GLOBAL_TAG( s, data, oper_ ) \
  if( oper.get_operator() == unary_operator< node >::tag:: DLAMBDA_PROTO_OD_ELEM( 0, oper_ ) ) \
    return tag:: DLAMBDA_PROTO_OD_ELEM( 0, oper_ );

#define DLAMBDA_PROTO_OPERATOR_BINARY_LOCAL_TAG_TO_GLOBAL_TAG( s, data, oper_ ) \
  if( oper.get_operator() == binary_operator< node >::tag:: DLAMBDA_PROTO_OD_ELEM( 0, oper_ ) ) \
    return tag:: DLAMBDA_PROTO_OD_ELEM( 0, oper_ );

      struct get_top_operator_tag : public boost::static_visitor< tag > {
        tag operator()( const any_operator& ) const {
          return tag::any;
        }
        tag operator()( const terminal& ) const {
          return tag::terminal;
        }
        tag operator()( const literal& ) const {
          return tag::literal;
        }
        tag operator()( const placeholder& ) const {
          return tag::placeholder;
        }
        tag operator()( const unary_operator< node > &oper ) const {
          BOOST_PP_SEQ_FOR_EACH(
            DLAMBDA_PROTO_OPERATOR_UNARY_LOCAL_TAG_TO_GLOBAL_TAG,
            unused,
            DLAMBDA_PROTO_UNARY_OPERATORS
          )
          throw invalid_operator();
        }
        tag operator()( const binary_operator< node > &oper ) const {
          BOOST_PP_SEQ_FOR_EACH(
            DLAMBDA_PROTO_OPERATOR_BINARY_LOCAL_TAG_TO_GLOBAL_TAG,
            unused,
            DLAMBDA_PROTO_BINARY_OPERATORS
          )
          if( oper.get_operator() == binary_operator< node >::tag:: comma )
            return tag:: comma;
          throw invalid_operator();
        }
        tag operator()( const ternary_operator< node >& ) const {
          return tag::ternary;
        }
        tag operator()( const function< node >& ) const {
          return tag::function;
        }
        tag operator()( const if_else< node >& ) const {
          return tag::if_else;
        }
        tag operator()( const while_< node >& ) const {
          return tag::while_;
        }
        tag operator()( const for_< node >& ) const {
          return tag::for_;
        }
        tag operator()( const construct< node >& ) const {
          return tag::construct;
        }
        tag operator()( const cast< node > &oper ) const {
          if( oper.cast_type() == cast< node >::cast_type_type::c_style )
            return tag::c_style_cast;
          else if( oper.cast_type() == cast< node >::cast_type_type::static_ )
            return tag::static_cast_;
          else if( oper.cast_type() == cast< node >::cast_type_type::const_ )
            return tag::const_cast_;
          else if( oper.cast_type() == cast< node >::cast_type_type::dynamic )
            return tag::dynamic_cast_;
          else if( oper.cast_type() == cast< node >::cast_type_type::reinterpret )
            return tag::reinterpret_cast_;
          else
            throw invalid_operator();
        }
      };
    }
    template< typename T >
    tag get_top_operator_tag( const T &oper ) {
      const auto raw = detail::make_raw_node< node, T >()( oper );
      return apply_visitor( detail::get_top_operator_tag(), raw );
    }
  }
}

#endif


