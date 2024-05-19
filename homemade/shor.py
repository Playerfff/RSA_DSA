import numpy as np
import math
from fractions import Fraction
from numpy.linalg import norm

def gcd(a, b):
    while b != 0:
        a, b = b, a % b
    return a

def quantum_order_finding(a, N):
    # 量子部分省略，假设能找到周期 r
    # 在实际量子计算中，这一步需要用量子计算机实现
    r = find_period_quantumly(a, N)
    return r

def shor_algorithm(N):
    # Step 1: Check if N is even
    if N % 2 == 0:
        return 2

    # Step 2: Find a random integer a such that 1 < a < N and gcd(a, N) == 1
    a = np.random.randint(2, N)
    while gcd(a, N) != 1:
        a = np.random.randint(2, N)

    # Step 3: Find the order r of a modulo N
    r = quantum_order_finding(a, N)

    # Step 4: If r is odd or a^(r/2) ≡ -1 (mod N), retry
    if r % 2 != 0 and pow(a, r // 2, N) != N - 1:
        x = pow(a, r // 2, N)
        factor1 = gcd(x - 1, N)
        factor2 = gcd(x + 1, N)
        if factor1 != 1 and factor1 != N:
            return factor1
        elif factor2 != 1 and factor2 != N:
            return factor2

    return None  # Retry if no factor is found

# 示例使用
N = 15  # 要分解的整数
factor = shor_algorithm(N)
print("Factor found:", factor)
