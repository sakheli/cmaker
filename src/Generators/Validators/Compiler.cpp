
#include <iostream>
#include <string>
#include <regex>
#include <memory>
#include "../../Command/Command.hpp"
#include "../Generators.hpp"


namespace Generators::ConfigJson{
  /*
   * Validates the compiler 
   * @param prefix: the prefix of the message
   * @param ctx: the context of the command
   * @param config_json: the config json context
   * @return: true if the language is valid
   */
  bool validateCompiler(std::string prefix, std::shared_ptr<Command::Context> ctx, std::shared_ptr<Config> config_json) {
    std::vector<std::string> supportedCppCompilers = {"g++", "clang++"};
    std::vector<std::string> supportedCCompilers = {"gcc", "clang", "msvc", "icc", "tcc", "emcc"};
    std::cout << prefix << ENDL;
    if(ctx->lang == "cpp" || ctx->lang == "c++"){
      ctx->compiler = supportedCppCompilers[0];
      std::cout << "  Supported Compilers: ( ";
      for(std::string comp : supportedCppCompilers){
        if(comp == supportedCppCompilers[0]){
          std::cout << " [ " << comp << " ] ";
          continue;
        }else{
          std::cout << comp << " ";
        }
      }
    }
    if(config_json->lang == "c"){
      ctx->compiler = supportedCCompilers[0];
      std::cout << "  Supported Compilers: ";
      for(std::string comp : supportedCCompilers){
        if(comp == supportedCCompilers[0]){
          std::cout << " [ " << comp << " ] ";
          continue;
        }else{
          std::cout << comp << " ";
        }
      }
    }
    std::cout << "): ";
#ifndef TEST
      std::getline(std::cin, config_json->compiler);
#endif
    //If the compiler is empty we're gonna set it
    if(config_json->compiler == "") {
      goto end;
    }
    if(config_json->lang == "cpp" || config_json->lang == "c++"){
      for(std::string compiler : supportedCppCompilers){
        if(config_json->compiler == compiler){
          goto end;
        }
      }
    }else if(config_json->lang == "c"){
      for(std::string compiler : supportedCCompilers){
        if(config_json->compiler == compiler){
          goto end;
        }
      }
    }

    return false;
    end:
      ctx->compiler = config_json->compiler == "" ? ctx->compiler : config_json->compiler;
    
    return true;
  }
}
