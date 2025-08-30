#include "story/story_impl.hh"

#include <cassert>
#include <libzork/store/store.hh>

#include "exceptions.hh"
#include "store/store_impl.hh"
#include "story/node_impl.hh"

namespace libzork::story
{
    StoryImpl::StoryImpl(
        std::unique_ptr<store::Store> store, std::string title,
        std::string script_path,
        std::vector<std::shared_ptr<story::NodeImpl>> stories,
        std::map<std::string, std::shared_ptr<story::NodeImpl>> stories_map)
        : store_(std::move(store))
        , title_(title)
        , script_path_(script_path)
        , stories_(stories)
        , stories_map_(stories_map)
    {}

    const std::string& StoryImpl::get_title() const
    {
        return this->title_;
    }

    const Node* StoryImpl::get_current() const
    {
        auto a = this->store_.get()->get_active_node();

        return a;
    }

    const std::shared_ptr<story::NodeImpl>
    StoryImpl::find_node(std::string& name) const
    {
        auto a = this->stories_map_.find(name);
        if (a == this->stories_map_.end())
            return nullptr;

        for (auto node : this->stories_)
        {
            if (node.get()->get_name().compare(name) == 0)
                return node;
        }

        return a->second;
    }

    void StoryImpl::set_current(const Node* node)
    {
        this->store_.get()->set_active_node(node);
    }

    const store::Store* StoryImpl::get_store() const
    {
        return this->store_.get();
    }

    std::ostream& StoryImpl::display(std::ostream& os) const
    {
        os << "digraph story {\n";
        for (size_t i = 0; i < this->stories_.size(); i++)
        {
            std::stringstream line;
            auto choices =
                this->stories_[i]; // choix possible pour le noeud actuel

            auto names = choices.get()->list_choices_name();

            if (names.size() == 1)
            {
                line << "\t" << choices.get()->get_name() << " -> " << names[0]
                     << ";\n";
            }
            else if (names.size() > 1)
            {
                line << "\t" << choices.get()->get_name() << " -> {\"";
                line << names[0] << "\"";

                for (size_t j = 1; j < names.size(); j++)
                {
                    line << " \"" << names[j] << "\" ";
                }
                line << "};\n";
            }
            else
            {
                line << "\t" << choices.get()->get_name() + ";\n";
            }
            os << line.str();
        }

        os << "}\n";
        return os;
    }

} // namespace libzork::story
