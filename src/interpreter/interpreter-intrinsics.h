// Copyright 2015 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef V8_INTERPRETER_INTERPRETER_INTRINSICS_H_
#define V8_INTERPRETER_INTERPRETER_INTRINSICS_H_

#include "src/allocation.h"
#include "src/base/smart-pointers.h"
#include "src/builtins.h"
#include "src/frames.h"
#include "src/interpreter/bytecodes.h"
#include "src/interpreter/interpreter-assembler.h"
#include "src/runtime/runtime.h"

namespace v8 {
namespace internal {

namespace compiler {
class Node;
}  // namespace compiler

// List of supported intrisics, with upper case name, lower case name and
// expected number of arguments (-1 denoting argument count is variable).
#define INTRINSICS_LIST(V)           \
  V(Call, call, -1)                  \
  V(IsArray, is_array, 1)            \
  V(IsJSProxy, is_js_proxy, 1)       \
  V(IsJSReceiver, is_js_receiver, 1) \
  V(IsRegExp, is_regexp, 1)          \
  V(IsSmi, is_smi, 1)                \
  V(IsTypedArray, is_typed_array, 1)

namespace interpreter {

class IntrinsicsHelper {
 public:
  explicit IntrinsicsHelper(InterpreterAssembler* assembler);

  compiler::Node* InvokeIntrinsic(compiler::Node* function_id,
                                  compiler::Node* context,
                                  compiler::Node* first_arg_reg,
                                  compiler::Node* arg_count);

  static bool IsSupported(Runtime::FunctionId function_id);

 private:
  enum InstanceTypeCompareMode {
    kInstanceTypeEqual,
    kInstanceTypeGreaterThanOrEqual
  };

  compiler::Node* IsInstanceType(compiler::Node* input, int type);
  compiler::Node* CompareInstanceType(compiler::Node* map, int type,
                                      InstanceTypeCompareMode mode);
  void AbortIfArgCountMismatch(int expected, compiler::Node* actual);

#define DECLARE_INTRINSIC_HELPER(name, lower_case, count)                \
  compiler::Node* name(compiler::Node* input, compiler::Node* arg_count, \
                       compiler::Node* context);
  INTRINSICS_LIST(DECLARE_INTRINSIC_HELPER)
#undef DECLARE_INTRINSIC_HELPER

  InterpreterAssembler* assembler_;

  DISALLOW_COPY_AND_ASSIGN(IntrinsicsHelper);
};

}  // namespace interpreter
}  // namespace internal
}  // namespace v8

#endif
