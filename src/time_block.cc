//
// Created by suun on 9/18/19.
//

#include "block.h"
#include <iomanip>

char const *BLOCK_NAME_TIME = "~time";

TimeBlock::TimeBlock() :
    Block(BLOCK_NAME_TIME), now_() {
  update();
}

void TimeBlock::update() {
  now_ = time(nullptr);
}

std::string TimeBlock::format() {
  char buf[256];
  std::tm *info = localtime(&now_);
  strftime(buf, sizeof(buf), "%d/%m %H:%M", info);
  return std::string(buf);
}
