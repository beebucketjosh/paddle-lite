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

#include "lite/core/optimizer/mir/fusion/match_matrix_activation_fuse_pass.h"

#include <memory>
#include <vector>

#include "lite/core/optimizer/mir/fusion/match_matrix_activation_fuser.h"
#include "lite/core/optimizer/mir/pass_registry.h"

namespace paddle {
namespace lite {
namespace mir {

void MatchMatrixActFusePass::Apply(const std::unique_ptr<SSAGraph>& graph) {
  fusion::MatchMatrixActFuser fuser("relu");
  fuser(graph.get());
}

}  // namespace mir
}  // namespace lite
}  // namespace paddle

REGISTER_MIR_PASS(lite_match_matrix_activation_fuse_pass,
                  paddle::lite::mir::MatchMatrixActFusePass)
    .BindTargets({TARGET(kCUDA)});
