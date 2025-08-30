#pragma once

#include <libzork/runner/smart.hh>
#include <map>
namespace libzork::runner
{

    class SmartRunnerImpl : public SmartRunner
    {
    public:
        SmartRunnerImpl(std::unique_ptr<story::Story> story,
                        std::map<std::string, std::vector<std::string>> dico,
                        std::istream& is, std::ostream& os);
        int count_synonyms(const std::string& left,
                           const std::string& right) const override;

        ~SmartRunnerImpl() override = default;

        void process_input() override;

    private:
        int verifBuiltins(std::string input);
        std::map<std::string, std::vector<std::string>> dico_;
    };

} // namespace libzork::runner
