#pragma once

#include <iostream>
#include <libzork/store/store.hh>
#include <map>
#include <vector>
namespace libzork::store
{

    class StoreImpl
        : public Store
        , public SaveStore
    {
    public:
        StoreImpl()
            : actu_(nullptr){};
        ~StoreImpl() override = default;

        void display_variables();

        const story::Node* get_active_node() const override;
        void set_active_node(const story::Node* node) override;

        bool has_variable(const std::string& name) const override;
        int get_variable(const std::string& name) const override;
        void set_variable(const std::string& name, int value) override;

        virtual void save(std::ostream& os) const override;
        virtual void restore(std::istream& is,
                             const story::Story& story) override;

    private:
        const story::Node* actu_;
        std::map<const std::string, int> variables_;
    };

} // namespace libzork::store
