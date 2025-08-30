#pragma once

#include <functional>
#include <iostream>
#include <libzork/vars/action.hh>

namespace libzork::vars
{

    static std::function<int(int, int)> add = [](int a, int b) {
        return a + b;
    };
    static std::function<int(int, int)> sub = [](int a, int b) {
        return a - b;
    };
    static std::function<int(int, int)> assign = [](int a, int b) {
        (void)a;
        return b;
    };

    class ActionImpl : public Action
    {
    public:
        ActionImpl(store::Store& store, const std::string variable,
                   std::function<int(int, int)> operation, int value);
        ~ActionImpl() override = default;
        void display()
        {
            std::cout << "ACTIONS: \n";
            std::cout << " - " << this->variable_ << "\n";
            std::cout << " - " << this->value_ << "\n";
        }
        void apply() const override;

    private:
        store::Store& store_;
        const std::string variable_;
        const std::function<int(int, int)> operation_;
        int value_;
    };

} // namespace libzork::vars
