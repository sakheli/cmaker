#include <iostream>
#include <string>
#include <regex>
#include <memory>
#include "../../Command/Command.hpp"
#include "../Generators.hpp"


namespace Generators::ConfigJson{
  /*
   * Validates the language
   * @param prefix: the prefix of the message
   * @param ctx: the context of the command
   * @param config_json: the config json context
   * @return: true if the language is valid
   */
  bool validateLang(std::string prefix, std::shared_ptr<Command::Context> ctx, std::shared_ptr<Config> config_json) {
    std::vector<std::string> supportedLangs = {"cpp", "c"};
    std::cout << prefix << ENDL;
    std::cout << "  Supported languages: ( ";
    for(std::string lang : supportedLangs){
      std::cout << lang << " ";
    }
    std::cout << "): ";
#ifndef TEST
      std::getline(std::cin, config_json->lang);
#endif
    
    if(config_json->lang == "") {
      goto end;
    }
    for(std::string lang : supportedLangs){
      if(config_json->lang == lang){
        goto end;
      }
    }
    return false;
    end:
      ctx->lang = config_json->lang == "" ? ctx->lang : config_json->lang;
    return true;
  }
}
