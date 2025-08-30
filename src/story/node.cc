#include <fstream>
#include <sstream>
#include <iostream>
#include <libzork/story/node.hh>

#include "exceptions.hh"
#include "story/node_impl.hh"

namespace libzork::story
{

    std::unique_ptr<Node> make_node(const std::string& name,
                                    const fs::path& script_path)
    {
        std::ifstream file_in;
        file_in.open(script_path);
        std::stringstream text;

        text << file_in.rdbuf();

        const std::string txt = text.str();
        auto a = std::make_unique<NodeImpl>(name, txt);

        return a;
    }

} // namespace libzork::story
