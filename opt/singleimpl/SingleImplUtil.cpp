/**
 * Copyright (c) 2016-present, Facebook, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 */

#include "SingleImplUtil.h"

#include "DexUtil.h"

DexType* get_concrete_type(SingleImpls& single_impls, DexType* type) {
  DexType* lookup_type = type;
  uint32_t array_level = get_array_level(type);
  if (array_level > 0) {
    auto array_type = get_array_type(type);
    assert(array_type);
    lookup_type = array_type;
  }
  const auto& intf_data = single_impls.find(lookup_type);
  if (intf_data != single_impls.end()) {
    auto concrete = intf_data->second.cls;
    if (array_level == 0) {
      return concrete;
    }
    const auto base_name = concrete->get_name()->c_str();
    uint32_t size = array_level + strlen(base_name);
    char array_name[size];
    char* p = array_name;
    while (array_level--)
      *p++ = '[';
    strcpy(p, concrete->get_name()->c_str());
    auto array_type = DexType::get_type(array_name, size);
    return array_type;
  }
  return nullptr;
}
