//
// Created by mateus on 22/04/2021.
//

#include <fstream>
#include <string>
#include "gtranslator.h"
#include <gtest/gtest.h>

// Demonstrate some basic assertions.
TEST(SIMPLETEST, BasicTranslationWithDirectKey) {
    std::ifstream key_file("key");
    std::string key;
    key_file >> key;
    key_file.close();
    gtranslator::GTranslator translator(key);

    auto result = translator.translate("olá mundo", "pt", "en");
    EXPECT_STREQ(result.c_str(), "\"Hello World\"");
}

TEST(SIMPLETEST, BasicTranslationWithFileKey) {
    gtranslator::GTranslator translator("key", true);

    auto result = translator.translate("olá mundo", "pt", "en");
    EXPECT_STREQ(result.c_str(), "\"Hello World\"");
}

GTEST_API_ int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}