import sys

def factorial(n):
    if n % 50 == 0:
        print(f"Calculando factorial({n})")

    dummy = sum(i * 0.000001 for i in range(500000))

    if n == 0 or n == 1:
        return 1
    return n * factorial(n - 1)

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print(f"Uso: python3 {sys.argv[0]} <numero>")
        sys.exit(1)
    n = int(sys.argv[1])
    sys.setrecursionlimit(10000)
    print(f"Iniciando calculo de {n}!")
    resultado = factorial(n)
    print(f"Terminado: {n}!")
