#include "story/node_impl.hh"

#include <fstream>
#include <iostream>

#include "exceptions.hh"

namespace libzork::story
{

    NodeImpl::NodeImpl(std::string name, std::string text)
        : name_(name)
        , text_(text)
    {
        this->choices_ = std::vector<std::unique_ptr<Choices>>();
    }

    const std::string& NodeImpl::get_name() const
    {
        return this->name_;
    }

    const std::string& NodeImpl::get_text() const
    {
        return this->text_;
    }

    std::vector<Choices*> NodeImpl::get_availables(int a) const
    {
        std::vector<Choices*> toret;

        for (auto elem = this->choices_.begin(); elem != this->choices_.end();
             ++elem)
        {
            if (a)
            {
                toret.push_back(elem->get());
            }
            else
            {
                auto choices_actu = elem->get();
                bool can = choices_actu->verif_conditions();
                // Si conditions ok
                if (can)
                {
                    toret.push_back(elem->get());
                }
            }
        }
        return toret;
    }

    const Node* NodeImpl::make_choice(size_t index) const
    {
        auto availables = this->get_availables();

        if (index >= availables.size())
            return nullptr;
        availables[index]->apply_actions();
        return availables[index]->get_target();
    }

    std::vector<std::string> NodeImpl::list_choices(bool check_conditions) const
    {
        (void)check_conditions;

        auto availables = this->get_availables();

        std::vector<std::string> all_texts = std::vector<std::string>();

        for (size_t i = 0; i < availables.size(); i++)
        {
            all_texts.push_back(availables[i]->get_text());
        }

        return all_texts;
    }

    std::vector<std::string> NodeImpl::list_choices_name() const
    {
        auto availables = this->get_availables();
        std::vector<std::string> all_texts = std::vector<std::string>();
        for (size_t i = 0; i < availables.size(); i++)
        {
            all_texts.push_back(availables[i]->get_target()->get_name());
        }
        return all_texts;
    }

    void NodeImpl::add_choice(
        const Node* other, const std::string& text,
        std::vector<std::unique_ptr<vars::Condition>> conditions,
        std::vector<std::unique_ptr<vars::Action>> operations)
    {
        //(void)conditions;
        //(void)operations;
        this->choices_.push_back(std::make_unique<Choices>(
            text, other, std::move(operations), std::move(conditions)));
    }

} // namespace libzork::story
