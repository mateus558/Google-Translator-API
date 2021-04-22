#include "gtranslator.h"
#include "json.hpp"
#include <iostream>
#include <fstream>
#include <set>
#include <filesystem>
#define API_URL "https://translation.googleapis.com/language/translate/v2?key="
#define CACHE_FILE "gtranslator.cache"

using json = nlohmann::json;

namespace gtranslator{
    std::map<std::string, std::string> GTranslator::cache = std::map<std::string, std::string>();
    bool GTranslator::use_cached = false, GTranslator::cache_loaded = false;

    GTranslator::GTranslator() {
        if(use_cached && !cache_loaded) load_cache();
    }

    GTranslator::GTranslator(const std::string& key, bool is_file) {
        assert((key != "") && "A key must be provided to request from google API.");
        if(is_file){
            std::ifstream key_file("key");
            key_file >> this->api_key;
            key_file.close();
        }else{
            this->api_key = key;
        }
        init_curl();
        if(use_cached && !cache_loaded) load_cache();
    }

    void GTranslator::init_curl() {
        if(curl){
            curl_easy_cleanup(curl);
            curl = nullptr;
        }
        curl_global_init(CURL_GLOBAL_ALL);
        chunk = curl_slist_append(this->chunk, "Content-Type:application/json");
        curl = curl_easy_init();
        if (curl) {
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
            curl_easy_setopt(curl, CURLOPT_URL, (std::string(API_URL)+this->api_key).c_str());
            curl_easy_setopt(curl, CURLOPT_TCP_FASTOPEN, 1L);
            curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1L);
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, GTranslator::write_callback);
        }
    }

    size_t GTranslator::write_callback(void* ptr, size_t size, size_t nmemb, std::string* data) {
        data->append((char*)ptr, size * nmemb);
        return size * nmemb;
    }

    std::string GTranslator::translate(const std::string& text, const std::string& from, const std::string& to){
        std::string readBuffer;

        // Verify if Sentence exist on translations cache
        if(cache.find(text) != cache.end() && use_cached){
            if(!cache[text].empty()) return cache[text];
        }

        if(curl && !this->api_key.empty()) {
            std::string params = R"({"q":")";
            params += text + std::string(R"(", "source":")");
            params += from + std::string(R"(", "target":")");
            params += to + std::string("\"}");

            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, params.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
            curl_easy_perform(curl);
        }

        try{
            json response = json::parse(readBuffer);
            std::string translation = response["data"]["translations"][0]["translatedText"].dump();
            cache[text] = translation;
            cache[translation] = text;
            return translation;
        }catch (...){
            // output exception information
            std::cerr << readBuffer << std::endl;
//            std::cerr << "message: " <<  e.errorString().toUtf8().constData() << '\n'
//                      << "exception id: " << e.error << '\n'
//                      << "byte position of error: " << e.offset << std::endl;
            return "";
        }
    }

    bool GTranslator::load_cache() {
        if(GTranslator::cache_loaded){
            return true;
        }

        // check if file exists and if yes: Is it really a file and no directory?
        if (std::filesystem::exists(CACHE_FILE)) {
            std::ifstream file(CACHE_FILE);

            if(!file){
                return false;
            }

            size_t i = 0;
            std::string pt, en;
            for( std::string line; getline( file, line ); i++) {
                if(i%2 == 0){
                    pt = line;
                }else{
                    en = line;
                    cache[pt] = en;
                    cache[en] = pt;
                }
            }
            file.close();
            GTranslator::cache_loaded = true;
            return true;
        }

        return false;
    }

    void GTranslator::save_cache() {
        std::ofstream file(CACHE_FILE, std::ofstream::out | std::ofstream::trunc);
        std::set<std::string> already_in_file;

        if(!file){
            return ;
        }

        for(auto const &x: cache){
            if(already_in_file.find(x.first) == already_in_file.end()) {
                file << x.first << '\n';
                already_in_file.insert(x.first);
            }
            if(already_in_file.find(x.second) == already_in_file.end()) {
                if(x.second == "") file << "\n"; else file << x.second << '\n';
                already_in_file.insert(x.second);
            }
        }
        file.close();
    }

    GTranslator::~GTranslator() {
        curl_easy_cleanup(curl);
        curl = nullptr;
        save_cache();
    }

    void GTranslator::set_use_cached(bool use) {
        use_cached = use;
    }

    void GTranslator::set_key(const std::string &key) {
        this->api_key = key;
        init_curl();
    }

}