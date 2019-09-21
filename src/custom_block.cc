//
// Created by suun on 9/19/19.
//

#include "block.h"

#include <utility>

CustomBlock::CustomBlock(std::string name, std::string desc) :
    Block(std::move(name)), desc_(std::move(desc)) {}

bool CustomBlock::update() { return false; }

std::string CustomBlock::format() {
  return desc_;
}
void CustomBlock::setDesc(std::string desc) {
  desc_ = std::move(desc);
}
