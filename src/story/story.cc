#include <iostream>
#include <libzork/store/store.hh>
#include <libzork/story/story.hh>
#include <map>
#include <yaml-cpp/yaml.h>

#include "exceptions.hh"
#include "story/story_impl.hh"
#include "story_impl.hh"
#include "vars/action_impl.hh"
#include "vars/condition_impl.hh"

namespace libzork::story
{

    std::vector<std::unique_ptr<vars::Action>>
    get_actions(YAML::Node list_actions, store::Store& store)
    {
        std::vector<std::unique_ptr<vars::Action>> toret;
        if (list_actions.IsNull())
            return toret;
        for (size_t i = 0; i < list_actions.size(); i++)
        {
            auto var_name = YAML::Dump(list_actions[i]["name"]);
            auto oper = YAML::Dump(list_actions[i]["operation"]);
            auto val = list_actions[i]["value"];
            int toint = stoi(YAML::Dump(val));

            auto toadd =
                libzork::vars::make_action(store, var_name, oper, toint);
            toret.push_back(std::move(toadd));
        }
        return toret;
    }
    std::vector<std::unique_ptr<vars::Condition>>
    get_conditions(YAML::Node list_conditions, store::Store& store)
    {
        std::vector<std::unique_ptr<vars::Condition>> toret;
        if (list_conditions.IsNull())
            return toret;

        for (size_t i = 0; i < list_conditions.size(); i++)
        {
            auto var_name = YAML::Dump(list_conditions[i]["name"]);
            auto oper = YAML::Dump(list_conditions[i]["comparison"]);
            auto val = list_conditions[i]["value"];
            int toint = stoi(YAML::Dump(val));

            auto toadd =
                libzork::vars::make_condition(store, var_name, oper, toint);
            toret.push_back(std::move(toadd));
        }
        return toret;
    }
    std::map<std::string, std::shared_ptr<story::NodeImpl>> stories;
    std::unique_ptr<Story> make_story(const fs::path& path)
    {
        YAML::Node config = YAML::LoadFile(path);
        auto title = config["title"];

        // Path du script relatif
        fs::path temp = YAML::Dump(config["scripts-path"]);

        // Path du script "absolue"
        fs::path script_path = path.parent_path() / temp;

        auto stories_yaml = config["story"]; // stories
        std::unique_ptr<store::Store> store = libzork::store::make_store();
        auto variables_yaml = config["variables"];
        for (size_t k = 0; k < variables_yaml.size(); k++)
        {
            store.get()->set_variable(
                YAML::Dump(variables_yaml[k]["name"]),
                std::stoi(YAML::Dump(variables_yaml[k]["value"])));
        }

        std::vector<std::shared_ptr<story::NodeImpl>> vec_stories =
            std::vector<std::shared_ptr<story::NodeImpl>>();
        // Creation des Noeuds
        for (size_t i = 0; i < stories_yaml.size(); i++)
        {
            auto actu = stories_yaml[i];
            auto story_name = YAML::Dump(actu["name"]);
            auto story_script_str = YAML::Dump(actu["script"]);

            auto story_script = script_path / story_script_str;

            auto choices = actu["choices"];

            std::shared_ptr<story::Node> nodde =
                libzork::story::make_node(story_name, story_script);

            auto add = std::make_shared<story::NodeImpl>(
                nodde.get()->get_name(), nodde.get()->get_text());

            stories.insert({ story_name, add });

            vec_stories.push_back(add);

            if (i == 0)
            {
                store.get()->set_active_node(add.get());
            }
        }

        // Creation de chaque target
        for (size_t i = 0; i < stories_yaml.size(); i++)
        {
            auto actu = stories_yaml[i];
            auto story_name = YAML::Dump(actu["name"]);
            auto choices = actu["choices"];

            auto node_actu = stories.find(story_name);
            for (size_t j = 0; j < choices.size(); j++)
            {
                auto choices_actu = choices[j];
                auto text = YAML::Dump(choices_actu["text"]);
                auto target = YAML::Dump(choices_actu["target"]);
                auto node_target = stories.find(target);
                // Recuperer la liste des conditions et des actions

                auto actions =
                    get_actions(choices_actu["actions"], *std::move(store));
                auto conditions = get_conditions(choices_actu["conditions"],
                                                 *std::move(store));

                node_actu->second->add_choice(node_target->second.get(), text,
                                              std::move(conditions),
                                              std::move(actions));
            }
        }

        return std::make_unique<story::StoryImpl>(
            std::move(store), YAML::Dump(title), script_path, vec_stories,
            stories);
    }

} // namespace libzork::story
