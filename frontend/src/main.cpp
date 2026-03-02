#include <iostream>
#include <filesystem>

#include <google/protobuf/stubs/common.h>
#include <onnx/onnx_pb.h>

#include "tree/tree.hpp"
#include "parse_onnx/parser.hpp"
#include "dump/dump.hpp"

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <model_name.onnx>" << std::endl;
        return 1;
    }

    const std::string base_path = "models/onnx/";   // хардкод пути
    std::string file_name = argv[1];

    std::string full_path = base_path + file_name;

    if (!std::filesystem::exists(full_path))
    {
        std::cerr << "Model not found: " << full_path << std::endl;
        return 1;
    }

    std::cout << "Loading model: " << full_path << std::endl;
    std::cout << "Protobuf version: " << GOOGLE_PROTOBUF_VERSION << std::endl;

    OnnxParser parser;
    auto tree = parser.Parse(full_path);

    if (!tree)
    {
        std::cerr << "Failed to parse model." << std::endl;
        return 1;
    }

    // автоматически формируем имя .dot
    std::filesystem::path p(file_name);
    std::string output_dot = "dump/" + p.stem().string() + ".dot";

    OnnxDumper dumper;
    dumper.DumpGraphViz(*tree, output_dot);

    std::cout << "Graph saved to: " << output_dot << std::endl;

    return 0;
}
