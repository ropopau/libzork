#include "vars/action_impl.hh"

#include <algorithm>
#include <iostream>
#include <map>

#include "exceptions.hh"

namespace libzork::vars
{
    ActionImpl::ActionImpl(store::Store& store, const std::string variable,
                           std::function<int(int, int)> operation, int value)
        : store_(store)
        , variable_(variable)
        , operation_(operation)
        , value_(value)
    {}

    void ActionImpl::apply() const
    {
        auto get_var = this->store_.get_variable(this->variable_);
        auto newa = this->operation_(get_var, this->value_);
        this->store_.set_variable(this->variable_, newa);
    }

} // namespace libzork::vars
