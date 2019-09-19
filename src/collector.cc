//
// Created by suun on 9/18/19.
//

#include "collector.h"
#include "utils.h"

#include <cerrno>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

Collector::Collector() : fd_(STDIN_FILENO) {}

Collector::Collector(int fd) : fd_(fd) {
  auto flags = fcntl(fd, F_GETFL, 0);
  fcntl(fd, F_SETFL, flags | O_NONBLOCK); // NOLINT(hicpp-signed-bitwise)
}

Collector::Collector(const char *fifo_name) {
  auto FIFO_MODE =
      (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH); // NOLINT(hicpp-signed-bitwise)
  if ((mkfifo(fifo_name, FIFO_MODE) < 0) && (errno != EEXIST)) {
    fprintf(stderr, "cannot create fifo %s", fifo_name);
    exit(-1);
  }
  fd_ = open(
      fifo_name, O_RDONLY | O_NONBLOCK, 0); // NOLINT(hicpp-signed-bitwise)
  open(fifo_name, O_WRONLY, 0);
}

void Collector::update() {
  for (auto &pair : blocks_) {
    pair.second->update();
  }
  while (true) {
    std::string s = readline(fd_);
    if (s.empty()) break;
    const char delimiter[] = "|";
    auto pos = s.find(delimiter);
    if (pos == std::string::npos) {
      fprintf(stderr, "line format not allowed: %s", s.c_str());
    }
    std::string name = s.substr(0, pos),
        desc = s.substr(pos + 1, s.size());
    if (desc.empty()) {
      blocks_.erase(name);
      continue;
    }
    auto block = std::make_shared<CustomBlock>(name, desc);
    blocks_[name] = block;
  }
}

void Collector::print(int fd) const {
  bool isFirst = true;
  for (auto &pair : blocks_) {
    const std::string &s = pair.second->format();
    if (isFirst) {
      isFirst = false;
    } else {
      write(fd, " | ", 3);
    }
    write(fd, s.c_str(), s.size());
  }
  write(fd, "\n", 1);
}

void Collector::append(const std::shared_ptr<Block> &block) {
  blocks_.insert(
      typename std::pair<const std::string, std::shared_ptr<Block>>(
          block->getName(), block));
}
