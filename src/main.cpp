#include <iostream>
#include <ostream>
#include <unistd.h>


#include "command.h"
#include <memory>
#include <glog/logging.h>
#include <iomanip>
#include <runtime/native/native_method.h>
#include "jvm.h"


//using namespace JVM;

#define VERSION 1.0
//#define LOG_TAG "main"


void InitLogPrefix(std::ostream& s, const google::LogMessageInfo &l, void*) {
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

void InitGlog(const char* program) {
  google::InitGoogleLogging(program, &InitLogPrefix);
  google::SetStderrLogging(google::GLOG_INFO);
  FLAGS_colorlogtostderr = true;
}

int main(int argc, char** argv) {
  InitGlog(argv[0]);
  std::shared_ptr<Command> start_cmd = Command::ParseCommand(argc, argv);
  if (start_cmd->PrintVersion()) {
    std::cout << "version " << VERSION << std::endl;
  } else if (start_cmd->PrintHelp() || start_cmd->GetClassName().empty()) {
    std::cout << "help" << std::endl;
  } else {
    JVM::CreateVM(start_cmd);
    JVM::Current()->Start();
  }
  google::ShutdownGoogleLogging();

  return 0;
}