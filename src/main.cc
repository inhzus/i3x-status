#include <clocale>
#include <chrono>
#include <memory>
#include <csignal>
#include <thread>
#include <unistd.h>

#include "collector.h"

bool isExit = false;

int main() {
  setlocale(LC_ALL, "");
  signal(SIGINT, [](int n) {
    isExit = true;
  });
  Collector collector("/tmp/i3x.status");
  collector.append(std::make_shared<PowerBlock>());
  collector.append(std::make_shared<TimeBlock>());
  while (true) {
    if (isExit) {
      break;
    }
    collector.update();
    collector.print(STDOUT_FILENO);
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
  return 0;
}
