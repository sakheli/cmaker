#include <iostream>
#include <string>
#include <regex>
#include <memory>
#include "../../Command/Command.hpp"
#include "../Generators.hpp"


namespace Generators::ConfigJson{
  /*
   * Validates the build directory
   * @param prefix: the prefix of the message
   * @param ctx: the context of the command
   * @param config_json: the config json context
   * @return: true if the build directory is valid
   */
  bool validateBuildDir(std::string prefix, std::shared_ptr<Command::Context> ctx, std::shared_ptr<Config> config_json) { 
    std::cout << prefix;
#ifndef TEST
      std::getline(std::cin, config_json->build_dir);
#endif
    //If the build directory is empty we're gonna set it
    if(config_json->build_dir == "") {
      goto end;
    }
    if (config_json->build_dir.size() > 255) {
      return false;
    }
    if (config_json->build_dir == "build") {
      goto end;
    }
    //check if the build directory is valid
    if (std::regex_match(config_json->build_dir, std::regex("^[a-zA-Z0-9_-]+$"))) {
      goto end;
    }
    if (sizeof(config_json->build_dir) > 255) {
      goto end;
    }
    return false;
    end:
      ctx->build_dir = config_json->build_dir == "" ? ctx->build_dir : config_json->build_dir;
    
    return true;
  }
}
