#ifndef DLAMBDA_PROTO_NODE_MATCH_FAILED_HPP
#define DLAMBDA_PROTO_NODE_MATCH_FAILED_HPP

#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/mpl/and.hpp>
#include <boost/utility/enable_if.hpp>
#include <dlambda/proto/is_node.hpp>

namespace dlambda {
  namespace proto {
    namespace detail {
      struct match_failed {
        template< typename T >
        match_failed(
          const T &
        ) {}
      };
    }
  }
}

#endif


