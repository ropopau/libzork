#include "vars/condition_impl.hh"

#include <algorithm>
#include <iostream>
#include <map>

#include "exceptions.hh"

namespace libzork::vars
{
    ConditionImpl::ConditionImpl(const store::Store& store,
                                 const std::string got, int needed,
                                 std::function<bool(int, int)> func,
                                 std::string dump)
        : store_(store)
        , got_(got)
        , needed_(needed)
        , func_(func)
        , dump_(dump)
    {}

    bool ConditionImpl::apply() const
    {
        auto val = this->store_.get_variable(this->got_);
        return this->func_(val, this->needed_);
    }

} // namespace libzork::vars
