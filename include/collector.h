//
// Created by suun on 9/18/19.
//

#ifndef I3X_STATUS_COLLECTOR_H
#define I3X_STATUS_COLLECTOR_H

#include "block.h"
#include <map>
#include <memory>

class Collector {
 private:
  int fd_;
  mutable bool first_;
  std::map<std::string, std::shared_ptr<Block>> blocks_;
 public:
  Collector();
  explicit Collector(int fd);
  explicit Collector(const char *filename);
  void update();
  void print(int fd) const;
  void append(const std::shared_ptr<Block> &block);
};

#endif //I3X_STATUS_COLLECTOR_H
