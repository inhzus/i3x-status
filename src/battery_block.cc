//
// Created by suun on 9/18/19.
//

#include "block.h"

char const *BLOCK_NAME_POWER = "power";

PowerBlock::PowerBlock() :
    Block(BLOCK_NAME_POWER), status_(), percent_() {
  update();
}

void PowerBlock::update() {
}

std::string PowerBlock::format() {
  char const *c = " ";
  switch (status_) {
    case POWER_AC:c = "\uF1E6";
      break;
    case POWER_BAT:
    case POWER_FULL:c = "\uF1E6";
      break;
  }
  char const *format = "%s %d%%%%";
  auto size = snprintf(nullptr, 0, format, c, percent_);
  std::unique_ptr<char[]> buf(new char[size + 1]);
  snprintf(buf.get(), size, format, c, percent_);
  return std::string(buf.get(), buf.get() + size);
}
