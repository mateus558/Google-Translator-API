//
// Created by mateuscmarim on 17/12/2020.
//

#ifndef UNTITLED_TRANSLATORS_H
#define UNTITLED_TRANSLATORS_H

#include <string>
#include <map>
#include <curl/curl.h>

namespace gtranslator{
        class GTranslator{
        private:
            CURL *curl{nullptr};
            struct curl_slist *chunk = nullptr;
            static bool use_cached, cache_loaded;
            static std::map<std::string, std::string> cache;

            static bool load_cache();
            static void save_cache();
            static size_t write_callback(void* ptr, size_t size, size_t nmemb, std::string* data);
        public:
            explicit GTranslator();
            ~GTranslator();

            static void set_use_cached(bool use);
            std::string translate(const std::string& text, const std::string& from, const std::string& to);
        };
    }


#endif //UNTITLED_TRANSLATORS_H
