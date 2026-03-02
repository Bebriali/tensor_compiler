#include <gtest/gtest.h>
#include <cstdlib>
#include <filesystem>
#include "parser.hpp"

// This test runs a Python helper to generate an ONNX model and then
// verifies that the project's ONNX parser can load it.

TEST(OnnxE2E, GenerateAndParseModel) {
    namespace fs = std::filesystem;

    const char* py = "python"; // assume `python` is on PATH

    // Check that python can import onnx; if not, skip this test.
    int rc_check = std::system((std::string(py) + " -c \"import onnx\" >nul 2>&1").c_str());
    if (rc_check != 0) {
        GTEST_SKIP() << "Skipping: Python 'onnx' package not available. Install with: pip install onnx";
    }

    // Find the generator script by walking upwards from the current working directory.
    fs::path script_rel = fs::path("tests") / "e2e" / "make_test_onnx.py";
    fs::path script_path;
    fs::path cur = fs::current_path();
    bool found = false;
    for (int i = 0; i < 6; ++i) {
        fs::path cand = cur / script_rel;
        if (fs::exists(cand)) { script_path = cand; found = true; break; }
        if (cur.has_parent_path()) cur = cur.parent_path(); else break;
    }
    ASSERT_TRUE(found) << "Could not find generator script: tests/e2e/make_test_onnx.py";

    // Output path will be inside current working directory (build dir)
    fs::path out = fs::current_path() / "generated_model.onnx";
    if (fs::exists(out)) fs::remove(out);

    std::string cmd = std::string(py) + " " + script_path.string() + " " + out.string();
    int rc = std::system(cmd.c_str());

    ASSERT_EQ(rc, 0) << "Python generator failed (rc=" << rc << ")";
    ASSERT_TRUE(fs::exists(out)) << "Generated ONNX file not found: " << out.string();

    OnnxParser parser;
    auto tree = parser.Parse(out.string());

    ASSERT_NE(tree, nullptr);
    EXPECT_GE(tree->GetSize(), 1);
}
