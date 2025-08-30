#pragma once

#include <string>      // pour std::string
#include <sstream>     // pour std::stringstream
#include <stdexcept>   // pour std::runtime_error
#include <utility>     // pour std::move si besoin
#include <iostream>    // optionnel
#include <source_location>
#include <stdexcept>

namespace libzork
{

    enum Builtins
    {
        QUIT,
        BRIEF,
        JUMP,
        SHOUT,
        UNDO,
        REDO,
        SAVE,
        RESTORE,
        DEBUG
    };

    class NotImplemented : public std::runtime_error
    {
    public:
        NotImplemented(const std::source_location location =
                           std::source_location::current());
    };

    class BuiltinsException : public std::exception
    {
    public:
        BuiltinsException(Builtins val)
            : message_(val)
        {}
        Builtins what()
        {
            return message_;
        }

    private:
        Builtins message_;
    };

    class InvalidChoice : public std::exception
    {
    public:
        InvalidChoice(std::string val)
            : message_(val)
        {}
        std::string what()
        {
            return message_;
        }

    private:
        std::string message_;
    };

} // namespace libzork
