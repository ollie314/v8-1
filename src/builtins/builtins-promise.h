// Copyright 2016 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "src/code-stub-assembler.h"

namespace v8 {
namespace internal {

typedef compiler::Node Node;
typedef CodeStubAssembler::ParameterMode ParameterMode;
typedef compiler::CodeAssemblerState CodeAssemblerState;

class PromiseBuiltinsAssembler : public CodeStubAssembler {
 public:
  explicit PromiseBuiltinsAssembler(CodeAssemblerState* state)
      : CodeStubAssembler(state) {}

  Node* AllocateAndInitPromise(Node* context, Node* parent);

  Node* ThrowIfNotJSReceiver(Node* context, Node* value,
                             MessageTemplate::Template msg_template);

  Node* SpeciesConstructor(Node* context, Node* object,
                           Node* default_constructor);

  Node* PromiseHasHandler(Node* promise);

  void PromiseSetHasHandler(Node* promise);

  void AppendPromiseCallback(int offset, compiler::Node* promise,
                             compiler::Node* value);

  Node* InternalPromiseThen(Node* context, Node* promise, Node* on_resolve,
                            Node* on_reject);

  Node* InternalPerformPromiseThen(Node* context, Node* promise,
                                   Node* on_resolve, Node* on_reject,
                                   Node* deferred_promise,
                                   Node* deferred_on_resolve,
                                   Node* deferred_on_reject);

  void InternalResolvePromise(Node* context, Node* promise, Node* result);

  void BranchIfFastPath(Node* context, Node* promise, Label* if_isunmodified,
                        Label* if_ismodified);

  Node* CreatePromiseContext(Node* native_context, int slots);
  Node* CreatePromiseResolvingFunctionsContext(Node* promise, Node* debug_event,
                                               Node* native_context);

  std::pair<Node*, Node*> CreatePromiseResolvingFunctions(
      Node* promise, Node* native_context, Node* promise_context);

  Node* CreatePromiseGetCapabilitiesExecutorContext(Node* native_context,
                                                    Node* promise_capability);

  Node* NewPromiseCapability(Node* context, Node* constructor,
                             Node* debug_event = nullptr);
};

}  // namespace internal
}  // namespace v8
