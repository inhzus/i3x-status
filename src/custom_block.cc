//
// Created by suun on 9/19/19.
//

#include "block.h"

#include <utility>

CustomBlock::CustomBlock(std::string name, std::string desc) :
    Block(std::move(name)), desc_(std::move(desc)) {}

void CustomBlock::update() {}

std::string CustomBlock::format() {
  return desc_;
}
