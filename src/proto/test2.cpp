#include <iostream>
#include <dlambda/proto/node.hpp>
#include <dlambda/proto/node/match.hpp>
#include <dlambda/proto/node/get_top_operator_tag.hpp>
#include <dlambda/proto/eval.hpp>
using namespace dlambda;
class context {
  proto::eval< void > evaluator;
public:
  context() : evaluator( std::vector< type >{ get_type< int >() } ) {
    evaluator.push( proto::terminal( any_type() ),
      [&]( const proto::eval< void >&, const proto::node& ) -> bool {
        std::cout << "値" << std::endl;
        return true;
      }
    );
    evaluator.push( proto::any() + proto::any(),
      [&]( const proto::eval< void > &evaluator, const proto::node &expr ) -> bool {
        evaluator( proto::left( expr ) );
        evaluator( proto::right( expr ) );
        std::cout << "加算" << std::endl;
        return true;
      }
    );
  }
  void operator()( const proto::node &expr ) const { evaluator( expr ); }
};
int main() {
  const proto::placeholder _1( 0 );
  context()( _1 + _1 );
}

