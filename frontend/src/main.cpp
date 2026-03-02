#include <iostream>
#include <google/protobuf/stubs/common.h>
#include <onnx/onnx_pb.h>
// #include <fstream>

#include "tree/tree.hpp"
#include "parse_onnx/parser.hpp"
#include "dump/dump.hpp"

int main()
{
    std::cout << "Protobuf version: " << GOOGLE_PROTOBUF_VERSION << std::endl;

    onnx::ModelProto model;
    std::cout << "ONNX object created successfully!" << std::endl;

    OnnxParser parser;
    auto tree = parser.Parse("models/onnx/model3.onnx");


    OnnxDumper dumper;
    // std::ofstream dump_file;
    // dump_file.open("../models/dump.dot");

    // dumper.DumpConsole(*tree);
    dumper.DumpGraphViz(*tree, "dump/model3.dot");

    // if (dump_file.is_open())
    //     dump_file.close();

    return 0;
}
