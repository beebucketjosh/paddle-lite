// Copyright (c) 2021 PaddlePaddle Authors. All Rights Reserved.
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

namespace nnadapter {
namespace operation {

#define ADAPTIVE_POOL_2D_OPERATION_EXTRACT_INPUTS_OUTPUTS                     \
  auto& input_operands = operation->input_operands;                           \
  auto& output_operands = operation->output_operands;                         \
  auto input_count = input_operands.size();                                   \
  auto output_count = output_operands.size();                                 \
  auto operation_type = operation->type;                                      \
  if (operation_type == NNADAPTER_ADAPTIVE_AVERAGE_POOL_2D) {                 \
    NNADAPTER_CHECK_EQ(input_count, 2);                                       \
    NNADAPTER_CHECK_EQ(output_count, 1);                                      \
  } else if (operation_type == NNADAPTER_ADAPTIVE_MAX_POOL_2D) {              \
    NNADAPTER_CHECK_EQ(input_count, 4);                                       \
    NNADAPTER_CHECK_EQ(output_count, 2);                                      \
  } else {                                                                    \
    NNADAPTER_LOG(FATAL) << "Unsupported pooling operation type "             \
                         << OperationTypeToString(operation->type)            \
                         << " is found.";                                     \
  }                                                                           \
  /* Input */                                                                 \
  auto input_operand = input_operands[0];                                     \
  NNADAPTER_VLOG(5) << "input: " << OperandToString(input_operand);           \
  /* Output size */                                                           \
  auto kernel_buffer = reinterpret_cast<int32_t*>(input_operands[1]->buffer); \
  auto kernel_height = kernel_buffer[0];                                      \
  auto kernel_width = kernel_buffer[1];                                       \
  NNADAPTER_VLOG(5) << "filter=[" << kernel_height << "," << kernel_width     \
                    << "]";                                                   \
  /* Output */                                                                \
  auto output_operand = output_operands[0];                                   \
  NNADAPTER_VLOG(5) << "output: " << OperandToString(output_operand);

}  // namespace operation
}  // namespace nnadapter
