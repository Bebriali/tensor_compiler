#include <gtest/gtest.h>
#include "tree.hpp"

TEST(NodeUnit, AddChildAndAttributes) {
    auto parent = std::make_shared<Node_t>("parent", "ParentOp");
    auto child = std::make_shared<Node_t>("child", "ChildOp");

    parent->AddChild(child);
    ASSERT_EQ(parent->children.size(), 1);
    EXPECT_EQ(parent->children[0]->name, "child");

    parent->setAttr("int_attr", int64_t(123));
    EXPECT_EQ(parent->attributes["int_attr"].asInt(), 123);

    parent->setAttr("flt_attr", float(3.14f));
    EXPECT_FLOAT_EQ(parent->attributes["flt_attr"].asFlt(), 3.14f);

    parent->setAttr("str_attr", std::string("hello"));
    EXPECT_EQ(parent->attributes["str_attr"].asStr(), "hello");

    parent->setAttr("vec_attr", std::vector<int64_t>{1,2,3});
    auto v = parent->attributes["vec_attr"].asVecInt();
    ASSERT_EQ(v.size(), 3);
    EXPECT_EQ(v[0], 1);
}
