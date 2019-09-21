//
// Created by suun on 9/18/19.
//

#include "collector.h"
#include "utils.h"

#include <cerrno>
#include <fcntl.h>
#include <sys/stat.h>

Collector::Collector() : fd_(STDIN_FILENO), first_(true) {}

Collector::Collector(int fd) : fd_(fd), first_(true) {
  auto flags = fcntl(fd, F_GETFL, 0);
  fcntl(fd, F_SETFL, flags | O_NONBLOCK); // NOLINT(hicpp-signed-bitwise)
}

Collector::Collector(const char *fifo_name) : first_(true) {
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

bool Collector::update() {
  bool updated = false;
  for (auto &pair : blocks_) {
    updated |= pair.second->update();
  }
  if (first_ || updated) {
    return true;
  }
  fd_set fifoSet;
  FD_ZERO(&fifoSet);
  int second = std::dynamic_pointer_cast<TimeBlock>(
      blocks_[BLOCK_NAME_TIME])->getSecond();
  timeval timeout{std::min(10, second), 0};
  FD_SET(fd_, &fifoSet);
  select(fd_ + 1, &fifoSet, nullptr, nullptr, &timeout);
  if (!FD_ISSET(fd_, &fifoSet)) {
    return updated;
  }
  std::string s = readline(fd_);
  if (s.empty()) return updated;
  const char delimiter[] = "|";
  auto pos = s.find(delimiter);
  if (pos == std::string::npos) {
    fprintf(stderr, "line format not allowed: %s", s.c_str());
  }
  std::string name = s.substr(0, pos),
      desc = s.substr(pos + 1, s.size());
  if (desc.empty()) {
    blocks_.erase(name);
    return updated;
  }
  auto block = std::make_shared<CustomBlock>(name, desc);
  auto oldBlock = blocks_[name];
  std::shared_ptr<CustomBlock> customBlock =
      std::dynamic_pointer_cast<CustomBlock>(oldBlock);
  if (!customBlock) {
    updated = true;
    blocks_[name] = std::make_shared<CustomBlock>(name, desc);
  } else if (desc != customBlock->format()) {
    updated = true;
    customBlock->setDesc(desc);
  }
  blocks_[name] = block;
  return updated;
}

void Collector::print(FILE *fp) const {
  char const *itemFormat = R"({"name":"%s","markup":"none","full_text":"%s"})";
  if (first_) {
    first_ = false;
    fprintf(fp, "{\"version\":1}\n[\n[");
    fflush(fp);
  } else {
    fprintf(fp, ",[");
  }
  bool isFirstItem = true;
  for (auto &pair : blocks_) {
    const std::string &s = pair.second->format();
    if (isFirstItem) {
      isFirstItem = false;
    } else {
      fprintf(fp, ",");
    }
    fprintf(fp, itemFormat, pair.second->getName().c_str(), s.c_str());
  }
  fprintf(fp, "]\n");
  fflush(fp);
}

void Collector::append(const std::shared_ptr<Block> &block) {
  blocks_.insert(
      typename std::pair<const std::string, std::shared_ptr<Block>>(
          block->getName(), block));
}
