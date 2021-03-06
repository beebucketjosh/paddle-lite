/* Copyright (c) 2018 PaddlePaddle Authors. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include "lite/backends/x86/jit/gen_base.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
// #include "paddle/fluid/memory/allocation/cpu_allocator.h"  // for
// posix_memalign
#include "lite/backends/x86/cpu_info.h"
#include "lite/backends/x86/jit/macro.h"
#include "lite/utils/env.h"
#include "lite/utils/log/cp_logging.h"

#ifndef _WIN32
#define posix_memalign_free free
#endif

#ifdef _WIN32
#define posix_memalign_free _aligned_free
#define posix_memalign(p, a, s) \
  (((*(p)) = _aligned_malloc((s), (a))), *(p) ? 0 : errno)
#endif

// DEFINE_bool(dump_jitcode, false, "Whether to dump the jitcode to file");
bool dump_jitcode = paddle::lite::GetBoolFromEnv("dump_jitcode");

namespace paddle {
namespace lite {
namespace jit {

// refer do not need CanBeUsed, it would be the last one.
void GenBase::dumpCode(const unsigned char* code) const {
  if (code) {
    static int counter = 0;
    std::ostringstream filename;
    filename << "paddle_jitcode_" << name() << "." << counter << ".bin";
    counter++;
    std::ofstream fout(filename.str(), std::ios::out);
    if (fout.is_open()) {
      fout.write(reinterpret_cast<const char*>(code), this->getSize());
      fout.close();
    }
  }
}

void* GenBase::operator new(size_t size) {
  void* ptr;
  constexpr size_t alignment = 32ul;
#ifdef _WIN32
  ptr = _aligned_malloc(size, alignment);
#else
  CHECK_EQ(posix_memalign(&ptr, alignment, size), 0) << "GenBase Alloc " << size
                                                     << " error!";
#endif
  CHECK(ptr) << "Fail to allocate GenBase CPU memory: size = " << size;
  return ptr;
}

void GenBase::operator delete(void* ptr) { posix_memalign_free(ptr); }

std::vector<int> packed_groups(int n, int k, int* block_out, int* rest_out) {
  int block;
  int max_num_regs;
  if (x86::MayIUse(x86::avx512f)) {
    block = ZMM_FLOAT_BLOCK;
    max_num_regs = 32;
  } else {
    block = YMM_FLOAT_BLOCK;
    max_num_regs = 16;
  }
  // one for x, one for y, others for z
  const int max_used_regs_for_n = max_num_regs - 2;
  const int aligned_n = n % block == 0 ? n : (n / block + 1) * block;
  const int num_block = aligned_n / block;
  const int num_groups = num_block / max_used_regs_for_n;
  std::vector<int> groups(num_groups, max_used_regs_for_n);
  int rest_num_regs = num_block % max_used_regs_for_n;
  if (rest_num_regs != 0) {
    groups.push_back(rest_num_regs);
  }
  if (block_out) {
    *block_out = block;
  }
  if (rest_out) {
    *rest_out = n % block;
  }
  return groups;
}

}  // namespace jit
}  // namespace lite
}  // namespace paddle
