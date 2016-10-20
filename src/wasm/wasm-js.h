// Copyright 2015 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef V8_WASM_JS_H_
#define V8_WASM_JS_H_

#include "src/allocation.h"
#include "src/base/hashmap.h"

namespace v8 {
namespace internal {
// Exposes a WASM API to JavaScript through the V8 API.
class WasmJs {
 public:
  static void Install(Isolate* isolate, Handle<JSGlobalObject> global_object);

  V8_EXPORT_PRIVATE static void InstallWasmModuleSymbolIfNeeded(
      Isolate* isolate, Handle<JSGlobalObject> global, Handle<Context> context);

  V8_EXPORT_PRIVATE static void InstallWasmMapsIfNeeded(
      Isolate* isolate, Handle<Context> context);
  static void InstallWasmConstructors(Isolate* isolate,
                                      Handle<JSGlobalObject> global,
                                      Handle<Context> context);

  static Handle<JSObject> CreateWasmMemoryObject(Isolate* isolate,
                                                 Handle<JSArrayBuffer> buffer,
                                                 bool has_maximum, int maximum);

  static bool IsWasmMemoryObject(Isolate* isolate, Handle<Object> value);

  static Handle<JSArrayBuffer> GetWasmMemoryArrayBuffer(Isolate* isolate,
                                                        Handle<Object> value);

  static uint32_t GetWasmMemoryMaximumSize(Isolate* isolate,
                                           Handle<Object> value);

  static void SetWasmMemoryInstance(Isolate* isolate, Handle<Object> value,
                                    Handle<JSObject> instance);
};

}  // namespace internal
}  // namespace v8
#endif
