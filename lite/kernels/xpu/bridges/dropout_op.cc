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

#include "lite/core/subgraph/subgraph_bridge_registry.h"
#include "lite/kernels/xpu/bridges/graph.h"
#include "lite/kernels/xpu/bridges/utility.h"

namespace paddle {
namespace lite {
namespace subgraph {
namespace xpu {

int DropoutConverter(void* ctx, OpLite* op, KernelBase* kernel) {
  CHECK(ctx != nullptr);
  CHECK(op != nullptr);
  auto graph = static_cast<Graph*>(ctx);
  auto op_info = op->op_info();
  auto op_type = op_info->Type();
  auto scope = op->scope();
  VLOG(3) << "[XPU] Converting " + op_type + "...";

  // Get input and output vars and op attributes
  auto x_name = op_info->Input("X").front();
  auto x = scope->FindMutableTensor(x_name);
  auto x_dims = x->dims();
  auto out_name = op_info->Output("Out").front();
  auto dropout_prob = op_info->GetAttr<float>("dropout_prob");
  auto dropout_implementation =
      op_info->GetAttr<std::string>("dropout_implementation");

  // X node
  std::shared_ptr<Node> x_node = nullptr;
  if (graph->Has(x_name)) {
    x_node = graph->Get(x_name);
  } else {
    x_node = graph->Add(x_name, *x);
  }

  // Dropout node
  if (dropout_implementation == "downgrade_in_infer") {
    graph->Add(out_name,
               graph->builder_.CreateScale(
                   *x_node->data(), 1.f - dropout_prob, 0.0f, false));
  } else if (dropout_implementation == "upscale_in_train") {
    graph->Add(out_name,
               graph->builder_.CreateScale(*x_node->data(), 1.0f, 0.0f, false));
  } else {
    LOG(WARNING) << "[XPU] Unsupported dropout_implementation == "
                 << dropout_implementation << " for dropout";
    return FAILED;
  }
  return SUCCESS;
}

}  // namespace xpu
}  // namespace subgraph
}  // namespace lite
}  // namespace paddle

REGISTER_SUBGRAPH_BRIDGE(dropout,
                         kXPU,
                         paddle::lite::subgraph::xpu::DropoutConverter);
