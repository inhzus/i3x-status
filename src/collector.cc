//
// Created by suun on 9/18/19.
//

#include "collector.h"
#include <unistd.h>

void Collector::update() {
  for (auto &pair : blocks_) {
    pair.second->update();
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
