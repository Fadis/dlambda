#ifndef DLAMBDA_PROTO_NODE_EVAL_HPP
#define DLAMBDA_PROTO_NODE_EVAL_HPP
#include <array>
#include <vector>
#include <utility>
#include <functional>

#include <boost/mpl/if.hpp>
#include <boost/mpl/not.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/optional.hpp>
#include <dlambda/proto/node.hpp>
#include <dlambda/proto/node/make_node.hpp>
#include <dlambda/proto/node/get_top_operator_tag.hpp>
#include <dlambda/proto/node/match.hpp>

namespace dlambda {
  namespace proto {
    template< typename T >
    const proto::node &left( const T &value_ ) {
      return boost::get< proto::node::binary_operator >( value_.get() ).args()[ 0 ];
    }
    template< typename T >
    const proto::node &right( const T &value_ ) {
      return boost::get< proto::node::binary_operator >( value_.get() ).args()[ 1 ];
    }
    template< typename T >
    const proto::node &value( const T &value_ ) {
      return boost::get< proto::node::unary_operator >( value_.get() ).args()[ 0 ];
    }
    struct invalid_expression {};
    template< typename ResultType >
    class eval {
    public:
      typedef typename boost::remove_cv< ResultType >::type result_type;
      typedef typename boost::mpl::if_<
        boost::is_same< result_type, void >,
        std::function< bool( const eval&, const node& ) >,
        std::function< boost::optional< result_type >( const eval&, const node& ) >
      >::type evaluator_type;
      typedef std::pair< node, evaluator_type > tagged_evaluator_type;
      typedef std::vector< tagged_evaluator_type > local_evaluators_type;
      typedef std::array< local_evaluators_type, tag_count > evaluators_type;
      template< typename TypeRange >
      eval( const TypeRange &argument_types ) : matcher( argument_types ) {}
      template< typename T >
      void push( const T &rule, const evaluator_type &cb ) {
        evaluator[ size_t( get_top_operator_tag( rule ) ) ].push_back( std::make_pair( detail::make_node< node, T >()( rule ), cb ) );
      }
      template< typename T >
      void push( T &&rule, const evaluator_type &cb ) {
        evaluator[ size_t( get_top_operator_tag( rule ) ) ].push_back( std::make_pair( detail::make_node< node, T >()( rule ), cb ) );
      }
      template< typename U = result_type >
      result_type operator()(
        const node &rule,
        typename boost::enable_if<
          boost::mpl::not_< boost::is_same< void, U > >
        >::type* = 0 ) const {
        std::vector< tag > possible_tags;
        possible_tags.push_back( get_top_operator_tag( rule ) );
        if(
          possible_tags.front() == tag::terminal ||
          possible_tags.front() == tag::literal ||
          possible_tags.front() == tag::placeholder
        )
          possible_tags.push_back( tag::terminal );
        possible_tags.push_back( tag::any );
        for( const auto &tag: possible_tags ) {
          const auto &local_evaluators = evaluator[ size_t( tag ) ];
          for(
            auto candidate = std::find_if(
              local_evaluators.begin(), local_evaluators.end(),
              [&]( const tagged_evaluator_type &evaluator ) {
                return apply_visitor( matcher, rule.get_raw(), evaluator.first.get_raw() );
              }
            );
            candidate != local_evaluators.end();
            candidate = std::find_if(
              std::next( candidate ), local_evaluators.end(),
              [&]( const tagged_evaluator_type &evaluator ) {
                return apply_visitor( matcher, rule.get_raw(), evaluator.first.get_raw() );
              }
            )
          ) {
            const auto result = candidate->second( *this, rule );
            if( result ) {
              return *result;
            }
          }
        }
        throw invalid_expression();
      }
      template< typename U = result_type >
      void operator()(
        const node &rule,
        typename boost::enable_if<
          boost::is_same< void, U >
        >::type* = 0 ) const {
        std::vector< tag > possible_tags;
        possible_tags.push_back( get_top_operator_tag( rule ) );
        if(
          possible_tags.front() == tag::terminal ||
          possible_tags.front() == tag::literal ||
          possible_tags.front() == tag::placeholder
        )
          possible_tags.push_back( tag::terminal );
        possible_tags.push_back( tag::any );
        for( const auto &tag: possible_tags ) {
          const auto &local_evaluators = evaluator[ size_t( tag ) ];
          for(
            auto candidate = std::find_if(
              local_evaluators.begin(), local_evaluators.end(),
              [&]( const tagged_evaluator_type &evaluator ) {
                return apply_visitor( matcher, rule.get_raw(), evaluator.first.get_raw() );
              }
            );
            candidate != local_evaluators.end();
            candidate = std::find_if(
              std::next( candidate ), local_evaluators.end(),
              [&]( const tagged_evaluator_type &evaluator ) {
                return apply_visitor( matcher, rule.get_raw(), evaluator.first.get_raw() );
              }
            )
          ) {
            const auto result = candidate->second( *this, rule );
            if( result ) {
              return;
            }
          }
        }
        throw invalid_expression();
      }
    private:
      detail::match matcher;
      evaluators_type evaluator;
    };
  }
}

#endif


