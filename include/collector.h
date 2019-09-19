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
  std::map<std::string, std::shared_ptr<Block>> blocks_;
 public:
  Collector() = default;
  void update();
  void print(int fd) const;
  void append(const std::shared_ptr<Block> &block);
};

#endif //I3X_STATUS_COLLECTOR_H
