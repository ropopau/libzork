#include <iostream>
#include <libzork/store/store.hh>
#include <libzork/vars/condition.hh>

#include "exceptions.hh"
#include "vars/condition_impl.hh"
namespace libzork::vars
{

    std::function<bool(int, int)> get_compare(const std::string& str,
                                              std::string& dump)
    {
        if (str.compare("lower") == 0)
        {
            dump += "<";
            return libzork::vars::lower;
        }
        else if (str.compare("lower_equal") == 0)
        {
            dump += "<=";
            return libzork::vars::lower_equal;
        }
        else if (str.compare("equal") == 0)
        {
            dump += "=";
            return libzork::vars::equal;
        }
        else if (str.compare("greater_equal") == 0)
        {
            dump += ">=";
            return libzork::vars::greater_equal;
        }
        else if (str.compare("greater") == 0)
        {
            dump += ">";
            return libzork::vars::greater;
        }
        return nullptr;
    }

    std::unique_ptr<Condition> make_condition(const store::Store& store,
                                              const std::string& variable,
                                              const std::string& comparison,
                                              int value)
    {
        std::string a = "//";
        std::function<bool(int, int)> compare_type = get_compare(comparison, a);
        if (compare_type == nullptr)
            throw std::runtime_error("Invalid comparison.");
        if (!store.has_variable(variable))
            throw std::runtime_error("Variable does not exists.");

        return std::make_unique<ConditionImpl>(store, variable, value,
                                               compare_type, a);
    }

} // namespace libzork::vars
