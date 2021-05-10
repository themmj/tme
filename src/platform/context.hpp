#ifndef _PLATFORM_CONTEXT_H
#define _PLATFORM_CONTEXT_H

#include <memory>

namespace tme {
    namespace platform {

        /**//**
         * Context acts as an injection point to take care of platform specific setup and teardown logic.
         *
         * Can be used to initialize data or frameworks when the program starts and clean up when the program shuts down.
         */
        class Context {
            public:
            /// platform specific setup and initialization logic
            static std::unique_ptr<Context> create();
            Context() {}
            virtual ~Context() {}
        };

    }
}

#endif

