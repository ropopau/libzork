#include "store/store_impl.hh"

#include <iostream>
#include <libzork/story/story.hh>
#include <string>
#include <yaml-cpp/yaml.h>

#include "exceptions.hh"
#include "story/node_impl.hh"
#include "story/story_impl.hh"
namespace libzork::store
{

    void StoreImpl::save(std::ostream& os) const
    {
        auto nn = this->actu_;
        auto actual_var = this->variables_;

        YAML::Node variable_list;

        for (auto elem = actual_var.begin(); elem != actual_var.end(); ++elem)
        {
            variable_list[elem->first] = elem->second;
        }
        YAML::Node Princesse;
        if (nn == nullptr)
        {
            Princesse["active-node"] = YAML::Null;
            Princesse["variables"] = YAML::Node();
        }
        else
        {
            Princesse["active-node"] = nn->get_name();
            Princesse["variables"] = variable_list;
        }
        os << Princesse;
    }

    void StoreImpl::restore(std::istream& is, const story::Story& story)
    {
        YAML::Node config = YAML::Load(is);
        std::string node_name = YAML::Dump(config["active-node"]);

        auto variables_list = config["variables"];

        for (auto i = variables_list.begin(); i != variables_list.end(); i++)
        {
            std::string var_name = YAML::Dump(i->first);
            std::string var_val_ = YAML::Dump(i->second);
            int var_val = std::stoi(var_val_);

            this->set_variable(var_name, var_val);
        }

        auto bb = dynamic_cast<const story::StoryImpl*>(&story);
        auto nono = bb->find_node(node_name);

        if (nono == nullptr)
            throw std::exception();

        this->set_active_node(nono.get());
        // reset undo redo history
    }

    const story::Node* StoreImpl::get_active_node() const
    {
        return this->actu_;
    }

    void StoreImpl::set_active_node(const story::Node* node)
    {
        // valeur disparait ici
        this->actu_ = node;
    }

    bool StoreImpl::has_variable(const std::string& name) const
    {
        if (this->variables_.find(name) == this->variables_.end())
            return false;
        return true;
    }

    void StoreImpl::display_variables()
    {
        std::cout << " ========== ETAT ACTUEL ========== \n";
        for (auto i = this->variables_.begin(); i != this->variables_.end();
             ++i)
        {
            std::cout << "-" << i->first << ": " << i->second << "\n";
        }
        std::cout << " ================================= \n";
    }

    int StoreImpl::get_variable(const std::string& name) const
    {
        if (!has_variable(name))
            throw std::runtime_error("Variable " + name + " does not exists !");
        return this->variables_.find(name)->second;
    }

    void StoreImpl::set_variable(const std::string& name, int value)
    {
        if (!has_variable(name))
            this->variables_.insert({ name, value });
        else
            this->variables_.find(name)->second = value;
    }

} // namespace libzork::store
