#ifndef CORE_SYSTEM_CONTEXT_H_
#define CORE_SYSTEM_CONTEXT_H_

namespace core {


enum class ContextKind {
  kDefault,
  kThread,
};

class Context {
 public:
  Context() {}
  Context(const ContextKind kind) { (void)kind; }
};
  
class WithContext {
 public:
  explicit WithContext(const Context& x) {(void)x; }
  ~WithContext() {}
};


} // namespace core
#endif // CORE_SYSTEM_CONTEXT_H_
