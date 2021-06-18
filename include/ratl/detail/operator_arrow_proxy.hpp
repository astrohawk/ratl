#ifndef _ratl_detail_operator_arrow_proxy_
#define _ratl_detail_operator_arrow_proxy_

// C++ Standard Library includes

// ratl includes
#include <ratl/detail/config.hpp>
#include <ratl/detail/types.hpp>

namespace ratl
{
namespace detail
{
template<class Tp>
class operator_arrow_proxy
{
public:
    operator_arrow_proxy(Tp const& value) : value_(value) {}

    Tp* operator->() const
    {
        return &value_;
    }

    operator Tp*() const
    {
        return &value_;
    }

private:
    mutable Tp value_;
};

} // namespace detail
} // namespace ratl

#endif // _ratl_detail_operator_arrow_proxy_
