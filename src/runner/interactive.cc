#include <libzork/exceptions.hh>
#include <libzork/runner/interactive.hh>

#include "exceptions.hh"

namespace libzork::runner
{

    InteractiveRunner::InteractiveRunner(std::unique_ptr<story::Story> story,
                                         std::istream& is, std::ostream& os)
        : Runner(std::move(story))
        , is_(is)
        , os_(os)
    {}

    void InteractiveRunner::print_script() const
    {
        this->os_ << this->story_.get()->get_current()->get_text() << "\n";
    }

    void InteractiveRunner::run()
    {
        // Recupere le noeud actuel pour le parcourir
        auto current = this->story_.get()->get_store()->get_active_node();
        int curr_size = current->list_choices().size();
        // Boucler tant qu'il y a au moins un choix
        // Affiche le script actuel
        // Attente d' un user input
        int invalid = 0;
        while (curr_size > 0)
        {
            try
            {
                if (invalid == 0)
                    print_script();
                else
                    invalid = 0;
                this->os_ << "> ";

                process_input();
                curr_size = this->story_.get()
                                ->get_store()
                                ->get_active_node()
                                ->list_choices()
                                .size();
            }
            catch (RunnerInterrupt& e)
            {
                this->os_ << e.what() << "\n";
                invalid = 1;
            }
            catch (BuiltinsException& b)
            {
                if (b.what() == Builtins::QUIT)
                    throw RunnerQuit();
                if (b.what() == Builtins::SAVE)
                    invalid = 1;
            }
        }
        print_script();
    }

} // namespace libzork::runner
