//
// Created by suun on 9/19/19.
//

#include "utils.h"
#include <unistd.h>

std::string readline(int fd) {
  char c;
  std::string buf;
  while (read(fd, &c, 1) > 0) {
    if (c == '\n') {
      break;
    }
    buf.push_back(c);
  }
  return buf;
}

