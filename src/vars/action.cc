#include <iostream>
#include <libzork/vars/action.hh>

#include "exceptions.hh"
#include "vars/action_impl.hh"

namespace libzork::vars
{
    std::function<int(int, int)> get_oper(const std::string& str)
    {
        if (str.compare("add") == 0)
            return libzork::vars::add;
        else if (str.compare("sub") == 0)
            return libzork::vars::sub;
        else if (str.compare("assign") == 0)
            return libzork::vars::assign;
        return nullptr;
    }

    std::unique_ptr<Action> make_action(store::Store& store,
                                        const std::string& variable,
                                        const std::string& action, int value)
    {
        std::function<int(int, int)> oper_type = get_oper(action);

        if (oper_type == nullptr)
            throw std::runtime_error("Invalid operation.");
        if (!store.has_variable(variable))
            throw std::runtime_error("Variable does not exists.");

        return std::make_unique<ActionImpl>(store, variable, oper_type, value);
    }

} // namespace libzork::vars
