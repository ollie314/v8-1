// Copyright 2016 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef V8_WASM_S_EXPR_H_
#define V8_WASM_S_EXPR_H_

#include <cstdint>
#include <ostream>
#include <tuple>
#include <vector>

namespace v8 {
namespace internal {

namespace wasm {

// Forward declaration.
struct WasmModule;

// Generate disassembly according to official text format.
// Output disassembly to the given output stream, and optionally return an
// offset table of <byte offset, line, column> via the given pointer.
void PrintWasmText(const WasmModule *module, uint32_t func_index,
                   std::ostream &os,
                   std::vector<std::tuple<uint32_t, int, int>> *offset_table);

}  // namespace wasm
}  // namespace internal
}  // namespace v8

#endif  // V8_WASM_S_EXPR_H_
