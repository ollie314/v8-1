// Copyright 2016 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "src/snapshot/serializer-common.h"

#include "src/external-reference-table.h"
#include "src/ic/stub-cache.h"
#include "src/list-inl.h"

#if defined(DEBUG) && defined(V8_OS_LINUX) && !defined(V8_OS_ANDROID)
#define SYMBOLIZE_FUNCTION
#include <execinfo.h>
#endif  // DEBUG && V8_OS_LINUX && !V8_OS_ANDROID

namespace v8 {
namespace internal {

ExternalReferenceEncoder::ExternalReferenceEncoder(Isolate* isolate) {
  map_ = isolate->external_reference_map();
  if (map_ != NULL) return;
  map_ = new base::HashMap();
  ExternalReferenceTable* table = ExternalReferenceTable::instance(isolate);
  for (int i = 0; i < table->size(); ++i) {
    Address addr = table->address(i);
    if (addr == ExternalReferenceTable::NotAvailable()) continue;
    // We expect no duplicate external references entries in the table.
    // AccessorRefTable getter may have duplicates, indicated by an empty string
    // as name.
    DCHECK(table->name(i)[0] == '\0' ||
           map_->Lookup(addr, Hash(addr)) == nullptr);
    map_->LookupOrInsert(addr, Hash(addr))->value = reinterpret_cast<void*>(i);
  }
  isolate->set_external_reference_map(map_);
}

uint32_t ExternalReferenceEncoder::Encode(Address address) const {
  DCHECK_NOT_NULL(address);
  base::HashMap::Entry* entry =
      const_cast<base::HashMap*>(map_)->Lookup(address, Hash(address));
  if (entry == nullptr) {
    void* function_addr = address;
    v8::base::OS::PrintError("Unknown external reference %p.\n", function_addr);
#ifdef SYMBOLIZE_FUNCTION
    v8::base::OS::PrintError("%s\n", backtrace_symbols(&function_addr, 1)[0]);
#endif  // SYMBOLIZE_FUNCTION
    v8::base::OS::Abort();
  }
  return static_cast<uint32_t>(reinterpret_cast<intptr_t>(entry->value));
}

const char* ExternalReferenceEncoder::NameOfAddress(Isolate* isolate,
                                                    Address address) const {
  base::HashMap::Entry* entry =
      const_cast<base::HashMap*>(map_)->Lookup(address, Hash(address));
  if (entry == NULL) return "<unknown>";
  uint32_t i = static_cast<uint32_t>(reinterpret_cast<intptr_t>(entry->value));
  return ExternalReferenceTable::instance(isolate)->name(i);
}

void SerializedData::AllocateData(int size) {
  DCHECK(!owns_data_);
  data_ = NewArray<byte>(size);
  size_ = size;
  owns_data_ = true;
  DCHECK(IsAligned(reinterpret_cast<intptr_t>(data_), kPointerAlignment));
}

// The partial snapshot cache is terminated by undefined. We visit the
// partial snapshot...
//  - during deserialization to populate it.
//  - during normal GC to keep its content alive.
//  - not during serialization. The partial serializer adds to it explicitly.
DISABLE_CFI_PERF
void SerializerDeserializer::Iterate(Isolate* isolate, ObjectVisitor* visitor) {
  List<Object*>* cache = isolate->partial_snapshot_cache();
  for (int i = 0;; ++i) {
    // Extend the array ready to get a value when deserializing.
    if (cache->length() <= i) cache->Add(Smi::kZero);
    // During deserialization, the visitor populates the partial snapshot cache
    // and eventually terminates the cache with undefined.
    visitor->VisitPointer(&cache->at(i));
    if (cache->at(i)->IsUndefined(isolate)) break;
  }
}

bool SerializerDeserializer::CanBeDeferred(HeapObject* o) {
  return !o->IsString() && !o->IsScript();
}

}  // namespace internal
}  // namespace v8
