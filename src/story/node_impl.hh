#pragma once

#include <iostream>
#include <libzork/story/node.hh>

#include "vars/action_impl.hh"
#include "vars/condition_impl.hh"
namespace libzork::story
{
    class Choices
    {
    public:
        Choices(const std::string text, const Node* target,
                std::vector<std::unique_ptr<vars::Action>> actions = {},
                std::vector<std::unique_ptr<vars::Condition>> conditions = {})
            : text_(text)
            , target_(target)
            , actions_(std::move(actions))
            , conditions_(std::move(conditions))
        {}

        const std::string get_text()
        {
            return this->text_;
        }
        const Node* get_target()
        {
            return this->target_;
        }

        void apply_actions()
        {
            for (auto a = this->actions_.begin(); a != this->actions_.end();
                 ++a)
            {
                a->get()->apply();
            }
        }

        bool verif_conditions(int da = 0)
        {
            for (auto a = this->conditions_.begin();
                 a != this->conditions_.end(); ++a)
            {
                if (da)
                {
                    auto asdf = dynamic_cast<vars::ConditionImpl*>(a->get());
                    asdf->display();
                }

                if (!a->get()->apply())
                {
                    return false;
                }
            }
            return true;
        }

    private:
        const std::string text_;
        const Node* target_;
        std::vector<std::unique_ptr<vars::Action>> actions_;
        std::vector<std::unique_ptr<vars::Condition>> conditions_;
    };

    class NodeImpl : public Node
    {
    public:
        NodeImpl(std::string name, std::string text);
        ~NodeImpl() override = default;
        std::vector<Choices*> get_availables(int a = 0) const;

        const std::string& get_name() const override;
        const std::string& get_text() const override;

        const Node* make_choice(std::size_t index) const override;
        std::vector<std::string>
        list_choices(bool check_conditions = true) const override;
        void add_choice(
            const Node* other, const std::string& text,
            std::vector<std::unique_ptr<vars::Condition>> conditions = {},
            std::vector<std::unique_ptr<vars::Action>> operations = {})
            override;
        std::vector<std::string> list_choices_name() const;

    private:
        const std::string name_;
        const std::string text_;
        std::vector<std::unique_ptr<Choices>> choices_;
    };

} // namespace libzork::story
