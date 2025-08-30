#pragma once

#include <libzork/story/story.hh>
#include <map>
#include <store/store_impl.hh>
#include <story/node_impl.hh>
#include <vars/action_impl.hh>
#include <vars/condition_impl.hh>
#include <yaml-cpp/yaml.h>

namespace libzork::story
{

    class StoryImpl : public Story
    {
    public:
        StoryImpl(std::unique_ptr<store::Store> store_, std::string title,
                  std::string script_path,
                  std::vector<std::shared_ptr<story::NodeImpl>> stories,
                  std::map<std::string, std::shared_ptr<story::NodeImpl>>
                      stories_map);
        ~StoryImpl() override = default;
        const std::string& get_title() const override;
        const Node* get_current() const override;
        const std::shared_ptr<story::NodeImpl>
        find_node(std::string& name) const;
        void set_current(const Node* node) override;
        const store::Store* get_store() const override;
        std::ostream& display(std::ostream& os) const override;

    private:
        std::unique_ptr<store::Store> store_;
        std::string title_;
        std::string script_path_;
        std::vector<std::shared_ptr<story::NodeImpl>> stories_;
        std::map<std::string, std::shared_ptr<story::NodeImpl>> stories_map_;
    };

} // namespace libzork::story
