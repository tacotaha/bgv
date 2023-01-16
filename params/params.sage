# Generates parameters for a secret key with coefficients sampled from {-1, 0, 1}
# https://projects.csail.mit.edu/HEWorkshop/HomomorphicEncryptionStandard2018.pdf (sec 2.0.3)

MODE = "classical"
assert(MODE in ["classical", "quantum"])

# Polynomial degree
LGN = 14
assert(LGN in range(10, 16))

# security level
BITS = 256
assert(BITS in [128, 192, 256])

# Fixed error distribution variance
SIGMA = 8/sqrt(2*pi)

# Montgomery reduction parameter
R = 2^32

def LGQ():
    if MODE == "quantum":
        l = [(25, 17, 13), (51, 35, 27), (101, 70, 54), (202, 141, 109), (411, 284, 220), (827, 571, 443)]
    else:
        l = [(27, 19, 14), (54, 37, 29), (109, 75, 58), (218, 152, 118), (438, 305, 237), (881, 611, 476)]
    return l[LGN - 10][(BITS - 128)/64]

def Q(lbound=30, ubound=31):
    q = []
    lgq = LGQ()
    while prod(q).nbits() < lgq:
        q_i = random_prime((1 << ubound)-1, False, (1 << lbound)-1)
        # Z_qi contains nth roots of unity
        if q_i % (1<<(LGN + 1)) == 1:
            q.append(q_i)
    return q

q_i = Q()
q = prod(q_i)
R_minus_q_inv = [inverse_mod(R - i, R) for i in q_i]
R_squared_mod_q = [R^2 % i for i in q_i]

print(f"{q.nbits()}-bit modulus q = {q}")
print("RNS decomposition: ", [hex(i) for i in q_i])
print("1/(R-q) mod R: ", [hex(i) for i in R_minus_q_inv])
print("R^2 mod q: ", [hex(i) for i in R_squared_mod_q])
