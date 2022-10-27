// Copyright 2022, NVIDIA CORPORATION & AFFILIATES. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//  * Neither the name of NVIDIA CORPORATION nor the names of its
//    contributors may be used to endorse or promote products derived
//    from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ``AS IS'' AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
// OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#pragma once

#include "data_loader.h"

namespace triton { namespace perfanalyzer {

class MockDataLoader : public DataLoader {
 public:
  cb::Error GetInputData(
      const ModelTensor& input, const int stream_id, const int step_id,
      const uint8_t** data_ptr, size_t* batch1_size) override
  {
    cb::Error result{DataLoader::GetInputData(
        input, stream_id, step_id, data_ptr, batch1_size)};
    if (*data_ptr != nullptr) {
      recorded_inputs_.push_back(*reinterpret_cast<const uint32_t*>(*data_ptr));
    }
    return result;
  }

  cb::Error ReadDataFromStr(
      const std::string& str, const std::shared_ptr<ModelTensorMap>& inputs,
      const std::shared_ptr<ModelTensorMap>& outputs)
  {
    rapidjson::Document d{};
    const unsigned int parseFlags = rapidjson::kParseNanAndInfFlag;
    d.Parse<parseFlags>(str.c_str());

    return ParseData(d, inputs, outputs);
  };

  std::vector<uint32_t> recorded_inputs_{};
};

}}  // namespace triton::perfanalyzer
