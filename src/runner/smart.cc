#include <libzork/runner/smart.hh>
#include <yaml-cpp/yaml.h>

#include "exceptions.hh"
#include "runner/smart_impl.hh"

/* 
* smart runner factory method
*/
namespace libzork::runner
{

    std::unique_ptr<SmartRunner>
    make_smart_runner(std::unique_ptr<story::Story> story,
                      const fs::path& synonyms_path, std::istream& is,
                      std::ostream& os)
    {
        std::map<std::string, std::vector<std::string>> dico;

        YAML::Node config = YAML::LoadFile(synonyms_path);
        for (size_t i = 0; i < config.size(); i++)
        {
            std::string word = YAML::Dump(config[i]["word"]);
            auto synonymes = config[i]["synonyms"];
            std::vector<std::string> syno_list;
            for (size_t j = 0; j < synonymes.size(); j++)
            {
                syno_list.push_back(YAML::Dump(synonymes[j]));
            }
            dico.insert({ word, syno_list });
        }
        return std::make_unique<SmartRunnerImpl>(std::move(story), dico, is,
                                                 os);
    }

} // namespace libzork::runner
