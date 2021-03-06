//
// Created by suun on 9/18/19.
//

#include "block.h"
#include "utils.h"

#include <regex>

char const *BLOCK_NAME_POWER = "~power";

bool caseEqual(const std::string &l, const std::string &r) {
  return std::equal(
      l.begin(), l.end(), r.begin(), r.end(),
      [](char a, char b) {
        return tolower(a) == tolower(b);
      });
}

PowerBlock::PowerBlock() :
    Block(BLOCK_NAME_POWER), status_(), percent_() {
  update();
}

bool PowerBlock::update() {
  FILE *fp = popen("acpi -b", "r");
  int fd = fileno(fp);
  std::string s = readline(fd);
  if (s.empty()) {
    return false;
  }
  std::regex pattern("\\d: ([^,]+), (\\d+)"); // NOLINT
  // (modernize-raw-string-literal)
  std::smatch match;
  std::regex_search(s, match, pattern);
  std::string status = match[1].str();
  unsigned oldPercent = percent_;
  Status oldStatus = status_;
  percent_ = std::stoi(match[2].str());
  if (caseEqual(status, "Discharging")) {
    status_ = POWER_BAT;
  } else if (caseEqual(status, "Unknown")) {
    status_ = POWER_BAT;
  } else if (caseEqual(status, "Charging")) {
    status_ = POWER_AC;
  } else if (caseEqual(status, "Full")) {
    status_ = POWER_FULL;
  } else {
    status_ = POWER_BAT;
  }
  pclose(fp);
  return oldPercent != percent_ || oldStatus != status_;
}

std::string PowerBlock::format() {
  char const *c = " ";
  switch (status_) {
    case POWER_AC:c = "\uF1E6";
      break;
    case POWER_BAT:
    case POWER_FULL:c = "\uF213";
      break;
  }
  char const *format = "%s %d%%";
  auto size = snprintf(nullptr, 0, format, c, percent_) + 1;
  std::unique_ptr<char[]> buf(new char[size]);
  snprintf(buf.get(), size, format, c, percent_);
  return std::string(buf.get(), buf.get() + size - 1);
}
