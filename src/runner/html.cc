#include <libzork/runner/html.hh>

#include "exceptions.hh"
#include "runner/html_impl.hh"

namespace libzork::runner
{

    std::unique_ptr<HTMLRunner>
    make_html_runner(std::unique_ptr<story::Story> story,
                     const fs::path& output_dir)
    {
        (void)story;
        (void)output_dir;
        throw NotImplemented();
    }

} // namespace libzork::runner
