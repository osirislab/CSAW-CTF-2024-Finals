#!/usr/bin/env python

from pathlib import Path


def one_byte(value):
    return value.to_bytes(1, "little")

def two_bytes(value):
    return value.to_bytes(2, "little")

def four_bytes(value):
    return value.to_bytes(4, "little")


def get_color_table():
    color_table = bytearray()
    color_table += one_byte(0x0)
    color_table += one_byte(0x0)
    color_table += one_byte(0x0)
    color_table += one_byte(0x0)
    color_table += one_byte(0x0)
    color_table += one_byte(0x0)
    color_table += one_byte(0xff)
    color_table += one_byte(0x0)
    color_table += one_byte(0x0)
    color_table += one_byte(0xff)
    color_table += one_byte(0x0)
    color_table += one_byte(0x0)
    color_table += one_byte(0xff)
    color_table += one_byte(0x0)
    color_table += one_byte(0x0)
    color_table += one_byte(0x0)
    color_table += one_byte(0xff)
    color_table += one_byte(0xff)
    color_table += one_byte(0xff)
    color_table += one_byte(0x0)
    color_table += one_byte(0x0)
    color_table += one_byte(0xff)
    color_table += one_byte(0xff)
    color_table += one_byte(0x0)
    color_table += one_byte(0xff)
    color_table += one_byte(0x0)
    color_table += one_byte(0xff)
    color_table += one_byte(0x0)
    color_table += one_byte(0xff)
    color_table += one_byte(0xff)
    color_table += one_byte(0x0)
    color_table += one_byte(0x0)
    color_table += one_byte(0x80)
    color_table += one_byte(0x0)
    color_table += one_byte(0xff)
    color_table += one_byte(0x0)
    color_table += one_byte(0x40)
    color_table += one_byte(0x80)
    color_table += one_byte(0xff)
    color_table += one_byte(0x0)
    color_table += one_byte(0x0)
    color_table += one_byte(0x40)
    color_table += one_byte(0x80)
    color_table += one_byte(0x0)
    color_table += one_byte(0x80)
    color_table += one_byte(0x80)
    color_table += one_byte(0x0)
    color_table += one_byte(0x0)
    color_table += one_byte(0x0)
    color_table += one_byte(0x0)
    color_table += one_byte(0x80)
    color_table += one_byte(0x0)
    color_table += one_byte(0x80)
    color_table += one_byte(0x0)
    color_table += one_byte(0x80)
    color_table += one_byte(0x0)
    color_table += one_byte(0x80)
    color_table += one_byte(0xff)
    color_table += one_byte(0x80)
    color_table += one_byte(0x0)
    color_table += one_byte(0x80)
    color_table += one_byte(0xff)
    color_table += one_byte(0xff)
    color_table += one_byte(0x0)

    return color_table


def get_bitmap_4bits():
    file_header = bytearray()
    file_header += two_bytes(0x4d42)
    file_header += four_bytes(0x96)
    file_header += two_bytes(0)
    file_header += two_bytes(0)
    file_header += four_bytes(0x76)
    file_header += four_bytes(0x28)
    file_header += four_bytes(8)
    file_header += four_bytes(8)
    file_header += two_bytes(1)
    file_header += two_bytes(0x4)
    file_header += four_bytes(0)
    file_header += four_bytes(0x20)
    file_header += four_bytes(0)
    file_header += four_bytes(0)
    file_header += four_bytes(16)
    file_header += four_bytes(16)

    color_table = get_color_table()

    bitmap = bytearray()
    bitmap += one_byte(0x22)
    bitmap += one_byte(0x22)
    bitmap += one_byte(0x22)
    bitmap += one_byte(0x22)
    bitmap += one_byte(0x01)
    bitmap += one_byte(0x41)
    bitmap += one_byte(0x41)
    bitmap += one_byte(0x40)
    bitmap += one_byte(0x04)
    bitmap += one_byte(0x14)
    bitmap += one_byte(0x14)
    bitmap += one_byte(0x10)
    bitmap += one_byte(0x05)
    bitmap += one_byte(0x55)
    bitmap += one_byte(0x55)
    bitmap += one_byte(0x50)
    bitmap += one_byte(0x05)
    bitmap += one_byte(0x55)
    bitmap += one_byte(0x55)
    bitmap += one_byte(0x50)
    bitmap += one_byte(0x04)
    bitmap += one_byte(0x14)
    bitmap += one_byte(0x14)
    bitmap += one_byte(0x10)
    bitmap += one_byte(0x01)
    bitmap += one_byte(0x41)
    bitmap += one_byte(0x41)
    bitmap += one_byte(0x40)
    bitmap += one_byte(0x33)
    bitmap += one_byte(0x33)
    bitmap += one_byte(0x33)
    bitmap += one_byte(0x33)
    
    return file_header + color_table + bitmap


def get_bitmap_24bits():
    file_header = bytearray()
    file_header += two_bytes(0x4d42)
    file_header += four_bytes(0x46)
    file_header += two_bytes(0)
    file_header += two_bytes(0)
    file_header += four_bytes(0x36)
    file_header += four_bytes(0x28)
    file_header += four_bytes(2)
    file_header += four_bytes(2)
    file_header += two_bytes(1)
    file_header += two_bytes(0x18)
    file_header += four_bytes(0)
    file_header += four_bytes(0x16)
    file_header += four_bytes(0)
    file_header += four_bytes(0)
    file_header += four_bytes(0)
    file_header += four_bytes(0)

    bitmap = bytearray()
    bitmap += one_byte(0x0)
    bitmap += one_byte(0x0)
    bitmap += one_byte(0xff)
    bitmap += one_byte(0xff)
    bitmap += one_byte(0xff)
    bitmap += one_byte(0xff)
    bitmap += one_byte(0x0)
    bitmap += one_byte(0x0)
    bitmap += one_byte(0xff)
    bitmap += one_byte(0x0)
    bitmap += one_byte(0x0)
    bitmap += one_byte(0x0)
    bitmap += one_byte(0xff)
    bitmap += one_byte(0x0)
    bitmap += one_byte(0x0)
    bitmap += one_byte(0x0)
    
    return file_header + bitmap


def main():
    bitmap = get_bitmap_24bits()
    Path('./test_image1').write_bytes(bitmap)

    bitmap = get_bitmap_4bits()
    Path('./test_image2').write_bytes(bitmap)

    
    return 0



if __name__ == '__main__':
    main()
