/* Copyright (c) 2016 PaddlePaddle Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License. */

#pragma once
#include <limits>
#include "lite/backends/x86/fluid/eigen.h"
#include "lite/core/context.h"
#include "lite/core/tensor.h"
#include "lite/utils/macros.h"

namespace paddle {
namespace lite {
namespace x86 {
namespace math {

template <typename T>
struct TolerableValue {
  HOSTDEVICE T operator()(const T& x) const {
    CHECK(static_cast<bool>(std::is_floating_point<T>::value));
    const T kApproInf = 1e20;

    if (x == INFINITY) return kApproInf;
    if (x == -INFINITY) return -kApproInf;
    return x;
  }
};

// NOTE(dzh): float16 value clip behave different.
// 1. Our ValueClipping has a  hardcore threshold 1e20
// for float number. 1e20 will resulting in overflow in float16.
// 2. float16 should expose the the real number overflow to python.
// because mixed-training depends the inf/nan value to determine
// if the scale value will be adjusted.
// Also. In standard implementation of cross entropy, other
// framework not has the ValueClipping.
template <>
struct TolerableValue<lite::fluid::float16> {
  HOSTDEVICE lite::fluid::float16 operator()(
      const lite::fluid::float16& x) const {
    if (lite::fluid::isfinite(x))
      return x;
    else if (x > static_cast<lite::fluid::float16>(0))
      return std::numeric_limits<lite::fluid::float16>::max();
    else
      return std::numeric_limits<lite::fluid::float16>::min();
  }
};

template <lite::TargetType Target, typename T>
class CrossEntropyFunctor {
 public:
  void operator()(const lite::Context<Target>& context,
                  lite::Tensor* out,
                  const lite::Tensor* prob,
                  const lite::Tensor* labels,
                  const bool softLabel,
                  const int ignore_index,
                  const int axis_dim);
};
}  // namespace math
}  // namespace x86
}  // namespace lite
}  // namespace paddle
