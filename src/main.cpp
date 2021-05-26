/** @file */

#include "core/storage.hpp"
#include "core/graphics/shader.hpp"
#include "core/graphics/texture.hpp"
#include "platform/context.hpp"
#include "app/editor.hpp"

/// Program entrypoint.
int main() {
    auto contextHandle = tme::platform::Context::create();
    tme::core::Log::init();
    TME_INFO("starting application");

    auto editor = tme::core::Storage<tme::app::Editor>::global()->create(tme::core::Storage<tme::app::Tilemap>::global()->create(32, 32, 350));
    editor->run();

    TME_INFO("shutting down");

    // cleanup logic before the contextHandle destroys the OpenGL context
    tme::core::Storage<tme::app::Editor>::global()->clear();
    tme::core::Storage<tme::core::graphics::Shader::Stage>::global()->clear();
    tme::core::Storage<tme::core::graphics::Shader>::global()->clear();
    tme::core::Storage<tme::core::graphics::Texture>::global()->clear();
    return 0;
}
