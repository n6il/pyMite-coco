#!/usr/bin/env python

# This file is Copyright 2001 Dean Hall.
# This file is part of the Python-on-a-Chip tools.
# This software is licensed under the MIT License.
# See the LICENSE file for details.

"""
Python Byte Code utility functions.

count(fn) - count the number of times each byte code
            appears in a code object.
"""

## @file
#  @copybrief bcode

## @package bcode
#  @brief Python Byte Code utility functions.
#
#  count(fn) - count the number of times each byte code
#              appears in a code object.



import dis, types


#The highest numbered byte code
MAX_BCODE = 150


def count(fn):
    """
    Compile the python source file named fn.
    Count all the bytecodes in the file.
    Return a list of counts indexed by bcode.
    """

    #create a code object
    src = open(fn).read()
    co  = compile(src, fn, "exec")

    #init list
    cnt = [0,] * MAX_BCODE

    #iterate through the nested code objects
    _rcount(co, cnt)

    return cnt


def _rcount(co, count):
    """
    Recursively descend through all the code objects
    in the given code object, co.
    Add byte code counts to the count dict.
    """
    #current byte code string
    str = co.co_code
    strlen = len(str)

    #count byte codes in current string
    i = 0
    while i < strlen:
        bcode = ord(str[i])
        i += 1
        #incr count
        count[bcode] += 1

        #if bcode has arg, incr ptr by 2 bytes
        if bcode >= 90:
            i += 2

    #count code objects in constant pool
    for obj in co.co_consts:
        if type(obj) == types.CodeType:
            _rcount(obj, count)

    return


def main(fn):
    """
    Count the bytecodes in the file, fn,
    and print them out in human-readable form.
    """
    #count the bytecodes in this file
    c = count(fn)

    #print bcode name and count
    for i in range(len(c)):
        if c[i]:
            print dis.opname[i], ":\t", c[i]
    return


if __name__ == "__main__":
    main("c:\\dwh\\tech\\cis\\py\\snap.py")
