// Copyright 2016 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Flags: --expose-wasm  --stress-gc --expose-gc

load("test/mjsunit/wasm/wasm-constants.js");
load("test/mjsunit/wasm/wasm-module-builder.js");

function run(f) {
  // wrap the creation in a closure so that the only thing returned is
  // the module (i.e. the underlying array buffer of WASM wire bytes dies).
  var module = (() => {
    var builder = new WasmModuleBuilder();
    builder.addImport("the_name_of_my_import", kSig_i_i);
    builder.addFunction("main", kSig_i_i)
      .addBody([
        kExprGetLocal, 0,
        kExprCallFunction, 0])
      .exportAs("main");
    print("module");
    return new WebAssembly.Module(builder.toBuffer());
  })();

  gc();
  for (var i = 0; i < 10; i++) {
    print("  instance " + i);
    var instance = new WebAssembly.Instance(module, {the_name_of_my_import: f});
    var g = instance.exports.main;
    assertEquals("function", typeof g);
    for (var j = 0; j < 10; j++) {
      assertEquals(f(j), g(j));
    }
  }
}

(function test() {
  for (var i = 0; i < 3; i++) {
    run(x => (x + 19));
    run(x => (x - 18));
  }
})();
