#include "runner/smart_impl.hh"

#include <cassert>
#include <fstream>
#include <functional>
#include <iostream>
#include <libzork/exceptions.hh>
#include <regex>
#include <set>
#include <yaml-cpp/yaml.h>

#include "exceptions.hh"
#include "store/store_impl.hh"
#include "story/story_impl.hh"

namespace libzork::runner
{
    SmartRunnerImpl::SmartRunnerImpl(
        std::unique_ptr<story::Story> story,
        std::map<std::string, std::vector<std::string>> dico, std::istream& is,
        std::ostream& os)
        : SmartRunner(std::move(story), is, os)
        , dico_(dico)
    {}

    int SmartRunnerImpl::verifBuiltins(std::string input)
    {
        if (input.compare(".quit") == 0)
            throw RunnerQuit();
        if (input.compare(".brief") == 0)
            return 1;
        if (input.compare(".jump") == 0)
            throw RunnerInterrupt("Boink");
        if (input.compare(".shout") == 0)
            throw RunnerInterrupt("Aaaarrrrgggghhhh!");

        if (input.compare(".undo") == 0)
        {
            throw BuiltinsException(Builtins::UNDO);
        }
        if (input.compare(".redo") == 0)
        {
            throw BuiltinsException(Builtins::REDO);
        }
        if (input.substr(0, 6).compare(".save ") == 0)
        {
            std::stringstream token(input);
            std::string tok;
            token >> tok;
            if (!(token >> tok))
                throw RunnerInterrupt("Give me a file to write.");
            std::string file_name = tok;
            if ((token >> tok))
                throw RunnerInterrupt("Invalid arguments.");

            std::ofstream file(file_name,
                               std::ios::out | std::ios::in | std::ios::trunc);

            auto casted_store = dynamic_cast<const store::StoreImpl*>(
                this->story_.get()->get_store());
            casted_store->save(file);
            throw RunnerInterrupt("Saved the game to " + file_name);
        }
        if (input.substr(0, 9).compare(".restore ") == 0)
        {
            std::stringstream token(input);
            std::string tok;
            token >> tok;
            if (!(token >> tok))
                throw RunnerInterrupt("Give me a file to write.");
            std::string file_name = tok;
            if ((token >> tok))
                throw RunnerInterrupt("Invalid arguments.");

            std::ifstream file(file_name);

            if (file.is_open())
            {
                auto aa = dynamic_cast<const store::StoreImpl*>(
                    this->story_.get()->get_store());
                auto ok = const_cast<store::StoreImpl*>(aa);
                // auto casted_store2 =
                // std::const_pointer_cast<store::StoreImpl>(this->story_.get()->get_store());
                try
                {
                    ok->restore(file, *this->story_.get());
                }
                catch (std::exception& e)
                {
                    throw RunnerInterrupt("No node");
                }
                // casted_store2->restore(file, *this->story_.get());
                throw RunnerInterrupt(ok->get_active_node()->get_text());
            }
            else
                throw RunnerInterrupt("File cannot be opened");
        }
        if (input.compare(".debug") == 0)
        {
            auto b = dynamic_cast<const store::StoreImpl&>(
                *this->story_.get()->get_store());
            b.display_variables();
            throw BuiltinsException(Builtins::DEBUG);
        }
        return 0;
    }

    void SmartRunnerImpl::process_input()
    {
        std::string input;
        std::getline(std::cin, input);
        if (verifBuiltins(input) == 1)
            return;

        int max_syno = 0;
        size_t max_ind = 0;
        auto choices = this->story_.get()->get_current();

        auto lst_choices = choices->list_choices();
        for (size_t i = 0; i < lst_choices.size(); i++)
        {
            int synonym_nb = count_synonyms(lst_choices[i], input);
            if (synonym_nb > max_syno)
            {
                max_syno = synonym_nb;
                max_ind = i;
            }
        }

        if (max_syno == 0)
            throw RunnerInterrupt("What the f are you talking about?!");

        auto next = choices->make_choice(max_ind);
        this->story_.get()->set_current(next);
    }

    std::set<std::string> create_vec(std::string choice)
    {
        std::set<std::string> choice_words;
        size_t debut = 0;
        size_t fin = 0;
        size_t choice_size = choice.size();
        while ((fin = choice.find_first_of(" ,;!?’-.:", debut))
               != std::string::npos)
        {
            if (debut < fin)
            {
                auto toadd = choice.substr(debut, fin - debut);
                choice_words.insert(toadd);
            }
            debut = fin + 1;
        }

        if (choice_size > debut)
        {
            auto toadd = choice.substr(debut, fin - debut);
            choice_words.insert(toadd);
        }
        return choice_words;
    }

    int SmartRunnerImpl::count_synonyms(const std::string& left,
                                        const std::string& right) const
    {
        std::string choice = left;
        std::transform(choice.begin(), choice.end(), choice.begin(),
                       [](char elem) { return std::tolower(elem); });

        std::string input = right;
        std::transform(input.begin(), input.end(), input.begin(),
                       [](char elem) { return std::tolower(elem); });

        auto choice_words = create_vec(choice);
        auto input_words = create_vec(input);
        /*
        std::cout << "The elements in set choices are: \n";
        for (auto it = choice_words.begin(); it != choice_words.end(); it++)
            std::cout << *it << " ";
        std::cout << "\n";

        std::cout << "The elements in set input are: \n";
        for (auto it = input_words.begin(); it != input_words.end(); it++)
            std::cout << *it << " ";
        std::cout << "\n";
        */

        int synonym_cpt = 0;

        for (auto in_word = input_words.begin(); in_word != input_words.end();
             ++in_word)
        {
            auto syno = this->dico_.find(*in_word);

            if (syno != this->dico_.end())
            {
                auto list_syno = syno->second;
                for (auto ch_word = choice_words.begin();
                     ch_word != choice_words.end(); ++ch_word)
                {
                    auto gg = *ch_word;
                    auto hh = *in_word;
                    if (std::find(list_syno.begin(), list_syno.end(), *ch_word)
                            != list_syno.end()
                        || gg.compare(hh) == 0)
                        synonym_cpt++;
                }
            }
        }

        return synonym_cpt;
    }

} // namespace libzork::runner
