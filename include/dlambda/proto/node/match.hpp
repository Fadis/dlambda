#ifndef DLAMBDA_PROTO_NODE_MATCH_HPP
#define DLAMBDA_PROTO_NODE_MATCH_HPP

#include <boost/variant.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/not.hpp>
#include <boost/utility/enable_if.hpp>

#include <dlambda/type.hpp>
#include <dlambda/proto/node.hpp>
#include <dlambda/proto/node/match_failed.hpp>
#include <dlambda/type_traits/match.hpp>

namespace dlambda {
  namespace proto {
    namespace detail {
      class match : public boost::static_visitor< bool > {
      public:
        template< typename TypeRange >
        match( const TypeRange &argument_types_ ) : argument_types( boost::begin( argument_types_ ), boost::end( argument_types_ ) ) {}
        template<
          typename Left, typename Right,
          typename Enable = typename boost::enable_if<
            boost::mpl::or_<
              boost::is_same< Left, any_operator >,
              boost::is_same< Right, any_operator >
            >
          >::type
        >
        bool operator()( const Left&, const Right& ) const {
          return true;
        }
        bool operator()( const terminal &left, const terminal &right ) const {
          return type_traits::match( left.type(), right.type() );
        }
        bool operator()( const literal &left, const literal &right ) const {
          return type_traits::match( left.type(), right.type() );
        }
        bool operator()( const literal &left, const terminal &right ) const {
          return type_traits::match( left.type(), right.type() );
        }
        bool operator()( const terminal &left, const literal &right ) const {
          return type_traits::match( left.type(), right.type() );
        }
     
        bool operator()( const placeholder &left, const placeholder &right ) const {
          return left.index() == right.index();
        }
        bool operator()( const placeholder &left, const terminal &right ) const {
          if( left.index() >= argument_types.size() )
            return false;
          return type_traits::match( argument_types[ left.index() ], right.type() );
        }
        bool operator()( const terminal &left, const placeholder &right ) const {
          if( right.index() >= argument_types.size() )
            return false;
          return type_traits::match( left.type(), argument_types[ right.index() ] );
        }
        bool operator()(
          const detail::unary_operator< node > &left,
          const detail::unary_operator< node > &right
        ) const {
          return
            left.get_operator() == right.get_operator() &&
            apply_visitor( *this, left.args()[ 0 ].get_raw(), right.args()[ 0 ].get_raw() );
        }
        
        bool operator()(
          const detail::binary_operator< node > &left,
          const detail::binary_operator< node > &right
        ) const {
          return
            left.get_operator() == right.get_operator() &&
            apply_visitor( *this, left.args()[ 0 ].get_raw(), right.args()[ 0 ].get_raw() ) &&
            apply_visitor( *this, left.args()[ 1 ].get_raw(), right.args()[ 1 ].get_raw() );
        }
     
        bool operator()(
          const detail::ternary_operator< node > &left,
          const detail::ternary_operator< node > &right
        ) const {
          return
            apply_visitor( *this, left.args()[ 0 ].get_raw(), right.args()[ 0 ].get_raw() ) &&
            apply_visitor( *this, left.args()[ 1 ].get_raw(), right.args()[ 1 ].get_raw() ) &&
            apply_visitor( *this, left.args()[ 2 ].get_raw(), right.args()[ 2 ].get_raw() );
        }
        
        bool operator()(
          const detail::function< node > &left,
          const detail::function< node > &right
        ) const {
          bool callee_matches = apply_visitor( *this, left.get_callee().get_raw(), right.get_callee().get_raw() );
          if( !callee_matches )
            return false;
          if( left.args().size() != right.args().size() )
            return false;
          for( size_t index = 0; index != left.args().size(); ++index ) {
            if( !apply_visitor( *this, left.args()[ index ].get_raw(), right.args()[ index ].get_raw() ) )
              return false;
          }
          return true;
        }
        bool operator()(
          const detail::if_else< node > &left,
          const detail::if_else< node > &right
        ) const {
          return
            apply_visitor( *this, left.condition().get_raw(), right.condition().get_raw() ) &&
            apply_visitor( *this, left.if_true().get_raw(), right.if_true().get_raw() ) &&
            apply_visitor( *this, left.if_false().get_raw(), right.if_false().get_raw() );
        }
        bool operator()(
          const detail::while_< node > &left,
          const detail::while_< node > &right
        ) const {
          return
            apply_visitor( *this, left.condition().get_raw(), right.condition().get_raw() ) &&
            apply_visitor( *this, left.if_true().get_raw(), right.if_true().get_raw() );
        }
        bool operator()(
          const detail::for_< node > &left,
          const detail::for_< node > &right
        ) const {
          return
            apply_visitor( *this, left.initialize().get_raw(), right.initialize().get_raw() ) &&
            apply_visitor( *this, left.condition().get_raw(), right.condition().get_raw() ) &&
            apply_visitor( *this, left.each().get_raw(), right.each().get_raw() ) &&
            apply_visitor( *this, left.if_true().get_raw(), right.if_true().get_raw() );
        }
        bool operator()(
          const detail::construct< node > &left,
          const detail::construct< node > &right
        ) const {
          if( !type_traits::match( left.type(), right.type() ) )
            return false;
          if( left.args().size() != right.args().size() )
            return false;
          for( size_t index = 0; index != left.args().size(); ++index ) {
            if( !apply_visitor( *this, left.args()[ index ].get_raw(), right.args()[ index ].get_raw() ) )
              return false;
          }
          return true;
        }
        bool operator()(
          const detail::cast< node > &left,
          const detail::cast< node > &right
        ) const {
          return
            left.cast_type() == right.cast_type() &&
            type_traits::match( left.type(), right.type() ) &&
            apply_visitor( *this, left.args()[ 0 ].get_raw(), right.args()[ 0 ].get_raw() );
        }
        bool operator()( const detail::match_failed &, const detail::match_failed & ) const {
          return false;
        }
      private:
        std::vector< type > argument_types;
      };
    }
    template< typename TypeRange >
    bool match( const node &left, const node &right, const TypeRange &argument_types ) {
      return apply_visitor( detail::match( argument_types ), left.get_raw(), right.get_raw() );
    }
  }
}

#endif

