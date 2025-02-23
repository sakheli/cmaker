#include "Command.hpp"
#include <functional>
#include <iostream>
#include <sys/epoll.h>
#include <sys/inotify.h>

namespace Command {

  size_t walk(int epoll_fd, std::string path){
    std::vector<int> filedescriptors;

    for(auto filetoken:std::filesystem::recursive_directory_iterator(path)){
      if(std::filesystem::is_directory(filetoken.path())){
        std::cout << filetoken.path() << std::endl;
        int temp_fd = inotify_init();
        if(temp_fd < 0){
          std::cout << "Error creating inotify instance" << std::endl;
          exit(1);
        }
        int err = inotify_add_watch(temp_fd, filetoken.path().c_str(), IN_MODIFY | IN_CREATE | IN_DELETE | IN_MOVED_FROM | IN_MOVED_TO);
        if(err < 0){
          std::cout << "Error adding watch" << std::endl;
          exit(1);
        }
        filedescriptors.push_back(temp_fd);
      }
    };


    for(int fd: filedescriptors){
      struct epoll_event temp_ev;
      temp_ev.data.fd = fd;
      temp_ev.events = EPOLLIN;
      epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &temp_ev);
    }
    return filedescriptors.size();

  }


  void watcher(const std::function<void()> &changeCallback, std::string path) {
    int inotify_fd = inotify_init();
    int epoll_fd = epoll_create(1);

    // Add inotify file descriptor to the epoll instance
    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = inotify_fd;
    // TODO: Add recursive directory watching

    size_t watch_desc = inotify_add_watch(inotify_fd, path.c_str(), IN_MODIFY | IN_CREATE | IN_DELETE | IN_MOVED_FROM | IN_MOVED_TO);
    if (watch_desc < 0) {
      std::cout << "Error adding watch" << std::endl;
      return;
    }
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, inotify_fd, &ev);
    size_t filedescriptors = walk(epoll_fd, path);



    // Add the directory you want to watch


    // Now, you can use epoll to wait for events
    struct epoll_event events[1];
    while (true) {
      std::cout << "\nWaiting for changes...\n" << std::endl;
      int num_events = epoll_wait(epoll_fd, events, 1, -1);
      std::cout << "\nChange detected!\n" << std::endl;
      std::vector<std::string> dirs;
#ifdef DEBUG
      for(auto filetoken:std::filesystem::recursive_directory_iterator(path)){
#else
      for(auto filetoken:std::filesystem::recursive_directory_iterator(path)){
#endif
          if(std::filesystem::is_directory(filetoken.path())){
            dirs.push_back(filetoken.path());
          }
      }
      if (dirs.size() != filedescriptors){
        std::cout << "\nDirectory change detected\n" << std::endl;
        filedescriptors= walk(epoll_fd, path.c_str());
      }
      if (num_events > 0) {
        char buffer[1024];
        while (read(events[0].data.fd,&buffer, sizeof(buffer))<0) {

        }
        std::cout << buffer << std::endl;
        changeCallback();

      }
    }


    // Don't forget to clean up your file descriptors when you're done
    close(watch_desc);
    close(inotify_fd);
    close(epoll_fd);
  }

  bool Interface::watch() {
    // This is where you call the watcher function and provide a lambda for the
    // callback

    
#ifdef DEBUG
    const std::string path = "./build/src";
#else
    const std::string path = "./src";

#endif

    watcher([this]() {
#ifdef DEBUG
        std::string command = "cmake ./build/ && ./build/make && ./build/" + ctx->build_dir + "/" + ctx->project_name;
        if (args->count("args") != 0) {
          std::vector<std::string> args_vec =  args->operator[]("args").as<std::vector<std::string>>();
          std::string command_args = std::accumulate(
            args_vec.begin(), args_vec.end(), args_vec[0],
            [](std::string a, std::string b) { return a + " " + b; }
          );

           command = "cmake ./build/ && make && ./build/" + ctx->build_dir + "/" + ctx->project_name + "  " + command_args;
        }

#else
        std::string command = "cmake . && make && ./" + ctx->build_dir + "/" + ctx->project_name;
        if (args->count("args") != 0) {
          std::cout << "estamos aqui" << std::endl;
          std::vector<std::string> args_vec =  args->operator[]("args").as<std::vector<std::string>>();
          std::string command_args = args_vec[0];
          if (args_vec.size() > 1) {
            command_args = std::accumulate(
              args_vec.begin(), args_vec.end(), args_vec[0],
              [](std::string a, std::string b) { return a + " " + b; }
          );
            std::cout << "args size is 0" << std::endl;
            return;
          }

          std::cout << "command_args: " << command_args << std::endl;
          command = "cmake . && make && ./" + ctx->build_dir + "/" + ctx->project_name + "  " + command_args;
        }
#endif
        std::cout << "Running command: " << command << std::endl;
        int success = system(command.c_str());
        if (success != 0) {
          std::cout << "Error running project" << std::endl;
        }

        // Call your recompilation command or any other action you want
        },path);
    return true;
  }
} // namespace Command

