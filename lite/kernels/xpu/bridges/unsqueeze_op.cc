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

int UnsqueezeConverter(void* ctx, OpLite* op, KernelBase* kernel) {
  CHECK(ctx != nullptr);
  CHECK(op != nullptr);
  auto graph = static_cast<Graph*>(ctx);
  auto op_info = op->op_info();
  auto op_type = op_info->Type();
  auto scope = op->scope();
  VLOG(3) << "[XPU] Converting " << op_type << "... ";

  auto x_name = op_info->Input("X").front();
  auto x = scope->FindMutableTensor(x_name);
  auto axes_v = op_info->GetAttr<std::vector<int>>("axes");

  auto out_name = op_info->Output("Out").front();

  // X node
  std::shared_ptr<Node> x_node = nullptr;
  if (graph->Has(x_name)) {
    x_node = graph->Get(x_name);
  } else {
    x_node = graph->Add(x_name, *x);
  }

  auto expend_dims_node = graph->Add(
      out_name,
      graph->builder_.CreateExpandDims(*x_node->data(), axes_v[0], 1));
  for (size_t i = 1; i < axes_v.size(); i++)
    expend_dims_node = graph->Add(out_name,
                                  graph->builder_.CreateExpandDims(
                                      *expend_dims_node->data(), axes_v[i], 1));

  return REBUILD_WHEN_SHAPE_CHANGED;
}

}  // namespace xpu
}  // namespace subgraph
}  // namespace lite
}  // namespace paddle

REGISTER_SUBGRAPH_BRIDGE(unsqueeze2,
                         kXPU,
                         paddle::lite::subgraph::xpu::UnsqueezeConverter);
