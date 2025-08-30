#include "runner/choice_impl.hh"

#include <libzork/exceptions.hh>
#include <sstream>

#include "exceptions.hh"
#include "story/story_impl.hh"

namespace libzork::runner
{

    ChoiceRunnerImpl::ChoiceRunnerImpl(std::unique_ptr<story::Story> story,
                                       std::istream& is, std::ostream& os)
        : ChoiceRunner(std::move(story), is, os)
    {}

    void ChoiceRunnerImpl::print_script() const
    {
        runner::InteractiveRunner::print_script();

        auto choices = this->story_.get()->get_store()->get_active_node();
        auto size = choices->list_choices().size() + 1;

        for (size_t i = 1; i < size; i++)
        {
            auto all_choices = choices->list_choices()[i - 1];
            this->os_ << i << ". " << all_choices << "\n";
        }
        this->os_ << "\n";
    }

    void ChoiceRunnerImpl::verifBuiltins(std::string input)
    {
        if (input.compare(".quit") == 0)
        {
            throw BuiltinsException(Builtins::QUIT);
        }
        if (input.compare(".brief") == 0)
        {
            throw BuiltinsException(Builtins::BRIEF);
        }
        if (input.compare(".jump") == 0)
        {
            throw BuiltinsException(Builtins::JUMP);
        }
        if (input.compare(".shout") == 0)
        {
            throw BuiltinsException(Builtins::SHOUT);
        }
        if (input.compare(".undo") == 0)
        {
            throw BuiltinsException(Builtins::UNDO);
        }
        if (input.compare(".redo") == 0)
        {
            throw BuiltinsException(Builtins::REDO);
        }
        if (input.compare(".save") == 0)
        {
            throw BuiltinsException(Builtins::SAVE);
        }
        if (input.compare(".restore") == 0)
        {
            throw BuiltinsException(Builtins::RESTORE);
        }
        if (input.compare(".debug") == 0)
        {
            auto b = dynamic_cast<const store::StoreImpl&>(
                *this->story_.get()->get_store());
            auto dsdd = this->story_.get()->get_current();
            auto dds = dynamic_cast<const story::NodeImpl*>(dsdd);
            auto dd = dds->get_availables(1);
            std::cout << "CHOIX: \n";
            for (size_t i = 0; i < dd.size(); i++)
            {
                std::cout << dd[i];
                std::cout << "\n" << dd[i]->verif_conditions(1) << "\n";
                std::cout << "\n";
            }
            std::cout << "==============\n";

            throw BuiltinsException(Builtins::DEBUG);
        }
    }

    void ChoiceRunnerImpl::process_input()
    {
        std::string in;
        std::getline(std::cin, in);

        verifBuiltins(in);

        // si ce n'est pas un nombre
        if (in.find_first_not_of("0123456789") != std::string::npos
            || in.size() == 0)
            throw RunnerInterrupt("Invalid Choice.");

        size_t input = std::stoi(in);

        auto choices = this->story_.get()->get_store()->get_active_node();
        size_t taille = choices->list_choices().size();

        if (input < 1 || input > taille)
            throw RunnerInterrupt("Invalid Choice.");

        if (taille == 0)
        {
            return;
        }
        auto next = choices->make_choice(input - 1);
        this->story_.get()->set_current(next);
    }

} // namespace libzork::runner
