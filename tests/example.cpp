//
// Created by mateus on 22/04/2021.
//

#include "gtranslator.h"
#include <gtest/gtest.h>

// Demonstrate some basic assertions.
TEST(SIMPLETEST, BasicTranslation) {
    gtranslator::GTranslator translator;

    auto result = translator.translate("olá mundo", "pt", "en");
    EXPECT_STREQ(result.c_str(), "Hello World");
}

GTEST_API_ int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}