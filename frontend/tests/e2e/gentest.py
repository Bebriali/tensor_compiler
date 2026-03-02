import torch
import torch.nn as nn

# Создаем очень глубокую линейную сеть
class BigModel(nn.Module):
    def __init__(self, layers=50):
        super().__init__()
        self.net = nn.Sequential(*[
            nn.Sequential(nn.Linear(1024, 1024), nn.ReLU())
            for _ in range(layers)
        ])
    def forward(self, x):
        return self.net(x)

model = BigModel(layers=100) # 100 слоев!
dummy_input = torch.randn(1, 1024)
torch.onnx.export(model, dummy_input, "huge_model.onnx")
