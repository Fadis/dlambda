#ifndef DLAMBDA_PROTO_DISPLAY_EXPR_HPP
#define DLAMBDA_PROTO_DISPLAY_EXPR_HPP

#include <iostream>
#include <iterator>
#include <boost/range/algorithm/copy.hpp>

#include <dlambda/type.hpp>
#include <dlambda/proto/node.hpp>
#include <dlambda/proto/eval.hpp>
#include <dlambda/proto/display_value.hpp>

namespace dlambda {
  namespace proto {
#define DLAMBDA_PROTO_UNARY_OPERATOR_DUMPER( s, data, oper ) \
    evaluator.push( \
      BOOST_PP_IF( \
        BOOST_PP_EQUAL( \
          DLAMBDA_PROTO_WITHOUT_INT, \
          DLAMBDA_PROTO_OD_ELEM( 3, oper ) \
        ), \
        DLAMBDA_PROTO_OD_ELEM( 1, oper ) any(), \
        any() DLAMBDA_PROTO_OD_ELEM( 1, oper ) \
      ) BOOST_PP_COMMA() \
      [&] ( const eval< void > &evaluator, const node &node_ ) -> bool { \
        const auto &expr = boost::get< node::unary_operator >( node_.get() ); \
        boost::copy( std::string( "[\"" BOOST_PP_STRINGIZE( DLAMBDA_PROTO_OD_ELEM( 1, oper ) ) "\"," ), out ); \
        evaluator( expr.args()[ 0 ] ); \
        *out = ']'; \
        ++out; \
        return true; \
      } \
    );
#define DLAMBDA_PROTO_BINARY_OPERATOR_DUMPER( s, data, oper ) \
    BOOST_PP_IF( \
      BOOST_PP_EQUAL( \
        DLAMBDA_PROTO_OUTSIDE, \
        DLAMBDA_PROTO_OD_ELEM( 2, oper ) \
      ), \
      evaluator.push( \
          any() DLAMBDA_PROTO_OD_ELEM( 1, oper ) any() BOOST_PP_COMMA() \
        [&] ( const eval< void > &evaluator, const node &node_ ) -> bool { \
          const auto &expr = boost::get< node::binary_operator >( node_.get() ); \
          boost::copy( std::string( "[\"" BOOST_PP_STRINGIZE( DLAMBDA_PROTO_OD_ELEM( 1, oper ) ) "\"," ), out ); \
          evaluator( expr.args()[ 0 ] ); \
          *out = ','; \
          ++out; \
          evaluator( expr.args()[ 1 ] ); \
          *out = ']'; \
          ++out; \
          return true; \
        } \
      );, \
    )
#define DLAMBDA_PROTO_MAX_FUNCTION_ARGUMENTS_COUNT 20
#define DLAMBDA_PROTO_FUNCTION_ARGUMENTS( z, index, data ) \
  any()
#define DLAMBDA_PROTO_FUNCTION_ARGUMENTS_DUMPER( z, index, data ) \
  *out = ','; \
  ++out; \
  evaluator( expr.args()[ index ] );
#define DLAMBDA_PROTO_FUNCTION_DUMPER( z, index, data ) \
    evaluator.push( \
        any() ( BOOST_PP_ENUM( index, DLAMBDA_PROTO_FUNCTION_ARGUMENTS, unused ) ) BOOST_PP_COMMA() \
      [&] ( const eval< void > & BOOST_PP_IF( BOOST_PP_EQUAL( index, 0 ), , evaluator ), const node & BOOST_PP_IF( BOOST_PP_EQUAL( index, 0 ), , node_ ) ) -> bool { \
        BOOST_PP_IF( BOOST_PP_EQUAL( index, 0 ), , const auto &expr = boost::get< node::function >( node_.get() ); ) \
        boost::copy( std::string( "[\"()\"" ), out ); \
        BOOST_PP_REPEAT( index, DLAMBDA_PROTO_FUNCTION_ARGUMENTS_DUMPER, unused ) \
        *out = ']'; \
        ++out; \
        return true; \
      } \
    );
#define DLAMBDA_PROTO_CONSTRUCT_DUMPER( z, index, data ) \
  evaluator.push( \
    construct( any_type() BOOST_PP_COMMA_IF( BOOST_PP_GREATER( index, 0 ) ) BOOST_PP_ENUM( index, DLAMBDA_PROTO_FUNCTION_ARGUMENTS, unused ) ) BOOST_PP_COMMA() \
    [&] ( const eval< void > & BOOST_PP_IF( BOOST_PP_EQUAL( index, 0 ), , evaluator ), const node & BOOST_PP_IF( BOOST_PP_EQUAL( index, 0 ), , node_ ) ) -> bool { \
      BOOST_PP_IF( BOOST_PP_EQUAL( index, 0 ), , const auto &expr = boost::get< node::function >( node_.get() ); ) \
      boost::copy( std::string( "[\"construct\"" ), out ); \
      BOOST_PP_REPEAT( index, DLAMBDA_PROTO_FUNCTION_ARGUMENTS_DUMPER, unused ) \
      *out = ']'; \
      ++out; \
      return true; \
    } \
  );
    template< typename OutputIterator >
    class dumper {
    public:
      dumper( OutputIterator &out_, const types::function< dlambda::type > &function_type ) : out ( out_ ), evaluator( function_type.args_type ) {
        evaluator.push(
          any(),
          [&] ( const eval< void > &, const node & ) -> bool {
            boost::copy( std::string( "\"any\"" ), out );
            return true;
          }
        );
        evaluator.push(
          literal( any_type() ),
          [&] ( const eval< void > &, const node &node_ ) -> bool {
            const auto &oper = boost::get< literal >( node_.get() );
            boost::copy( std::string( "\"" ), out );
            display_value( out, oper );
            boost::copy( std::string( "\"" ), out );
            return true;
          }
        );
        evaluator.push(
          terminal( any_type() ),
          [&] ( const eval< void > &, const node & ) -> bool {
            boost::copy( std::string( "\"placeholder\"" ), out );
            return true;
          }
        );
        BOOST_PP_SEQ_FOR_EACH(
          DLAMBDA_PROTO_UNARY_OPERATOR_DUMPER,
          unused,
          DLAMBDA_PROTO_UNARY_OPERATORS
        )
        BOOST_PP_SEQ_FOR_EACH(
          DLAMBDA_PROTO_BINARY_OPERATOR_DUMPER,
          unused,
          DLAMBDA_PROTO_BINARY_OPERATORS
        )
        BOOST_PP_REPEAT(
          DLAMBDA_PROTO_MAX_FUNCTION_ARGUMENTS_COUNT,
          DLAMBDA_PROTO_FUNCTION_DUMPER,
          unused
        )
        evaluator.push(
          ( any(), any() ),
          [&] ( const eval< void > &evaluator, const node &node_ ) -> bool {
            const auto &oper = boost::get< node::binary_operator >( node_.get() );
            boost::copy( std::string( "[\",\"," ), out );
            evaluator( oper.args()[ 0 ] );
            *out = ',';
            ++out;
            evaluator( oper.args()[ 1 ] );
            *out = ']';
            ++out;
            return true;
          }
        );
        evaluator.push(
          any() = any(),
          [&] ( const eval< void > &evaluator, const node &node_ ) -> bool {
            const auto &oper = boost::get< node::binary_operator >( node_.get() );
            boost::copy( std::string( "[\"=\"," ), out );
            evaluator( oper.args()[ 0 ] );
            *out = ',';
            ++out;
            evaluator( oper.args()[ 1 ] );
            *out = ']';
            ++out;
            return true;
          }
        );
        evaluator.push(
          any() [ any() ],
          [&] ( const eval< void > &evaluator, const node &node_ ) -> bool {
            const auto &oper = boost::get< node::binary_operator >( node_.get() );
            boost::copy( std::string( "[\"[]\"," ), out );
            evaluator( oper.args()[ 0 ] );
            *out = ',';
            ++out;
            evaluator( oper.args()[ 1 ] );
            *out = ']';
            ++out;
            return true;
          }
        );
        evaluator.push(
          if_( any() )[ any() ].else_[ any() ],
          [&] ( const eval< void > &evaluator, const node &node_ ) -> bool {
            const auto &oper = boost::get< node::if_else >( node_.get() );
            boost::copy( std::string( "[\"if\"," ), out );
            evaluator( oper.condition() );
            *out = ',';
            ++out;
            evaluator( oper.if_true() );
            *out = ',';
            ++out;
            evaluator( oper.if_false() );
            *out = ']';
            ++out;
            return true;
          }
        );
        evaluator.push(
          while_( any() )[ any() ],
          [&] ( const eval< void > &evaluator, const node &node_ ) -> bool {
            const auto &oper = boost::get< node::while_ >( node_.get() );
            boost::copy( std::string( "[\"while\"," ), out );
            evaluator( oper.condition() );
            *out = ',';
            ++out;
            evaluator( oper.if_true() );
            *out = ']';
            ++out;
            return true;
          }
        );
        evaluator.push(
          for_( any(), any(), any() )[ any() ],
          [&] ( const eval< void > &evaluator, const node &node_ ) -> bool {
            const auto &oper = boost::get< node::for_ >( node_.get() );
            boost::copy( std::string( "[\"for\"," ), out );
            evaluator( oper.initialize() );
            *out = ',';
            ++out;
            evaluator( oper.condition() );
            *out = ',';
            ++out;
            evaluator( oper.each() );
            *out = ',';
            ++out;
            evaluator( oper.if_true() );
            *out = ']';
            ++out;
            return true;
          }
        );
        BOOST_PP_REPEAT(
          DLAMBDA_PROTO_MAX_FUNCTION_ARGUMENTS_COUNT,
          DLAMBDA_PROTO_CONSTRUCT_DUMPER,
          unused
        )
      }
      template< typename Expr >
      void operator()( const Expr &expr ) const {
        evaluator( expr );
      }
    private:
      OutputIterator &out;
      eval< void > evaluator;
    };
    template< typename FunctionType, typename OutputIterator, typename Expr >
    void display_expr( OutputIterator &out, const Expr &expr ) {
      const auto function_type = get_type< FunctionType >();
      dumper< OutputIterator > dumper_( out, boost::get< types::function< dlambda::type > >( function_type ) );
      dumper_( expr );
    }
    template< typename FunctionType, typename Expr >
    void display_expr( const Expr &expr ) {
      const auto function_type = get_type< FunctionType >();
      std::ostream_iterator< char > out( std::cout );
      dumper< std::ostream_iterator< char > > dumper_( out, boost::get< types::function< dlambda::type > >( function_type ) );
      dumper_( expr );
    }
  }
}

#endif

