#ifndef DLAMBDA_PROTO_IS_NODE_HPP
#define DLAMBDA_PROTO_IS_NODE_HPP

#include <boost/mpl/bool.hpp>
#include <boost/type_traits/is_same.hpp>
#include <dlambda/proto/node/literal.hpp>
#include <dlambda/proto/node/placeholder.hpp>
#include <dlambda/proto/node/terminal.hpp>
#include <dlambda/proto/node/any.hpp>
#include <dlambda/proto/node/unary_operator.hpp>
#include <dlambda/proto/node/binary_operator.hpp>
#include <dlambda/proto/node/ternary_operator.hpp>
#include <dlambda/proto/node/function.hpp>
#include <dlambda/proto/node/if.hpp>
#include <dlambda/proto/node/while.hpp>
#include <dlambda/proto/node/for.hpp>
#include <dlambda/proto/node/construct.hpp>
#include <dlambda/proto/node/cast.hpp>

namespace dlambda {
  namespace proto {
    namespace detail {
      template< typename Node, typename T >
      struct is_node : public boost::mpl::bool_<
        boost::is_same< literal, T >::value ||
        boost::is_same< unary_operator< Node >, T >::value ||
        boost::is_same< binary_operator< Node >, T >::value ||
        boost::is_same< ternary_operator< Node >, T >::value ||
        boost::is_same< function< Node >, T >::value ||
        boost::is_same< placeholder, T >::value ||
        boost::is_same< if_< Node >, T >::value ||
        boost::is_same< if_else< Node >, T >::value ||
        boost::is_same< while_< Node >, T >::value ||
        boost::is_same< for_< Node >, T >::value ||
        boost::is_same< construct< Node >, T >::value ||
        boost::is_same< terminal, T >::value ||
        boost::is_same< any_operator, T >::value ||
        boost::is_same< cast< Node >, T >::value
      > {};
    }
  }
}

#endif


