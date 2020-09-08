from cs50 import get_int

def block_builder(n):
    #empty space
    if n < 1 or n > 8:
        block_builder(get_int("Invalid, insert height: "))
    #block
    for i in range(1,n+1):
        print(" "*(n-i)+"#"*i + "  " + "#"*i)
    

    

block_builder(get_int("Height: "))
    
    