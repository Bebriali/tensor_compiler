#!/usr/bin/env python3
import sys
from pathlib import Path

try:
    import onnx
    from onnx import helper, TensorProto
except Exception as e:
    print("Missing dependency 'onnx'. Install with: pip install onnx", file=sys.stderr)
    raise

if len(sys.argv) < 2:
    print("Usage: make_test_onnx.py <output_path>", file=sys.stderr)
    sys.exit(2)

out_path = Path(sys.argv[1])

# Simple Gemm node: Y = A*B + C
node = helper.make_node(
    'Gemm',
    inputs=['A', 'B', 'C'],
    outputs=['Y'],
    name='SimpleGemm'
)

# Value infos
A = helper.make_tensor_value_info('A', TensorProto.FLOAT, [1, 2])
B = helper.make_tensor_value_info('B', TensorProto.FLOAT, [2, 2])
C = helper.make_tensor_value_info('C', TensorProto.FLOAT, [1, 2])
Y = helper.make_tensor_value_info('Y', TensorProto.FLOAT, [1, 2])

# Initializers (small tensors)
import numpy as np
init_B = helper.make_tensor('B', TensorProto.FLOAT, [2,2], np.array([[1,0],[0,1]], dtype=np.float32).flatten().tolist())
init_C = helper.make_tensor('C', TensorProto.FLOAT, [1,2], np.array([0,0], dtype=np.float32).tolist())

graph = helper.make_graph(
    [node],
    'TestGraph',
    [A],
    [Y],
    initializer=[init_B, init_C]
)

model = helper.make_model(graph, producer_name='make_test_onnx')

out_path.parent.mkdir(parents=True, exist_ok=True)
onnx.save(model, str(out_path))
print("Saved test ONNX model to", out_path)
