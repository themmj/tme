#include "gtest/gtest.h"
#include "core/log.hpp"

namespace tme {
    namespace core {

        TEST(LogTest, Initialization) {
            auto& logger = Log::getInstance();
            EXPECT_EQ(logger, nullptr);
            Log::init();
            EXPECT_NE(logger, nullptr);
        }

    }
}
