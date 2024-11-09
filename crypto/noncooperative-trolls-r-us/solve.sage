from sage.all import *
from fpylll import *
import numpy as np
import binascii
import math

# NTRU

# ntru uses a ring over (x^N - 1)
# param: a and b are both polynomials
# returns: polynomial over (x^N - 1)
def polymult(a,b): return (a * b) % (x^N - 1) 

# f mod q shift range 0,q to -q/2,q/2
# param: f polynomial, q for modulus
# returns: polynomial mod q with range -q/2,q/2
def rangedmod(f,q): return Z([((fi + q//2) % q) - q//2 for fi in f]) 

# finds f_inv mod p
# param: f polynomial, p for modulus 
# returns: f_p polynomial = f inversion over mod p
def finvmodp(f,p):
    # change base
    R = Z.change_ring(Zmod(p)).quotient(x^N - 1) 
    # invert
    return Z(lift(1 / R(f)))   

# decryption derived from wikipedia/encrypt func
# param: encrypted_message and f,f_p polynomials
# returns: decrypted message polynomial 
def decrypt(encrypted_message, secret_key): 
    f,f_p = secret_key

    # a = polynomial multiplication (message, f) (mod q)
    # keep numbers -q/2, q/2 ... sage likes to rearrange to 0, q
    a = rangedmod(polymult(encrypted_message,f),q)
     
    # polynomial multiplication (a, f_p) (mod p)
    return rangedmod(polymult(a,f_p),p)

# mult matrix construction
# param: h polynomial 
# returns: M matrix (N,N) of rotated h vectors
def create_M_mat(h):
    M = Matrix(ZZ, N, N)

    for i in range(N):
        for j in range(N):
            M[i,(j+i)%N] = h[j]%q
    
    return M

# given public key and public values (N, p, q, d) find f and f_p
# param: pub (public key polynomial), N, p, q, d ints
# returns: list of possible key,message polynomial pairs
def solve(pub, N, p, q, d):
    # basis matrix construction
    #    _______________
    #   |       |       |
    #   |   I   |   M   |
    #   |_______|_______|
    #   |       |       |
    #   |   0   |  qI   |
    #   |_______|_______|  
    # where I is identity matrix rank N and M is multiplication/circulant matrix of h (rotated h vectors)
    #
    basis_matrix = Matrix(ZZ, N*2, N*2)

    M = create_M_mat(pub)

    q_idm = identity_matrix(N) * q
    idm = identity_matrix(N) # todo simplify
    for i in range(N):
        r = np.concatenate((idm[i],M[i])) 
        basis_matrix[i, :] = vector(list(r))
    for i in range(N):
        r = np.zeros(2*N)
        r[N:2*N] = q_idm[i]
        basis_matrix[i+N, :] = vector(list(r))

    # matrix reduction -> since we have relation h = g/f * p mod q we know f,g is in the vector space (rotated)
    A = IntegerMatrix.from_matrix([[int(x) for x in v] for v in basis_matrix])
    Mat = GSO.Mat(A)
    bkz = BKZ.reduction(A, o=BKZ.Param(block_size=2))
    Mat.update_gso()
    
    # https://eprint.iacr.org/2021/999.pdf
    # Vol(R)^(1/N)
    ddet = pow(Mat.get_root_det(0,-1), 2/N)
    # threshold = sqrt(q/(2pi*e))*Vol(R)^(1/N) via first minimum via gaussian heuristic
    thresh = pow(q/(2*np.pi*np.e),.5)*ddet

    # list ver of bkz reduction result
    bb = [[int(x) for x in v] for v in bkz]
    # all shortest vectors (less than threshold)
    short_vectors = [row for row in bb if np.inner(row,row) < thresh]

    # loop through short vectors and get all possible (f, decoded_message) pairs (some f may not be invertible over p)
    possible_messages = []
    for v in short_vectors:
        f = v[:N]
        g = v[N:]
        try:
            f_p = finvmodp(f,p)
            decrm = decrypt(encrypted_message, (Z(f),f_p))
            possible_messages.append((f,decrm))
        except:
            print("failed f attempt")

    return possible_messages



# post-NTRU ciphers

# helpers:

# base x to binary to get bytes
# param: number in base x, x default 5
# returns: binary representation of inputted base x number
def basex_to_binary(number, x=5):
    decimal = int(0)
    power = int(0)
    for digit in reversed(number):
        decimal += int(int(digit) * (x ^ int(power)))
        power += 1

    binary = bin(decimal)[2:]  # [2:] removes the '0b' prefix
    return binary

# front pad with 0 if binary isn't of length % 8 = 0
# param: c = string representation of binary str
# returns: front padded c 
def pad0_for_ascii(c):
    bintr = 0
    while len(c) > 8*bintr:
        bintr += 1
    d = "0"*(8*bintr - len(c)) + c
    return d

# xor two binary representation strings
# param: var (ciphertext) and key, both strings of binary
# returns: var xor key where key is looped to be same length as var
def xor_binstr(var, key, byteorder=sys.byteorder):
    # loop key to be equal length var
    while (len(key) < len(var)):
        key += key
    key = key[:len(var)]

    # != for xor
    res = [int(int(var[i]) != int(key[i])) for i in range(len(var))]

    # convert back to string 
    res_binstr = ""
    for r in res:
        res_binstr += (str(r))
    return res_binstr

# ciphertext.txt text deciphering via "misdirection"
# param: given string 
# returns: binary interpretation of string = [1 if l in "misdirection" else 0 for l in given]
def input_ascii_to_bin(given):
    # "riddle me this, batman: what's inconveniently long and full of misdirection?"
    # get binary from text: any letter in "misdirection" = 1, else 0
    bin_ciphertext = ""
    misdir = set([i for i in "misdirection"])
    for i in given:
        if i in misdir:
            bin_ciphertext += "1"
        else:
            bin_ciphertext += "0"
    # pad for ascii conversion, just in case
    bin_ciphertext = pad0_for_ascii(bin_ciphertext)
    return bin_ciphertext

# non-helpers:

# uses (_,m) to interpret bin_ciphertext into coordinate clue
# param: possible_keys (list of possible key, message polynomial pairs), bin_ciphertext (binary interpretation of ciphertext.txt)
# returns: bns = list of potential decoded messages 
def check_keys(possible_keys, bin_ciphertext):
    bns = []
    for key in possible_keys: # can just run on one if you're confident about relative shortness, i'm running on all to be sure
        f, decrm = key
        
        # use coefficients from secret, make string without negatives to interpret base 5
        decrm = decrm.list()
        secretstuff = [x%5 for x in decrm] 
        s = ""
        for i in secretstuff:
            s += str(i)
        
        # key
        byte_key = basex_to_binary(s)
        
        # xor ciphertext and key
        xord_ciph = xor_binstr(bin_ciphertext, byte_key)
        # pad 0
        xord_ciph = pad0_for_ascii(xord_ciph)
        
        # binary to ascii
        n = int(xord_ciph, 2)
        bn = binascii.unhexlify('%x' % n).decode()
        bns.append(bn)
    return bns

# takes coordinate clue and interprets into flag 
# param: b_eval = coordinate clue, giveninput = ciphertext.txt text
# returns: flag
def decipher(b_eval, giveninput):
    misdir = set([i for i in "misdirection"])
    
    # b_clue = thiscantberight+surelyitsover -> 
    #       "cant be right" = left, "surely it's over" = over -> 
    #       leftover + chal descr "so you don't waste anything" = look at ciphertext.txt again ->
    #       "misdirection" was already used 
    # b_eval = coords 
    b_clue, b_eval = b_eval.split('[') # ignores [
    b_nums = b_eval[:-1].split(',') # ignores ] at end, splits by ,
    
    # take out misdir
    leftovers = []
    for i in giveninput:
        if i not in misdir:
            leftovers.append(i)
    
    # use b_nums as indexes in the leftover alphanumeric soup, leave _ as is
    res = ""
    for n in b_nums:
        if n == "_":
            res += n
        else:
            res += leftovers[int(n)]
    # print(res)
    return res



# run everything:

# i may be the sole witness to a crime, but that doesn't mean i need to be helpful. solve my puzzle, and i'll give you a clue...
# riddle me this, batman: what's inconveniently long and full of misdirection?
# please remember to wrap up your food when you're done so you don't waste anything. csawctf{flag}

# global given:
# keys.txt
N = 55
p = 7 
q = 1048576
d = 5 
h = [310887, 216621, -265484, 174500, -17777, -120033, 509794, 472383, -207899, 137579, -79499, -124308, -276940, 128084, -47635, -98217, -355596, 162484, 512568, 115895, 173988, -275814, 147875, 300084, -307229, 211325, -513412, 353600, 122876, -294850, -157799, -22315, 520548, -82043, 94241, -280271, -362973, 322837, 514352, 334680, -320170, 345083, 46507, -339191, 340263, 395440, -232497, 298320, 78618, -27607, -35665, -233166, 315029, -153394, -327546]
en = [9456, -87785, 54329, -52914, 192255, -362113, 192465, 207086, -299247, 33383, 323491, -56227, -256001, 183181, 347373, 199740, -142719, -155115, -234629, 367075, 83187, -434294, 30813, -288694, 487957, 173236, -426480, 322954, -316362, -284385, 33915, -193405, -378609, -345142, 62277, 52961, 370215, 383304, 179975, 287570, -254824, 306056, 33864, -480414, 41906, -387104, -504860, -17263, 15869, 47583, 128248, -71449, -225403, -458480, 514062]

# thisisabinarystring.txt... inconveniently long
giveninput = "eto4rd6smo2jowyv5noeok52dtn5mmrr3xeiyv3izddncmm53sa6dlzrcryn7nlsogq5cndbuiwo2nq8ctqddoio39apg7yp8j1a8xeprpitwtcd7ryfoswddtgqampr3zwcbigiimtduiyc10rc4d25p5jdjrpalxtbjc20lpwuzmwteisd3srdoeorfarnscqva4rrevd4ewqdrd8g9xticma7bw40moe5fn0ewn4lu6rstxsetsh4s490shrrt2t4slsfwbdnasos75sdkn7mtcctrsfaz1be9ssdri1rcorf9d5fisrox6rnj1rwr53ocoqvp2nnzoigmtia1rapt3nlzsibwro2ehetetyoitmerfm2scp4oivtzod43swsenn2lvoan4w65djnjswoazd3ki29npmewhkmted31dztm3sczdemzdgi6rnogyessi9d6m7rdqrr8eo8y8tpveoojopn1mflltd6rnersq7rxn8jrpyqd8imhnqora6nbrdpxjs96pssc6em9v9zyyj5othdu0ce03do329m0oozmzuo2movsoldccemjw4ctnzndh7neist7iascplmbspuo34errc6drxevpl3tetsni5f47sflt7dzd4gibii2em6xmr2c2oq06skenewbdtuym198ia8csdqthcjcrc2r0nc9oprr9uderp6encnvovamww41rqtt5yevliioymntlrr4ek4td0mncutyodcopwpjg3eeynm7tt60qenpv0p46ikmrr0rrtc7sex606ficjmnps2dc1ehmud4nizb4nvtilusinou1ccpjtn7mepdlvyigzis2trdprzjct8iil9ccb9ziersxmhrouonsf8c9c2asacm5e51xnm2oqjueu1n1nd7ye7aziroxic4e68scog9eacrraicscb9rqksb7cuxew35h7xijrr3dj3emyocn86tsfrwtjgsqsmzoesnndsrk87ma8tpmrdjcn3mhdtw1d49c21hsyaerti4toyq527me9g7lwlltpdlpsxj9qdhtsntieint7oibz91xj5p2ljhyuc74nezqourpceuiwwe70o80eesrwinymh7whew6onob3bvr4jm7owcttiarr6w48xeiirsr0gziwosvf"

# run solve: 

# make polynomial ring 
Z.<x> = ZZ[]
# make h into polynomial 
public_key = Z(h)
# make e into polynomial 
encrypted_message = Z(en)

# get binary from cipertext.txt via "misdirection"
bin_ciphertext = input_ascii_to_bin(giveninput)
# sanity check
# assert bin_ciphertext == "1110110111001000011110001110111100110001011111100100100111010101100011100101010011011111000000000000001010110111010011011100010100010101111101010011010000010100001001000000010111110111111100111100001110101001110000111100000011100101010000111011110010001011101010100011011100110101111111000001011111011110010011110011001010011100001101101110010010100110011010111101111110101100110101100101111000101000010101010010010010110001111001011011011101010111001111010101101101100010011101010100011011111001010010001011010110010110001000111011000000001101001100110001011010010110110111110001110110011111010110010100100111101101000011111100001001010100101101100110101000101110011001000100111010101110101101011001110011110100100001011001001110111011010011011101011110000001101101100011000000101110111101100000110110101101010101100010110011110011001101101000100110111010011011001100011110101101110010100101101000110100010010110010001110110100111001011101111001001001001000000101101001101110011010100101101111111100010010111011010110010010001001111011000001100000001010010000101111111110110000000000000010011001010110100100100111101101000010011100001001010111101100000111111000101100"

# get possible (f,m) pairs
possible_keys = solve(public_key, N, p, q, d)

# get possible bin_ciphertext interpretations
bns = check_keys(possible_keys, bin_ciphertext)

# get flag
flag = decipher(bns[0], giveninput)
# assert flag == "1vy_111_7h3_hall_w17h_a_ba53ball_ba7"