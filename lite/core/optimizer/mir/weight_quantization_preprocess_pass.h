// Copyright (c) 2019 PaddlePaddle Authors. All Rights Reserved.
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
#include "lite/core/op_registry.h"
#include "lite/core/optimizer/mir/pass.h"
#include "lite/core/target_wrapper.h"

namespace paddle {
namespace lite {
namespace mir {
/*
 * If the model is quantized by WeightQuantization in PostTrainingQuantization,
 * the data type of the weight in quantized ops (conv2d, depthwise_conv2d) is
 * int, and the scale is save in the quantized ops.
 * For abs_max method in WeightQuantization, WeightQuantizationPreprocessPass
 * obtains the scale value of conv2d, depthwise_conv2d and mul, expands the
 * scale list, and save the list in the quantized ops.
 */
class WeightQuantizationPreprocessPass : public ProgramPass {
 public:
  void Apply(const std::unique_ptr<SSAGraph>& graph) override;
};

}  // namespace mir
}  // namespace lite
}  // namespace paddle
