#!/usr/bin/env python3
# -*- coding: utf-8 -*-

#text = 'STM32F4 USB HS 设备全速模式'
text = 'CMOS STM32F4 USB HS'

def ascii2hex(h_ascii, l_ascii):
    h_val = '%c' % h_ascii
    l_val = '%c' % l_ascii

    return h_val + l_val

def convert_a_unicode(unicode_c):
    """
    unicode字符转换为小端双字节
    """

    unicode_bytes = []

    byte_c = unicode_c.encode('ascii', 'backslashreplace')
    length = len(byte_c)

    if not (1 == length or 6 == length):
        raise ValueError('参数错误')

    if 1 == length:
        """
        ASCII
        """
        #print('ASCII', end = ',')
        #print(unicode_c, end = ' :          =>')
        unicode_bytes.append('0x' + str(byte_c[0]))
        unicode_bytes.append('0x00')


    if 6 == length:
        """
        中文
        """
        #print('中文', end = ' ,')
        #print(unicode_c, end = ':') 
        #print(byte_c, end = '=>')

        # 低字节
        low_byte_str = '0x' + ascii2hex(byte_c[4], byte_c[5])
        unicode_bytes.append(low_byte_str)

        # 高字节
        high_byte_str = '0x' + ascii2hex(byte_c[2], byte_c[3])
        unicode_bytes.append(high_byte_str)

    return tuple(unicode_bytes)

if __name__ == '__main__': 
    length = 0
    for c in text:
        length += 1
        unicode_bytes = convert_a_unicode(c)
        print('    %s,%s,    //%c' %(unicode_bytes[0], unicode_bytes[1], c))

    print("长度:0x%02x" % (2*length))

