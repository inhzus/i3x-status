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
  virtual bool update() = 0;
  virtual std::string format() = 0;
};

class PowerBlock : public Block {
 private:
  Status status_;
  unsigned percent_;
 public:
  explicit PowerBlock();
  bool update() override;
  std::string format() override;
};

class TimeBlock : public Block {
 private:
  std::tm *now_;
 public:
  explicit TimeBlock();
  bool update() override;
  std::string format() override;
  int getSecond();
};

class CustomBlock : public Block {
 private:
  std::string desc_;
 public:
  explicit CustomBlock(std::string name, std::string desc);
  bool update() override;
  std::string format() override;
  void setDesc(std::string desc);
};

#endif //I3X_STATUS_BLOCK_H
