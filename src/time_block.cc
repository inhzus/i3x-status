//
// Created by suun on 9/18/19.
//

#include "block.h"
#include <iomanip>

char const *BLOCK_NAME_TIME = "~time";

TimeBlock::TimeBlock() :
    Block(BLOCK_NAME_TIME), now_(nullptr) {
  update();
}

bool TimeBlock::update() {
  auto t = time(nullptr);
  int oldMinute = now_ ? now_->tm_min : -1;
  now_ = localtime(&t);
  return oldMinute != now_->tm_min;
}

std::string TimeBlock::format() {
  char buf[256];
  strftime(buf, sizeof(buf), "%d/%m %H:%M ", now_);
  return std::string(buf);
}
int TimeBlock::getSecond() {
  return now_ ? now_->tm_sec : -1;
}
