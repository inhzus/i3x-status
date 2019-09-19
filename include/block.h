//
// Created by suun on 9/18/19.
//

#ifndef I3X_STATUS_BLOCK_H
#define I3X_STATUS_BLOCK_H

#include <ctime>
#include <memory>
#include <string>
#include <vector>

extern char const *BLOCK_NAME_POWER, *BLOCK_NAME_TIME;

enum Status {
  POWER_BAT,
  POWER_AC,
  POWER_FULL
};

class Block {
 private:
  std::string name_;
 public:
  Block() = default;
  explicit Block(std::string name) : name_(std::move(name)) {};
  [[nodiscard]] std::string getName() const { return name_; }
  virtual void update() = 0;
  virtual std::string format() = 0;
};

class PowerBlock : public Block {
 private:
  Status status_;
  unsigned percent_;
 public:
  explicit PowerBlock();
  void update() override;
  std::string format() override;
};

class TimeBlock : public Block {
 private:
  time_t now_;
 public:
  explicit TimeBlock();
  void update() override;
  std::string format() override;
};

#endif //I3X_STATUS_BLOCK_H
