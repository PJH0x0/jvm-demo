#pragma once

#include "base/base_insts.h"
#include <cstdint>
#include <cmath>
#include <type_traits>

namespace instructions {

template<typename T>
void _cmp(std::shared_ptr<runtime::Frame> frame, bool gFlag = false) {
  runtime::OperandStack& stack = frame->getOperandStack();
  T v1 = PopOperandStack<T>(stack);
  T v2 = PopOperandStack<T>(stack);
  if (std::is_floating_point<T>::value) {
    if (std::isnan(v1) || std::isnan(v2)) {
      PushOperandStack(stack, gFlag ? 1 : -1) ;
    }
  }
  if (v2 > v1) {
    PushOperandStack(stack, 1);
  } else if (v2 == v1) {
    PushOperandStack(stack, 0);
  } else {
    PushOperandStack(stack, -1);
  }
}
class LCMP : public NoOperandsInstruction {
  public:
  void Execute(std::shared_ptr<runtime::Frame> frame) override {
    _cmp<int32_t>(frame);
  }
};

template<typename T>
class FCMPG : public NoOperandsInstruction {
  public:
  void Execute(std::shared_ptr<runtime::Frame> frame) override {
    static_assert(std::is_floating_point<T>::value, "Call FCMP without float point");
    _cmp<T>(frame, true);
  }
};

template<typename T>
class FCMPL : public NoOperandsInstruction {
  public:
  void Execute(std::shared_ptr<runtime::Frame> frame) override {
    static_assert(std::is_floating_point<T>::value, "Call FCMP without float point");
    _cmp<T>(frame);
  }
};

class IFEQ : public BranchInstruction {
  public:
  void Execute(std::shared_ptr<runtime::Frame> frame) override {
    runtime::OperandStack& stack = frame->getOperandStack();
    int32_t val = PopOperandStack<int32_t>(stack);
    if (!val) {
      branch(frame);
    }
  }
};
class IFNE : public BranchInstruction {
  public:
  void Execute(std::shared_ptr<runtime::Frame> frame) override {
    runtime::OperandStack& stack = frame->getOperandStack();
    int32_t val = PopOperandStack<int32_t>(stack);
    if (val) {
      branch(frame);
    }
  }
};

class IFLT : public BranchInstruction {
  public:
  void Execute(std::shared_ptr<runtime::Frame> frame) override {
    runtime::OperandStack& stack = frame->getOperandStack();
    int32_t val = PopOperandStack<int32_t>(stack);

    if (val < 0) {
      branch(frame);
    }
  }
};
class IFLE : public BranchInstruction {
  public:
  void Execute(std::shared_ptr<runtime::Frame> frame) override {
    runtime::OperandStack& stack = frame->getOperandStack();
    int32_t val = PopOperandStack<int32_t>(stack);

    if (val <= 0) {
      branch(frame);
    }
  }
};
class IFGT : public BranchInstruction {
  public:
  void Execute(std::shared_ptr<runtime::Frame> frame) override {
    runtime::OperandStack& stack = frame->getOperandStack();
    int32_t val = PopOperandStack<int32_t>(stack);

    if (val > 0) {
      branch(frame);
    }
  }
};
class IFGE : public BranchInstruction {
  public:
  void Execute(std::shared_ptr<runtime::Frame> frame) override {
    runtime::OperandStack& stack = frame->getOperandStack();
    int32_t val = PopOperandStack<int32_t>(stack);

    if (val >= 0) {
      branch(frame);
    }
  }
};
class IF_ICMPEQ : public BranchInstruction {
  public:
  void Execute(std::shared_ptr<runtime::Frame> frame) override {
    runtime::OperandStack& stack = frame->getOperandStack();
    int32_t val1 = PopOperandStack<int32_t>(stack);
    int32_t val2 = PopOperandStack<int32_t>(stack);
    if (val2 == val1) {
      branch(frame);
    }
  }
};
class IF_ICMPNE : public BranchInstruction {
  public:
  void Execute(std::shared_ptr<runtime::Frame> frame) override {
    runtime::OperandStack& stack = frame->getOperandStack();
    int32_t val1 = PopOperandStack<int32_t>(stack);
    int32_t val2 = PopOperandStack<int32_t>(stack);
    if (val2 != val1) {
      branch(frame);
    }
  }
};

class IF_ICMPLT : public BranchInstruction {
  public:
  void Execute(std::shared_ptr<runtime::Frame> frame) override {
    runtime::OperandStack& stack = frame->getOperandStack();
    int32_t val1 = PopOperandStack<int32_t>(stack);
    int32_t val2 = PopOperandStack<int32_t>(stack);
    if (val2 < val1) {
      branch(frame);
    }
  }
};
class IF_ICMPLE : public BranchInstruction {
  public:
  void Execute(std::shared_ptr<runtime::Frame> frame) override {
    runtime::OperandStack& stack = frame->getOperandStack();
    int32_t val1 = PopOperandStack<int32_t>(stack);
    int32_t val2 = PopOperandStack<int32_t>(stack);
    if (val2 <= val1) {
      branch(frame);
    }
  }
};
class IF_ICMPGT : public BranchInstruction {
  public:
  void Execute(std::shared_ptr<runtime::Frame> frame) override {
    runtime::OperandStack& stack = frame->getOperandStack();
    int32_t val1 = PopOperandStack<int32_t>(stack);
    int32_t val2 = PopOperandStack<int32_t>(stack);
    if (val2 > val1) {
      branch(frame);
    }
  }
};
class IF_ICMPGE : public BranchInstruction {
  public:
  void Execute(std::shared_ptr<runtime::Frame> frame) override {
    runtime::OperandStack& stack = frame->getOperandStack();
    int32_t val1 = PopOperandStack<int32_t>(stack);
    int32_t val2 = PopOperandStack<int32_t>(stack);
    if (val2 >= val1) {
      branch(frame);
    }
  }
};
class IF_ACMPEQ : public BranchInstruction {
  public:
  void Execute(std::shared_ptr<runtime::Frame> frame) override {
    runtime::OperandStack& stack = frame->getOperandStack();
    void* ref1 = stack.popRef();
    void* ref2 = stack.popRef();
    if (ref2 == ref1) {
      branch(frame);
    }
  }
};
class IF_ACMPNE : public BranchInstruction {
  public:
  void Execute(std::shared_ptr<runtime::Frame> frame) override {
    runtime::OperandStack& stack = frame->getOperandStack();
    void* ref1 = stack.popRef();
    void* ref2 = stack.popRef();
    if (ref2 != ref1) {
      branch(frame);
    }
  }
};
}