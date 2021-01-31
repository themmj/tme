#include "gtest/gtest.h"
#include <sstream>
#include <string>
#include "core/loggable.hpp"

namespace tme {
    namespace core {

        class _TestStreamable : public Loggable {
            public:
            static const std::string s_testStreamableContent;
            std::string toString() const override {
                return s_testStreamableContent;
            }
        };

        const std::string _TestStreamable::s_testStreamableContent{"this is a test"};

        TEST(LoggableTest, StreamOutput) {
            std::stringstream ss;
            _TestStreamable ts;
            ss << ts;
            std::string res = ss.str();
            EXPECT_EQ(res.compare(_TestStreamable::s_testStreamableContent), 0);
        }

    }
}

