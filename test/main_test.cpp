#include "gtest/gtest.h"

#include "core/log_test.cpp"
#include "core/loggable_test.cpp"
#include "core/events/event_test.cpp"
#include "core/events/dispatcher_test.cpp"

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

