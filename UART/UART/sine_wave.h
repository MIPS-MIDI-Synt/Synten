#include <stdint.h>

uint16_t sine_table[1024] =
{
        0x200, 0x203, 0x206, 0x209, 0x20c, 0x20f, 0x212, 0x215,
        0x219, 0x21c, 0x21f, 0x222, 0x225, 0x228, 0x22b, 0x22f,
        0x232, 0x235, 0x238, 0x23b, 0x23e, 0x241, 0x244, 0x248,
        0x24b, 0x24e, 0x251, 0x254, 0x257, 0x25a, 0x25d, 0x260,
        0x263, 0x266, 0x26a, 0x26d, 0x270, 0x273, 0x276, 0x279,
        0x27c, 0x27f, 0x282, 0x285, 0x288, 0x28b, 0x28e, 0x291,
        0x294, 0x297, 0x29a, 0x29d, 0x2a0, 0x2a3, 0x2a6, 0x2a9,
        0x2ac, 0x2af, 0x2b2, 0x2b5, 0x2b8, 0x2bb, 0x2be, 0x2c1,
        0x2c3, 0x2c6, 0x2c9, 0x2cc, 0x2cf, 0x2d2, 0x2d5, 0x2d8,
        0x2da, 0x2dd, 0x2e0, 0x2e3, 0x2e6, 0x2e8, 0x2eb, 0x2ee,
        0x2f1, 0x2f4, 0x2f6, 0x2f9, 0x2fc, 0x2ff, 0x301, 0x304,
        0x307, 0x309, 0x30c, 0x30f, 0x311, 0x314, 0x317, 0x319,
        0x31c, 0x31f, 0x321, 0x324, 0x326, 0x329, 0x32b, 0x32e,
        0x330, 0x333, 0x335, 0x338, 0x33a, 0x33d, 0x33f, 0x342,
        0x344, 0x347, 0x349, 0x34b, 0x34e, 0x350, 0x353, 0x355,
        0x357, 0x35a, 0x35c, 0x35e, 0x360, 0x363, 0x365, 0x367,
        0x369, 0x36c, 0x36e, 0x370, 0x372, 0x374, 0x377, 0x379,
        0x37b, 0x37d, 0x37f, 0x381, 0x383, 0x385, 0x387, 0x389,
        0x38b, 0x38d, 0x38f, 0x391, 0x393, 0x395, 0x397, 0x399,
        0x39b, 0x39c, 0x39e, 0x3a0, 0x3a2, 0x3a4, 0x3a6, 0x3a7,
        0x3a9, 0x3ab, 0x3ad, 0x3ae, 0x3b0, 0x3b2, 0x3b3, 0x3b5,
        0x3b6, 0x3b8, 0x3ba, 0x3bb, 0x3bd, 0x3be, 0x3c0, 0x3c1,
        0x3c3, 0x3c4, 0x3c6, 0x3c7, 0x3c9, 0x3ca, 0x3cb, 0x3cd,
        0x3ce, 0x3cf, 0x3d1, 0x3d2, 0x3d3, 0x3d5, 0x3d6, 0x3d7,
        0x3d8, 0x3d9, 0x3db, 0x3dc, 0x3dd, 0x3de, 0x3df, 0x3e0,
        0x3e1, 0x3e2, 0x3e3, 0x3e4, 0x3e5, 0x3e6, 0x3e7, 0x3e8,
        0x3e9, 0x3ea, 0x3eb, 0x3ec, 0x3ed, 0x3ed, 0x3ee, 0x3ef,
        0x3f0, 0x3f1, 0x3f1, 0x3f2, 0x3f3, 0x3f3, 0x3f4, 0x3f5,
        0x3f5, 0x3f6, 0x3f6, 0x3f7, 0x3f8, 0x3f8, 0x3f9, 0x3f9,
        0x3fa, 0x3fa, 0x3fa, 0x3fb, 0x3fb, 0x3fc, 0x3fc, 0x3fc,
        0x3fd, 0x3fd, 0x3fd, 0x3fd, 0x3fe, 0x3fe, 0x3fe, 0x3fe,
        0x3fe, 0x3ff, 0x3ff, 0x3ff, 0x3ff, 0x3ff, 0x3ff, 0x3ff,
        0x3ff, 0x3ff, 0x3ff, 0x3ff, 0x3ff, 0x3ff, 0x3ff, 0x3fe,
        0x3fe, 0x3fe, 0x3fe, 0x3fe, 0x3fe, 0x3fd, 0x3fd, 0x3fd,
        0x3fc, 0x3fc, 0x3fc, 0x3fb, 0x3fb, 0x3fb, 0x3fa, 0x3fa,
        0x3f9, 0x3f9, 0x3f8, 0x3f8, 0x3f7, 0x3f7, 0x3f6, 0x3f6,
        0x3f5, 0x3f4, 0x3f4, 0x3f3, 0x3f2, 0x3f2, 0x3f1, 0x3f0,
        0x3ef, 0x3ef, 0x3ee, 0x3ed, 0x3ec, 0x3eb, 0x3eb, 0x3ea,
        0x3e9, 0x3e8, 0x3e7, 0x3e6, 0x3e5, 0x3e4, 0x3e3, 0x3e2,
        0x3e1, 0x3e0, 0x3df, 0x3de, 0x3dc, 0x3db, 0x3da, 0x3d9,
        0x3d8, 0x3d6, 0x3d5, 0x3d4, 0x3d3, 0x3d1, 0x3d0, 0x3cf,
        0x3cd, 0x3cc, 0x3cb, 0x3c9, 0x3c8, 0x3c6, 0x3c5, 0x3c4,
        0x3c2, 0x3c1, 0x3bf, 0x3be, 0x3bc, 0x3ba, 0x3b9, 0x3b7,
        0x3b6, 0x3b4, 0x3b2, 0x3b1, 0x3af, 0x3ad, 0x3ac, 0x3aa,
        0x3a8, 0x3a6, 0x3a5, 0x3a3, 0x3a1, 0x39f, 0x39d, 0x39c,
        0x39a, 0x398, 0x396, 0x394, 0x392, 0x390, 0x38e, 0x38c,
        0x38a, 0x388, 0x386, 0x384, 0x382, 0x380, 0x37e, 0x37c,
        0x37a, 0x378, 0x375, 0x373, 0x371, 0x36f, 0x36d, 0x36b,
        0x368, 0x366, 0x364, 0x362, 0x35f, 0x35d, 0x35b, 0x358,
        0x356, 0x354, 0x351, 0x34f, 0x34d, 0x34a, 0x348, 0x345,
        0x343, 0x341, 0x33e, 0x33c, 0x339, 0x337, 0x334, 0x332,
        0x32f, 0x32d, 0x32a, 0x328, 0x325, 0x322, 0x320, 0x31d,
        0x31b, 0x318, 0x315, 0x313, 0x310, 0x30d, 0x30b, 0x308,
        0x305, 0x303, 0x300, 0x2fd, 0x2fa, 0x2f8, 0x2f5, 0x2f2,
        0x2ef, 0x2ed, 0x2ea, 0x2e7, 0x2e4, 0x2e1, 0x2df, 0x2dc,
        0x2d9, 0x2d6, 0x2d3, 0x2d0, 0x2ce, 0x2cb, 0x2c8, 0x2c5,
        0x2c2, 0x2bf, 0x2bc, 0x2b9, 0x2b6, 0x2b3, 0x2b0, 0x2ad,
        0x2ab, 0x2a8, 0x2a5, 0x2a2, 0x29f, 0x29c, 0x299, 0x296,
        0x293, 0x290, 0x28d, 0x28a, 0x287, 0x284, 0x280, 0x27d,
        0x27a, 0x277, 0x274, 0x271, 0x26e, 0x26b, 0x268, 0x265,
        0x262, 0x25f, 0x25c, 0x259, 0x255, 0x252, 0x24f, 0x24c,
        0x249, 0x246, 0x243, 0x240, 0x23d, 0x239, 0x236, 0x233,
        0x230, 0x22d, 0x22a, 0x227, 0x224, 0x220, 0x21d, 0x21a,
        0x217, 0x214, 0x211, 0x20e, 0x20a, 0x207, 0x204, 0x201,
        0x1fe, 0x1fb, 0x1f8, 0x1f5, 0x1f1, 0x1ee, 0x1eb, 0x1e8,
        0x1e5, 0x1e2, 0x1df, 0x1db, 0x1d8, 0x1d5, 0x1d2, 0x1cf,
        0x1cc, 0x1c9, 0x1c6, 0x1c2, 0x1bf, 0x1bc, 0x1b9, 0x1b6,
        0x1b3, 0x1b0, 0x1ad, 0x1aa, 0x1a6, 0x1a3, 0x1a0, 0x19d,
        0x19a, 0x197, 0x194, 0x191, 0x18e, 0x18b, 0x188, 0x185,
        0x182, 0x17f, 0x17b, 0x178, 0x175, 0x172, 0x16f, 0x16c,
        0x169, 0x166, 0x163, 0x160, 0x15d, 0x15a, 0x157, 0x154,
        0x152, 0x14f, 0x14c, 0x149, 0x146, 0x143, 0x140, 0x13d,
        0x13a, 0x137, 0x134, 0x131, 0x12f, 0x12c, 0x129, 0x126,
        0x123, 0x120, 0x11e, 0x11b, 0x118, 0x115, 0x112, 0x110,
        0x10d, 0x10a, 0x107, 0x105, 0x102, 0xff, 0xfc, 0xfa,
        0x0f7, 0xf4, 0xf2, 0xef, 0xec, 0xea, 0xe7, 0xe4,
        0x0e2, 0xdf, 0xdd, 0xda, 0xd7, 0xd5, 0xd2, 0xd0,
        0x0cd, 0xcb, 0xc8, 0xc6, 0xc3, 0xc1, 0xbe, 0xbc,
        0x0ba, 0xb7, 0xb5, 0xb2, 0xb0, 0xae, 0xab, 0xa9,
        0x0a7, 0xa4, 0xa2, 0xa0, 0x9d, 0x9b, 0x99, 0x97,
        0x094, 0x92, 0x90, 0x8e, 0x8c, 0x8a, 0x87, 0x85,
        0x083, 0x81, 0x7f, 0x7d, 0x7b, 0x79, 0x77, 0x75,
        0x073, 0x71, 0x6f, 0x6d, 0x6b, 0x69, 0x67, 0x65,
        0x063, 0x62, 0x60, 0x5e, 0x5c, 0x5a, 0x59, 0x57,
        0x055, 0x53, 0x52, 0x50, 0x4e, 0x4d, 0x4b, 0x49,
        0x048, 0x46, 0x45, 0x43, 0x41, 0x40, 0x3e, 0x3d,
        0x03b, 0x3a, 0x39, 0x37, 0x36, 0x34, 0x33, 0x32,
        0x030, 0x2f, 0x2e, 0x2c, 0x2b, 0x2a, 0x29, 0x27,
        0x026, 0x25, 0x24, 0x23, 0x21, 0x20, 0x1f, 0x1e,
        0x01d, 0x1c, 0x1b, 0x1a, 0x19, 0x18, 0x17, 0x16,
        0x015, 0x14, 0x14, 0x13, 0x12, 0x11, 0x10, 0x10,
        0x00f, 0xe, 0xd, 0xd, 0xc, 0xb, 0xb, 0xa,
        0x009, 0x9, 0x8, 0x8, 0x7, 0x7, 0x6, 0x6,
        0x005, 0x5, 0x4, 0x4, 0x4, 0x3, 0x3, 0x3,
        0x002, 0x2, 0x2, 0x1, 0x1, 0x1, 0x1, 0x1,
        0x001, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
        0x000, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1,
        0x001, 0x1, 0x1, 0x1, 0x2, 0x2, 0x2, 0x2,
        0x003, 0x3, 0x3, 0x4, 0x4, 0x5, 0x5, 0x5,
        0x006, 0x6, 0x7, 0x7, 0x8, 0x9, 0x9, 0xa,
        0x00a, 0xb, 0xc, 0xc, 0xd, 0xe, 0xe, 0xf,
        0x010, 0x11, 0x12, 0x12, 0x13, 0x14, 0x15, 0x16,
        0x017, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e,
        0x01f, 0x20, 0x21, 0x22, 0x23, 0x24, 0x26, 0x27,
        0x028, 0x29, 0x2a, 0x2c, 0x2d, 0x2e, 0x30, 0x31,
        0x032, 0x34, 0x35, 0x36, 0x38, 0x39, 0x3b, 0x3c,
        0x03e, 0x3f, 0x41, 0x42, 0x44, 0x45, 0x47, 0x49,
        0x04a, 0x4c, 0x4d, 0x4f, 0x51, 0x52, 0x54, 0x56,
        0x058, 0x59, 0x5b, 0x5d, 0x5f, 0x61, 0x63, 0x64,
        0x066, 0x68, 0x6a, 0x6c, 0x6e, 0x70, 0x72, 0x74,
        0x076, 0x78, 0x7a, 0x7c, 0x7e, 0x80, 0x82, 0x84,
        0x086, 0x88, 0x8b, 0x8d, 0x8f, 0x91, 0x93, 0x96,
        0x098, 0x9a, 0x9c, 0x9f, 0xa1, 0xa3, 0xa5, 0xa8,
        0x0aa, 0xac, 0xaf, 0xb1, 0xb4, 0xb6, 0xb8, 0xbb,
        0x0bd, 0xc0, 0xc2, 0xc5, 0xc7, 0xca, 0xcc, 0xcf,
        0x0d1, 0xd4, 0xd6, 0xd9, 0xdb, 0xde, 0xe0, 0xe3,
        0x0e6, 0xe8, 0xeb, 0xee, 0xf0, 0xf3, 0xf6, 0xf8,
        0x0fb, 0xfe, 0x100, 0x103, 0x106, 0x109, 0x10b, 0x10e,
        0x111, 0x114, 0x117, 0x119, 0x11c, 0x11f, 0x122, 0x125,
        0x127, 0x12a, 0x12d, 0x130, 0x133, 0x136, 0x139, 0x13c,
        0x13e, 0x141, 0x144, 0x147, 0x14a, 0x14d, 0x150, 0x153,
        0x156, 0x159, 0x15c, 0x15f, 0x162, 0x165, 0x168, 0x16b,
        0x16e, 0x171, 0x174, 0x177, 0x17a, 0x17d, 0x180, 0x183,
        0x186, 0x189, 0x18c, 0x18f, 0x192, 0x195, 0x199, 0x19c,
        0x19f, 0x1a2, 0x1a5, 0x1a8, 0x1ab, 0x1ae, 0x1b1, 0x1b4,
        0x1b7, 0x1bb, 0x1be, 0x1c1, 0x1c4, 0x1c7, 0x1ca, 0x1cd,
        0x1d0, 0x1d4, 0x1d7, 0x1da, 0x1dd, 0x1e0, 0x1e3, 0x1e6,
        0x1ea, 0x1ed, 0x1f0, 0x1f3, 0x1f6, 0x1f9, 0x1fc, 0x200
};