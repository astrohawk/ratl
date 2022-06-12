/**
 * Copyright (c) 2018-2022 Hamish Cook
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef _ratl_detail_operator_arrow_proxy_
#define _ratl_detail_operator_arrow_proxy_

namespace ratl
{
namespace detail
{
template<typename Tp>
class operator_arrow_proxy
{
public:
    explicit operator_arrow_proxy(const Tp& value) : value_(value) {}

    inline Tp* operator->() noexcept
    {
        return &value_;
    }

    inline const Tp* operator->() const noexcept
    {
        return &value_;
    }

    inline operator const Tp*() noexcept
    {
        return &value_;
    }

    inline operator Tp*() const noexcept
    {
        return &value_;
    }

private:
    Tp value_;
};

} // namespace detail
} // namespace ratl

#endif // _ratl_detail_operator_arrow_proxy_
