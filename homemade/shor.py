import sympy
import random
import math


def gcd(a, b):
    """
    计算两个数的最大公约数 (Greatest Common Divisor, GCD)。
    使用欧几里得算法来找到GCD。
    """
    while b != 0:
        a, b = b, a % b
    return a


def modular_exponentiation(base, exponent, modulus):
    """
    执行模幂运算 (base^exponent % modulus)。
    使用快速幂算法来有效计算大整数的幂。
    """
    result = 1
    base = base % modulus  # 将base取模
    while exponent > 0:
        if (exponent % 2) == 1:  # 如果指数是奇数
            result = (result * base) % modulus  # 更新结果并取模
        exponent = exponent >> 1  # 将指数右移一位（整除2）
        base = (base * base) % modulus  # base平方后取模
    return result  # 返回计算结果


def find_factor_of_prime_power(n):
    """
    检查n是否是某个质数的幂，如果是，则返回该质数。
    """
    for k in range(2, math.floor(math.log2(n)) + 1):  # k从2到log2(n)的最大整数
        c = math.pow(n, 1 / k)  # 计算n的k次根
        c1 = math.floor(c)
        if c1 ** k == n:
            return c1  # 返回c1作为因子
        c2 = math.ceil(c)
        if c2 ** k == n:
            return c2
    return None  # 如果没有找到，返回None


def quantum_order_finder(x, n):
    """
    量子模拟部分：计算使 x^r ≡ 1 (mod n) 的最小正整数 r。
    这里是一个模拟，没有实际的量子计算。
    """
    for r in range(1, n):  # 尝试所有可能的r从1到n-1
        if modular_exponentiation(x, r, n) == 1:  # 如果x^r % n == 1
            return r  # 返回r作为周期
    return None  # 如果没有找到，返回None


def find_factor(n, max_attempts=30):
    """
    尝试找到给定合数 n 的非平凡因子。

    参数:
    - n: 需要因式分解的整数
    - max_attempts: 最大尝试次数,在 find_factor 函数中，多次尝试的目的是为了增加成功找到非平凡因子的概率。
                    Shor's算法本质上是一个概率性算法，成功找到因子的概率并不是100%，所以需要多次尝试来提高成功的几率。
    """
    if sympy.isprime(n):
        return None
    if n % 2 == 0:
        return 2
    c = find_factor_of_prime_power(n)
    if c is not None:
        return c
    for _ in range(max_attempts):
        x = random.randint(2, n - 1)
        c = gcd(x, n)
        if 1 < c < n:
            return c
        r = quantum_order_finder(x, n)  # 找到n的周期r
        if r is None or r % 2 != 0:
            continue
        y = modular_exponentiation(x, r // 2, n)
        if y == n - 1:
            continue
        c = gcd(y - 1, n)
        if 1 < c < n:
            return c
    return None


def main(n):
    if n < 2:
        raise ValueError(f"Invalid input {n}, expected positive integer greater than one.")
    d = find_factor(n)
    if d is None:
        print(f"No non-trivial factor of {n} found. It is probably a prime.")
    else:
        print(f"{d} is a non-trivial factor of {n}")


if __name__ == '__main__':
    n = int(input("Enter a composite number to factorize: "))
    main(n)
