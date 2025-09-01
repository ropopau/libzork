#include "options.hh"

#include <getopt.h>
#include <iostream>

namespace
{
    constexpr option options[] = {
        { "story", required_argument, nullptr, 's' },
        { "smart", required_argument, nullptr, 'm' },
    };

    std::string usage(const std::string& name)
    {
        return "usage: " + name
            + " (--story <story.yml>)"
              " [--smart <synonyms.yml> >]";
    };
} // namespace

Config parse_options(int argc, char** argv)
{
    Config config;
    int opt;
    while ((opt = getopt_long(argc, argv, "s:m:t:u:o:n:r:", options, nullptr))
           != -1)
    {
        switch (opt)
        {
        case 's': // --story
            config.story_path = optarg;
            break;
        case 'm': // --smart
            config.story_type = StoryType::Smart;
            config.story_arg = optarg;
            break;
        default:
            throw std::invalid_argument(usage(argv[0]));
        }
    };

    if (config.story_path.empty())
        throw std::invalid_argument("option '--story' is mandatory");

    return config;
}
