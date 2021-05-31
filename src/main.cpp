/** @file */

#include "core/exceptions/common.hpp"
#include "core/storage.hpp"
#include "platform/context.hpp"
#include "app/tilemap.hpp"
#include "app/editor.hpp"

/// Program entrypoint.
int main() {
    auto contextHandle = tme::platform::Context::create();
    tme::core::Log::init();
    TME_INFO("starting application");


    try {
        auto editor = tme::core::Storage<tme::app::Editor>::global()->create();
        editor->run();
    } catch (const tme::core::exceptions::Base& e) {
        TME_CRITICAL("An error occurred during start up.\n {}: {}\nStopping application.", e.type(), e.what());
        return -1;
    }


    TME_INFO("shutting down");

    // cleanup logic before the contextHandle destroys the OpenGL context
    tme::core::Storage<tme::app::Editor>::global()->clear();
    return 0;
}
