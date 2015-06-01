#!/usr/bin/env python3
# -*- coding: utf-8 -*-

g_priority_table = ['Idle', 'Low', 'BelowNormal', 'Normal', 'AboveNormal', 'High', 'Realtime']

def find_highest_one_bit(v):
    value = v
    pos = -1 

    while 0x00 != value:
        value = value >> 1
        pos = pos + 1

    return pos


def int2bin(v):
    value = v
    result = ''

    if v & 0x80: 
        result += '1'
    else:
        result += '0'

    if v & 0x40: 
        result += '1'
    else:
        result += '0'

    if v & 0x20: 
        result += '1'
    else:
        result += '0'

    if v & 0x10: 
        result += '1'
    else:
        result += '0'

    if v & 0x08: 
        result += '1'
    else:
        result += '0'

    if v & 0x04: 
        result += '1'
    else:
        result += '0'

    if v & 0x02: 
        result += '1'
    else:
        result += '0'

    if v & 0x01: 
        result += '1'
    else:
        result += '0'

    return result

def make_bitmap():
    pos = 0
    comment = ''

    print('    0,  /* 00000000 Idle 没有任何优先级线程就绪 这是不可能的情况 */')

    for i in range(1 ,0x80):
        pos = find_highest_one_bit(i) 
        comment = ',  /* '

        print('    ', end = '')
        print(pos, end = '') 
        comment += int2bin(i)
        comment += ' ' + g_priority_table[pos] + ' */'

        print(comment)

if __name__ == '__main__':
    make_bitmap()

