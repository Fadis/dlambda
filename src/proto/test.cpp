#include <iostream>
#include <dlambda/proto/node.hpp>
#include <dlambda/proto/node/match.hpp>
#include <dlambda/proto/node/get_top_operator_tag.hpp>
#include <dlambda/proto/eval.hpp>
#include <dlambda/proto/display_expr.hpp>

dlambda::proto::node double_if_true( bool &cond, const dlambda::proto::node &expr ) {
  if( cond )
    return expr * 2;
  else
    return expr;
}

int main() {
  int index;
  using namespace dlambda::proto;
  if_( 3 == lit( 2 ) )[
    1 + lit( 2 )
  ].else_[
    if_( 1 )[
      3 * lit( 4 )
    ]
  ],
  while_( true == lit( true ) )[
    'a' + lit( 'b' ) + construct( dlambda::get_type< int >(), 3 )
  ],
  for_( ref( index ) = 0, ref( index ) != 10, ++ref( index ) )[
    'a'
  ];
  const std::vector< dlambda::type > argument_types{{ dlambda::types::int_( true, false, 32, true ), dlambda::types::char_( true, false ) }};
  std::cout << match( node( lit( 1 ) + 3 ), node( terminal( dlambda::any_type() ) + terminal( dlambda::get_type< int >() ) ), argument_types ) << std::endl;
  std::cout << match( node( while_( true )[ 2 ] + 5 ), node( terminal( dlambda::any_type() ) + terminal( dlambda::get_type< int >() ) ), argument_types ) << std::endl;
  std::cout << match( node( while_( true )[ 2 ] + 5 ), node( any() + terminal( dlambda::get_type< int >() ) ), argument_types ) << std::endl;
  std::cout << match( node( while_( true )[ 2 ] + 5 ), node( while_( terminal( dlambda::get_type< int >() ) )[ terminal( dlambda::get_type< int >() ) ] + terminal( dlambda::get_type< int >() ) ), argument_types ) << std::endl;
  std::cout << match( node( terminal( dlambda::any_type() )  ), node( lit( 0 ) ), argument_types ) << std::endl;
  
  std::ostream_iterator< char > out( std::cout );
  display_expr< void( int, char ) >( out, ( lit( 0 ) ) );
  std::cout << std::endl;
  display_expr< float( int, char ) >( placeholder( 0 ) * 0.6f + placeholder( 1 ) * 0.4f );
  std::cout << std::endl;
  display_expr< void( int, char ) >( out, ( lit( 0 ) [ 5 ] = terminal( dlambda::get_type< int >() ) + terminal( dlambda::get_type< unsigned int >() ) * terminal( dlambda::get_type< int >() ) << 3, lit( 0 ) = placeholder( 1 ) ) );
  display_expr< void( float, float ) >( out, placeholder( 0 ) * 0.6f + placeholder( 1 ) * 0.4f  );
  std::cout << std::endl;
}

