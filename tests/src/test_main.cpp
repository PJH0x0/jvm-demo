#include "gtest/gtest.h"
#include <glog/logging.h>
#include <iomanip>
#include <unistd.h>

void initLogPrefix(std::ostream& s, const google::LogMessageInfo &l, void*) {
  
  s << std::setw(2) << 1 + l.time.month()
   << '-'
   << std::setw(2) << l.time.day()
   << ' '
   << std::setw(2) << l.time.hour() << ':'
   << std::setw(2) << l.time.min()  << ':'
   << std::setw(2) << l.time.sec()
   //<< std::setw(3) << l.time.usec()
   << ' '
   << l.severity[0]
   << ' '
   << std::setfill(' ') << std::setw(5)
   << getpid() << std::setfill('0')
   << ' '
   << std::setfill(' ') << std::setw(5)
   << l.thread_id << std::setfill('0')
   #ifdef LOG_TAG
   << ' '
   << std::setfill(' ') << std::setw(5)
   << LOG_TAG << std::setfill('0')
   #else
   << ' '
   << std::setfill(' ') << std::setw(5)
   << l.filename << std::setfill('0')
   #endif
   ;
  //  << ' '
  //  << l.filename << ':' << l.line_number;
}
void initGlog(char* program) {
  google::InitGoogleLogging(program, &initLogPrefix);
  google::SetStderrLogging(google::GLOG_INFO);
  FLAGS_colorlogtostderr = true;
}

int main(int argc, char **argv) {
  initGlog(argv[0]);
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}