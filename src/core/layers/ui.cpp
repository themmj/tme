/** @file */
#include "core/layers/ui.hpp"
#include "core/events/window.hpp"

namespace tme {
    namespace core {
        namespace layers {

            void Ui::onEvent(events::Event& event) {
                dispatchEvent<events::WindowUpdate>(event, &Ui::handleWindowUpdate);
            }

            bool Ui::handleWindowUpdate(events::WindowUpdate&) {
                update();
                return false;
            }

        }
    }
}

