#pragma once

#include <string>
#include <memory>
#include <unordered_map>
#include <onnx/onnx_pb.h>
#include "tree.hpp"

class OnnxParser
{
public:
    OnnxParser() = default;

    std::unique_ptr<Tree_t> Parse(const std::string& file_path);

private:
    bool LoadModelProto(const std::string& file_path, onnx::ModelProto& model) const ;
};
