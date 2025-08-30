#pragma once

#include <libzork/runner/html.hh>

namespace libzork::runner
{

    class HTMLRunnerImpl : public HTMLRunner
    {
    public:
        ~HTMLRunnerImpl() override = default;

        void run() override;
    };

} // namespace libzork::runner
