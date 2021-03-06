// Copyright (c) 2020 PaddlePaddle Authors. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include <memory>
#include "lite/core/kernel.h"
#include "lite/core/op_registry.h"
#include "lite/kernels/nnadapter/engine.h"

namespace paddle {
namespace lite {
namespace kernels {
namespace nnadapter {

class SubgraphCompute : public KernelLite<TARGET(kNNAdapter),
                                          PRECISION(kAny),
                                          DATALAYOUT(kNCHW)> {
 public:
  using param_t = operators::SubgraphParam;

  void PrepareForRun() override;

  void Run() override;

  virtual ~SubgraphCompute() = default;

 private:
  std::unique_ptr<Engine> engine_;
};

}  // namespace nnadapter
}  // namespace kernels
}  // namespace lite
}  // namespace paddle
