

"""
registers id 
0 rAX
1 rCX
2 rDX
3 rBX
4 SIB
5 rBP
6 rSI
7 rDI
"""

a = 1
for x in range(0, 16):
    print("{:x}".format(a+0x05))
    a *= 2

"""
06
07
09
0d
15
25
45
85
"""
