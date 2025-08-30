#pragma once

#include <functional>
#include <iostream>
#include <libzork/vars/condition.hh>
namespace libzork::vars
{

    static std::function<bool(int, int)> lower = [](int a, int b) {
        return a < b;
    };
    static std::function<bool(int, int)> lower_equal = [](int a, int b) {
        return a <= b;
    };
    static std::function<bool(int, int)> greater = [](int a, int b) {
        return a > b;
    };
    static std::function<bool(int, int)> greater_equal = [](int a, int b) {
        return a >= b;
    };
    static std::function<bool(int, int)> equal = [](int a, int b) {
        return a == b;
    };

    class ConditionImpl : public Condition
    {
    public:
        ConditionImpl(const store::Store& store, const std::string got,
                      int needed, std::function<bool(int, int)> func,
                      std::string dump);

        ~ConditionImpl() override = default;

        void display()
        {
            std::cout << "CONDITIONS: \n";
            std::cout << " - " << this->got_ << ": "
                      << this->store_.get_variable(got_) << "\n";
            std::cout << " - " << this->needed_ << "\n";
            std::cout << " - " << this->dump_ << "\n";
        }

        bool apply() const override;

    private:
        const store::Store& store_;
        const std::string got_;
        int needed_;
        std::function<bool(int, int)> func_;
        std::string dump_;
    };

} // namespace libzork::vars
