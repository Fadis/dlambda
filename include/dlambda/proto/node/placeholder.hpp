#ifndef DLAMBDA_PROTO_PLACEHOLDER_HPP
#define DLAMBDA_PROTO_PLACEHOLDER_HPP

#include <cstdlib>

namespace dlambda {
  namespace proto {
    class placeholder {
    public:
      placeholder( size_t index__ ) : index_( index__ ) {}
      placeholder( const placeholder & ) = default;
      placeholder( placeholder && ) = default;
      placeholder &operator=( const placeholder & ) = default;
      placeholder &operator=( placeholder && ) = default;
      size_t index() const { return index_; }
    private:
      size_t index_;
    };
  }
}

#endif


