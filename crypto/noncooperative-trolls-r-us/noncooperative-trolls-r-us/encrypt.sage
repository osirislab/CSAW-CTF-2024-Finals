def gen_poly(d):
    res = N*[0]
    for j in range(d):  
        r = randint(0,N-1)
        while res[r] != 0:
            r = randint(0,N-1)
        res[r] = randint(-1,1)
    return Zx(res)
    
def gen_msg(d):
    return Z(list(randint(0,d-1)-2 for j in range(N)))

def encrypt(m, h, d): 
    r = gen_poly(d)
    return rangedmod(polymult(h,p*r) + m,q)

def generate_keys(d, p, q):
    while True:
        try:
            f = gen_poly(d)
            g = gen_poly(d)
            f_p = finvmodp(f,p)
            f_q = finvmodp(f,q)  
            secret_key = f, f_p
            h = rangedmod(p * polymult(f_q,g),q)
            break
        except:
            print("bad f and/or g, try again")
    
    return h,secret_key
