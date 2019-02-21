Algorithms: Project 1
Julie Aichinger

Part 1:
In this part of the project the objective was to implemet 
RSA encryption. This was done by implementing Fermat's test 
to generate p and q, both of which are two large prime 
numbers with a size >= 512 bits. These values of p and 
q were then saved in a file named p_q.txt. This file 
had one integer per line, and no whitespace. The extended 
Eclidean algorithm was then used to generate two pairs of 
keys: e and n, d and n, where n=p*q. Both keys were then saved 
in their respective file, e_n.txt and d_n.txt. This again had 
one integer per line and no whitespace.

To run part1:
Locate directory in the terminal. 
"make clean"
"make all"
"./rsa435"

Part 2:
In this part of the project the objective was to generate 
and verify digital signatures using SHA-256 hash. This was
done by generatring a SHA-256 hash of the content of the file
to be signed. Then using the private key in d_n.txt it dycrypts
the hash value. It then signs and the signature is saved to 
file.txt.siganture. To verify the signature and that the file 
has not been changed, it then obtains the signature from the 
file.txt.signature and using the public keys from e_n.txt is able
to encrypt the signature. Then by comparing the encrypted signature 
and the SHA-256 hash value, it reports if the document is authentic 
or modified.

To run part1:
Locate directory in the terminal. 
Make sure there is a e_n.txt and a d_n.txt.
"make clean"
"make all"
To sign: "./messageDigest435 s file.txt"
To verify: "./messageDigest435 v file.txt"

