#include "gtest/gtest.h"

#include "sigcounter.hpp"
int SignalCounter::assertionFailed = SIGTRAP;

#include "core/log_test.cpp"
#include "core/loggable_test.cpp"
#include "core/key_test.cpp"
#include "core/events/event_test.cpp"
#include "core/events/dispatcher_test.cpp"
#include "core/events/window_test.cpp"
#include "core/events/key_test.cpp"
#include "core/events/mouse_test.cpp"
#include "core/window_test.cpp"
#include "platform/glfw_test.cpp"
#include "core/layers/layer_test.cpp"
#include "core/layers/imgui_test.cpp"
#include "core/storage_test.cpp"
#include "core/application_test.cpp"

int main(int argc, char** argv) {
    SignalCounter::instance()->listen(SignalCounter::assertionFailed);
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

