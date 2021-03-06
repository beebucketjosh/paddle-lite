/* Copyright (c) 2018 paddlepaddle Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License. */

#include "lite/core/op_registry.h"

namespace paddle {
namespace lite {
namespace x86 {
namespace math {

void fill_bias_act(float* tensor,
                   const float* bias,
                   int channel,
                   int channel_size,
                   bool flag_bias,
                   const operators::ActivationParam* act_param);

}  // namespace math
}  // namespace x86
}  // namespace lite
}  // namespace paddle
