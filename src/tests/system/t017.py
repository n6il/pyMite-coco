# This file is Copyright 2003 Dean Hall.
#
# This file is part of the Python-on-a-Chip program.
# Python-on-a-Chip is free software: you can redistribute it and/or modify
# it under the terms of the GNU LESSER GENERAL PUBLIC LICENSE Version 2.1.
#
# Python-on-a-Chip is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# A copy of the GNU LESSER GENERAL PUBLIC LICENSE Version 2.1
# is seen in the file COPYING up one directory from this.

#
# System Test 017
# Tests implementation of seq_compare()
#


l1 = [2,5,6]
l2 = [2,5,6]
assert l1 == l2

l3 = [2,5,5]
assert l2 != l3

t1 = (2,5,6)
t2 = (2,5,6)
assert t1 == t2

t3 = (2,5,5)
assert t2 != t3
