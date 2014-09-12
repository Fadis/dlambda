#ifndef DLAMBDA_PROTO_MAKE_NODE_FWD_HPP
#define DLAMBDA_PROTO_MAKE_NODE_FWD_HPP

namespace dlambda {
  namespace proto {
    namespace detail {
      template< typename Node, typename T, typename Enable = void >
      struct make_raw_node {};
      template< typename Node, typename T, typename Enable = void >
      struct make_node {};
    }
  }
}

#endif


