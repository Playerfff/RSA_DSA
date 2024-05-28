import numpy as np

# 定义参数
p = 23  # 素数
g = 5   # 基
y = 8   # 目标值
n = int(np.ceil(np.log2(p)))  # 所需量子比特数
print(f"离散对数问题: 求x使得g^x ≡ y (mod p), 其中g={g}, y={y}, p={p}")

# 初始化量子态, 创建一个均匀叠加态，即所有态的幅度相等。
state = np.ones(2 ** n) / np.sqrt(2 ** n)  # 均匀叠加态


# 构建Oracle
def oracle(state, p, g, y, n):
    for x in range(2 ** n):
        if pow(g, x, p) == y:
            state[x] = -state[x]  # oracle翻转目标态的相位，而其他态保持不变
    return state


# 扩散算子, 放大目标态的变化
def diffusion_operator(state, n):
    mean = np.mean(state)
    state = 2 * mean - state
    return state


# Grover迭代次数
num_iterations = int(np.pi / 4 * np.sqrt(2 ** n))

# 应用Grover迭代
for _ in range(num_iterations):
    state = oracle(state, p, g, y, n)
    state = diffusion_operator(state, n)


# 测量并解码结果
def measure(state):
    probabilities = np.abs(state) ** 2
    most_likely_index = np.argmax(probabilities)
    return most_likely_index


result = measure(state)
print(f'Found x: {result}, such that {g}^{result} ≡ {y} (mod {p})')
