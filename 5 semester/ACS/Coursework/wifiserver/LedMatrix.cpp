#include "ledmatrix.h"

//**************************************************************************************************

const uint8_t PROGMEM LedMatrix::font[] =
{
  0x00, 0x00, 0x00, 0x00, 0x00,
  0x3E, 0x5B, 0x4F, 0x5B, 0x3E,
  0x3E, 0x6B, 0x4F, 0x6B, 0x3E,
  0x1C, 0x3E, 0x7C, 0x3E, 0x1C,
  0x18, 0x3C, 0x7E, 0x3C, 0x18,
  0x1C, 0x57, 0x7D, 0x57, 0x1C,
  0x1C, 0x5E, 0x7F, 0x5E, 0x1C,
  0x00, 0x18, 0x3C, 0x18, 0x00,
  0xFF, 0xE7, 0xC3, 0xE7, 0xFF,
  0x00, 0x18, 0x24, 0x18, 0x00,
  0xFF, 0xE7, 0xDB, 0xE7, 0xFF,
  0x30, 0x48, 0x3A, 0x06, 0x0E,
  0x26, 0x29, 0x79, 0x29, 0x26,
  0x40, 0x7F, 0x05, 0x05, 0x07,
  0x40, 0x7F, 0x05, 0x25, 0x3F,
  0x5A, 0x3C, 0xE7, 0x3C, 0x5A,
  0x7F, 0x3E, 0x1C, 0x1C, 0x08,
  0x08, 0x1C, 0x1C, 0x3E, 0x7F,
  0x14, 0x22, 0x7F, 0x22, 0x14,
  0x5F, 0x5F, 0x00, 0x5F, 0x5F,
  0x06, 0x09, 0x7F, 0x01, 0x7F,
  0x00, 0x66, 0x89, 0x95, 0x6A,
  0x60, 0x60, 0x60, 0x60, 0x60,
  0x94, 0xA2, 0xFF, 0xA2, 0x94,
  0x08, 0x04, 0x7E, 0x04, 0x08,
  0x10, 0x20, 0x7E, 0x20, 0x10,
  0x08, 0x08, 0x2A, 0x1C, 0x08,
  0x08, 0x1C, 0x2A, 0x08, 0x08,
  0x1E, 0x10, 0x10, 0x10, 0x10,
  0x0C, 0x1E, 0x0C, 0x1E, 0x0C,
  0x30, 0x38, 0x3E, 0x38, 0x30,
  0x06, 0x0E, 0x3E, 0x0E, 0x06,
  0x00, 0x00, 0x00, 0x00, 0x00,   //   0x20  32
  0x00, 0x00, 0x5F, 0x00, 0x00,   // ! 0x21  33
  0x00, 0x07, 0x00, 0x07, 0x00,   // " 0x22  34
  0x14, 0x7F, 0x14, 0x7F, 0x14,   // # 0x23  35
  0x24, 0x2A, 0x7F, 0x2A, 0x12,   // $ 0x24  36
  0x4C, 0x2C, 0x10, 0x68, 0x64,   // % 0x25  37
  0x36, 0x49, 0x55, 0x22, 0x50,   // & 0x26  38
  0x00, 0x05, 0x03, 0x00, 0x00,   // ' 0x27  39
  0x00, 0x1C, 0x22, 0x41, 0x00,   // ( 0x28  40
  0x00, 0x41, 0x22, 0x1C, 0x00,   // ) 0x29  41
  0x14, 0x08, 0x3E, 0x08, 0x14,   // * 0x2A  42
  0x08, 0x08, 0x3E, 0x08, 0x08,   // + 0x2B  43
  0x00, 0x00, 0x50, 0x30, 0x00,   // , 0x2C  44
  0x10, 0x10, 0x10, 0x10, 0x10,   // - 0x2D  45
  0x00, 0x60, 0x60, 0x00, 0x00,   // . 0x2E  46
  0x20, 0x10, 0x08, 0x04, 0x02,   // / 0x2F  47
  0x3E, 0x51, 0x49, 0x45, 0x3E,   // 0 0x30  48
  0x00, 0x42, 0x7F, 0x40, 0x00,   // 1 0x31  49
  0x42, 0x61, 0x51, 0x49, 0x46,   // 2 0x32  50
  0x21, 0x41, 0x45, 0x4B, 0x31,   // 3 0x33  51
  0x18, 0x14, 0x12, 0x7F, 0x10,   // 4 0x34  52
  0x27, 0x45, 0x45, 0x45, 0x39,   // 5 0x35  53
  0x3C, 0x4A, 0x49, 0x49, 0x30,   // 6 0x36  54
  0x01, 0x71, 0x09, 0x05, 0x03,   // 7 0x37  55
  0x36, 0x49, 0x49, 0x49, 0x36,   // 8 0x38  56
  0x06, 0x49, 0x49, 0x29, 0x1E,   // 9 0x39  57
  0x00, 0x36, 0x36, 0x00, 0x00,   // : 0x3A  58
  0x00, 0x56, 0x36, 0x00, 0x00,   // ; 0x3B  59
  0x08, 0x14, 0x22, 0x41, 0x00,   // < 0x3C  60
  0x14, 0x14, 0x14, 0x14, 0x14,   // = 0x3D  61
  0x00, 0x41, 0x22, 0x14, 0x08,   // > 0x3E  62
  0x02, 0x01, 0x51, 0x09, 0x06,   // ? 0x3F  63
  0x3C, 0x42, 0x5A, 0x56, 0x0C,   // @ 0x40  64
  0x7E, 0x11, 0x11, 0x11, 0x7E,   // A 0x41  65
  0x7F, 0x49, 0x49, 0x49, 0x36,   // B 0x42  66
  0x3E, 0x41, 0x41, 0x41, 0x22,   // C 0x43  67
  0x7F, 0x41, 0x41, 0x22, 0x1C,   // D 0x44  68
  0x7F, 0x49, 0x49, 0x49, 0x41,   // E 0x45  69
  0x7F, 0x09, 0x09, 0x09, 0x01,   // F 0x46  70
  0x3E, 0x41, 0x49, 0x49, 0x7A,   // G 0x47  71
  0x7F, 0x08, 0x08, 0x08, 0x7F,   // H 0x48  72
  0x00, 0x41, 0x7F, 0x41, 0x00,   // I 0x49  73
  0x20, 0x40, 0x41, 0x3F, 0x01,   // J 0x4A  74
  0x7F, 0x08, 0x14, 0x22, 0x41,   // K 0x4B  75
  0x7F, 0x40, 0x40, 0x40, 0x40,   // L 0x4C  76
  0x7F, 0x02, 0x0C, 0x02, 0x7F,   // M 0x4D  77
  0x7F, 0x04, 0x08, 0x10, 0x7F,   // N 0x4E  78
  0x3E, 0x41, 0x41, 0x41, 0x3E,   // O 0x4F  79
  0x7F, 0x09, 0x09, 0x09, 0x06,   // P 0x50  80
  0x3E, 0x41, 0x51, 0x21, 0x5E,   // Q 0x51  81
  0x7F, 0x09, 0x19, 0x29, 0x46,   // R 0x52  82
  0x46, 0x49, 0x49, 0x49, 0x31,   // S 0x53  83
  0x01, 0x01, 0x7F, 0x01, 0x01,   // T 0x54  84
  0x3F, 0x40, 0x40, 0x40, 0x3F,   // U 0x55  85
  0x1F, 0x20, 0x40, 0x20, 0x1F,   // V 0x56  86
  0x3F, 0x40, 0x38, 0x40, 0x3F,   // W 0x57  87
  0x63, 0x14, 0x08, 0x14, 0x63,   // X 0x58  88
  0x07, 0x08, 0x70, 0x08, 0x07,   // Y 0x59  89
  0x61, 0x51, 0x49, 0x45, 0x43,   // Z 0x5A  90
  0x00, 0x7F, 0x41, 0x41, 0x00,   // [ 0x5B  91
  0x02, 0x04, 0x08, 0x10, 0x20,   // \ 0x5C  92
  0x00, 0x41, 0x41, 0x7F, 0x00,   // ] 0x5D  93
  0x04, 0x02, 0x01, 0x02, 0x04,   // ^ 0x5E  94
  0x40, 0x40, 0x40, 0x40, 0x40,   // _ 0x5F  95
  0x00, 0x01, 0x02, 0x04, 0x00,   // ` 0x60  96
  0x20, 0x54, 0x54, 0x54, 0x78,   // a 0x61  97
  0x7F, 0x48, 0x44, 0x44, 0x38,   // b 0x62  98
  0x38, 0x44, 0x44, 0x44, 0x20,   // c 0x63  99
  0x38, 0x44, 0x44, 0x48, 0x7F,   // d 0x64 100
  0x38, 0x54, 0x54, 0x54, 0x18,   // e 0x65 101
  0x08, 0x7E, 0x09, 0x01, 0x02,   // f 0x66 102
  0x0C, 0x52, 0x52, 0x52, 0x3E,   // g 0x67 103
  0x7F, 0x08, 0x04, 0x04, 0x78,   // h 0x68 104
  0x00, 0x44, 0x7D, 0x40, 0x00,   // i 0x69 105
  0x20, 0x40, 0x44, 0x3D, 0x00,   // j 0x6A 106
  0x7F, 0x10, 0x28, 0x44, 0x00,   // k 0x6B 107
  0x00, 0x41, 0x7F, 0x40, 0x00,   // l 0x6C 108
  0x7C, 0x04, 0x18, 0x04, 0x78,   // m 0x6D 109
  0x7C, 0x08, 0x04, 0x04, 0x78,   // n 0x6E 110
  0x38, 0x44, 0x44, 0x44, 0x38,   // o 0x6F 111
  0x7C, 0x14, 0x14, 0x14, 0x08,   // p 0x70 112
  0x08, 0x14, 0x14, 0x18, 0x7C,   // q 0x71 113
  0x7C, 0x08, 0x04, 0x04, 0x08,   // r 0x72 114
  0x48, 0x54, 0x54, 0x54, 0x20,   // s 0x73 115
  0x04, 0x3F, 0x44, 0x40, 0x20,   // t 0x74 116
  0x3C, 0x40, 0x40, 0x20, 0x7C,   // u 0x75 117
  0x1C, 0x20, 0x40, 0x20, 0x1C,   // v 0x76 118
  0x3C, 0x40, 0x30, 0x40, 0x3C,   // w 0x77 119
  0x44, 0x28, 0x10, 0x28, 0x44,   // x 0x78 120
  0x0C, 0x50, 0x50, 0x50, 0x3C,   // y 0x79 121
  0x44, 0x64, 0x54, 0x4C, 0x44,   // z 0x7A 122
  0x00, 0x08, 0x36, 0x41, 0x00,   // { 0x7B 123
  0x00, 0x00, 0x7F, 0x00, 0x00,   // | 0x7C 124
  0x00, 0x41, 0x36, 0x08, 0x00,   // } 0x7D 125
  0x08, 0x04, 0x08, 0x10, 0x08,   // ~ 0x7E 126
  0x00, 0x00, 0x00, 0x00, 0x00,   //  0x7F 127
  0x00, 0x00, 0x00, 0x00, 0x00,   //  0x80 128
  0x00, 0x00, 0x00, 0x00, 0x00,   //  0x81 129
  0x00, 0x00, 0x00, 0x00, 0x00,   //  0x82 130
  0x00, 0x00, 0x00, 0x00, 0x00,   //  0x83 131
  0x00, 0x00, 0x00, 0x00, 0x00,   //  0x84 132
  0x40, 0x00, 0x40, 0x00, 0x40,   //  0x85 133
  0x04, 0x04, 0xFF, 0x04, 0x04,   //  0x86 134
  0x24, 0x24, 0xFF, 0x24, 0x24,   //  0x87 135
  0x28, 0x7C, 0xAA, 0xAA, 0x82,   //  0x88 136
  0x00, 0x00, 0x00, 0x00, 0x00,   //  0x89 137
  0x00, 0x00, 0x00, 0x00, 0x00,   //  0x8A 138
  0x00, 0x00, 0x00, 0x00, 0x00,   //  0x8B 139
  0x00, 0x00, 0x00, 0x00, 0x00,   //  0x8C 140
  0x00, 0x00, 0x00, 0x00, 0x00,   //  0x8D 141
  0x00, 0x00, 0x00, 0x00, 0x00,   //  0x8E 142
  0x00, 0x00, 0x00, 0x00, 0x00,   //  0x8F 143
  0x00, 0x00, 0x00, 0x00, 0x00,   //  0x90 144
  0x00, 0x06, 0x05, 0x00, 0x00,   //  0x91 145
  0x00, 0x00, 0x05, 0x03, 0x00,   //  0x92 146
  0x06, 0x05, 0x00, 0x06, 0x05,   //  0x93 147
  0x05, 0x03, 0x00, 0x05, 0x03,   //  0x94 148
  0x18, 0x3C, 0x3C, 0x3C, 0x18,   //  0x95 149
  0x00, 0x00, 0x00, 0x00, 0x00,   //  0x96 150
  0x00, 0x00, 0x00, 0x00, 0x00,   //  0x97 151
  0x78, 0x48, 0x48, 0x78, 0x00,   //   0x98 152
  0x00, 0x00, 0x00, 0x00, 0x00,   //  0x99 153
  0x00, 0x00, 0x00, 0x00, 0x00,   //  0x9A 154
  0x00, 0x00, 0x00, 0x00, 0x00,   //  0x9B 155
  0x00, 0x00, 0x00, 0x00, 0x00,   //  0x9C 156
  0x00, 0x00, 0x00, 0x00, 0x00,   //  0x9D 157
  0x00, 0x00, 0x00, 0x00, 0x00,   //  0x9E 158
  0x00, 0x00, 0x00, 0x00, 0x00,   //  0x9F 159
  0x00, 0x00, 0x00, 0x00, 0x00,   //   0xA0 160
  0x00, 0x00, 0x00, 0x00, 0x00,   //  0xA1 161
  0x00, 0x00, 0x00, 0x00, 0x00,   //  0xA2 162
  0x00, 0x00, 0x00, 0x00, 0x00,   //  0xA3 163
  0x22, 0x1C, 0x14, 0x1C, 0x22,   //  0xA4 164
  0x7E, 0x02, 0x02, 0x02, 0x03,   //  0xA5 165
  0x00, 0x00, 0xE7, 0x00, 0x00,   //  0xA6 166
  0x4A, 0x95, 0xA5, 0xA9, 0x52,   //  0xA7 167
  0x7C, 0x55, 0x54, 0x45, 0x44,   //  0xA8 168
  0x00, 0x18, 0x24, 0x24, 0x00,   //  0xA9 169
  0x3E, 0x49, 0x49, 0x41, 0x22,   //  0xAA 170
  0x08, 0x14, 0x2A, 0x14, 0x22,   //  0xAB 171
  0x04, 0x04, 0x04, 0x04, 0x0C,   //  0xAC 172
  0x00, 0x08, 0x08, 0x08, 0x00,   //  0xAD 173
  0x00, 0x00, 0x00, 0x00, 0x00,   //  0xAE 174
  0x00, 0x45, 0x7C, 0x45, 0x00,   //  0xAF 175
  0x00, 0x04, 0x0A, 0x04, 0x00,   //  0xB0 176
  0x44, 0x44, 0x5F, 0x44, 0x44,   //  0xB1 177
  0x00, 0x41, 0x7F, 0x41, 0x00,   //  0xB2 178
  0x00, 0x00, 0x7A, 0x00, 0x00,   //  0xB3 179
  0x00, 0x78, 0x08, 0x0C, 0x00,   //  0xB4 180
  0x00, 0xFC, 0x20, 0x3C, 0x20,   //  0xB5 181
  0x0C, 0x1E, 0xFE, 0x02, 0xFE,   //  0xB6 182
  0x00, 0x18, 0x18, 0x00, 0x00,   //  0xB7 183
  0x39, 0x54, 0x54, 0x49, 0x00,   //  0xB8 184
  0x78, 0x10, 0x20, 0x7B, 0x03,   //  0xB9 185
  0x38, 0x54, 0x54, 0x44, 0x00,   //  0xBA 186
  0x22, 0x14, 0x2A, 0x14, 0x08,   //  0xBB 187
  0x00, 0x00, 0x00, 0x00, 0x00,   //  0xBC 188
  0x00, 0x00, 0x00, 0x00, 0x00,   //  0xBD 189
  0x00, 0x00, 0x00, 0x00, 0x00,   //  0xBE 190
  0x00, 0x54, 0x70, 0x44, 0x00,   //  0xBF 191
  0x7C, 0x12, 0x11, 0x12, 0x7C,   //  0xC0 192
  0x7F, 0x49, 0x49, 0x49, 0x31,   //  0xC1 193
  0x7F, 0x49, 0x49, 0x49, 0x36,   //  0xC2 194
  0x7F, 0x01, 0x01, 0x01, 0x01,   //  0xC3 195
  0x60, 0x3F, 0x21, 0x3F, 0x60,   //  0xC4 196
  0x7F, 0x49, 0x49, 0x49, 0x41,   //  0xC5 197
  0x77, 0x08, 0x7F, 0x08, 0x77,   //  0xC6 198
  0x22, 0x41, 0x49, 0x49, 0x36,   //  0xC7 199
  0x7F, 0x10, 0x08, 0x04, 0x7F,   //  0xC8 200
  0x7E, 0x10, 0x09, 0x04, 0x7E,   //  0xC9 201
  0x7F, 0x08, 0x14, 0x22, 0x41,   //  0xCA 202
  0x40, 0x3E, 0x01, 0x01, 0x7F,   //  0xCB 203
  0x7F, 0x02, 0x0C, 0x02, 0x7F,   //  0xCC 204
  0x7F, 0x08, 0x08, 0x08, 0x7F,   //  0xCD 205
  0x3E, 0x41, 0x41, 0x41, 0x3E,   //  0xCE 206
  0x7F, 0x01, 0x01, 0x01, 0x7F,   //  0xCF 207
  0x7F, 0x09, 0x09, 0x09, 0x06,   //  0xD0 208
  0x3E, 0x41, 0x41, 0x41, 0x22,   //  0xD1 209
  0x01, 0x01, 0x7F, 0x01, 0x01,   //  0xD2 210
  0x07, 0x48, 0x48, 0x48, 0x3F,   //  0xD3 211
  0x0E, 0x11, 0x7F, 0x11, 0x0E,   //  0xD4 212
  0x63, 0x14, 0x08, 0x14, 0x63,   //  0xD5 213
  0x3F, 0x20, 0x20, 0x3F, 0x60,   //  0xD6 214
  0x07, 0x08, 0x08, 0x08, 0x7F,   //  0xD7 215
  0x7F, 0x40, 0x7E, 0x40, 0x7F,   //  0xD8 216
  0x3F, 0x20, 0x3F, 0x20, 0x7F,   //  0xD9 217
  0x01, 0x7F, 0x48, 0x48, 0x30,   //  0xDA 218
  0x7F, 0x48, 0x30, 0x00, 0x7F,   //  0xDB 219
  0x00, 0x7F, 0x48, 0x48, 0x30,   //  0xDC 220
  0x22, 0x41, 0x49, 0x49, 0x3E,   //  0xDD 221
  0x7F, 0x08, 0x3E, 0x41, 0x3E,   //  0xDE 222
  0x46, 0x29, 0x19, 0x09, 0x7F,   //  0xDF 223
  0x20, 0x54, 0x54, 0x54, 0x78,   //  0xE0 224
  0x3C, 0x4A, 0x4A, 0x4A, 0x31,   //  0xE1 225
  0x7C, 0x54, 0x54, 0x28, 0x00,   //  0xE2 226
  0x7C, 0x04, 0x04, 0x0C, 0x00,   //  0xE3 227
  0x60, 0x3C, 0x24, 0x3C, 0x60,   //  0xE4 228
  0x38, 0x54, 0x54, 0x54, 0x18,   //  0xE5 229
  0x6C, 0x10, 0x7C, 0x10, 0x6C,   //  0xE6 230
  0x00, 0x44, 0x54, 0x54, 0x28,   //  0xE7 231
  0x7C, 0x20, 0x10, 0x08, 0x7C,   //  0xE8 232
  0x7C, 0x21, 0x12, 0x09, 0x7C,   //  0xE9 233
  0x7C, 0x10, 0x28, 0x44, 0x00,   //  0xEA 234
  0x40, 0x38, 0x04, 0x04, 0x7C,   //  0xEB 235
  0x7C, 0x08, 0x10, 0x08, 0x7C,   //  0xEC 236
  0x7C, 0x10, 0x10, 0x10, 0x7C,   //  0xED 237
  0x38, 0x44, 0x44, 0x44, 0x38,   //  0xEE 238
  0x7C, 0x04, 0x04, 0x04, 0x7C,   //  0xEF 239
  0x7C, 0x14, 0x14, 0x14, 0x08,   //  0xF0 240
  0x38, 0x44, 0x44, 0x44, 0x00,   //  0xF1 241
  0x04, 0x04, 0x7C, 0x04, 0x04,   //  0xF2 242
  0x0C, 0x50, 0x50, 0x50, 0x3C,   //  0xF3 243
  0x08, 0x14, 0x7C, 0x14, 0x08,   //  0xF4 244
  0x44, 0x28, 0x10, 0x28, 0x44,   //  0xF5 245
  0x3C, 0x20, 0x20, 0x3C, 0x60,   //  0xF6 246
  0x0C, 0x10, 0x10, 0x10, 0x7C,   //  0xF7 247
  0x7C, 0x40, 0x7C, 0x40, 0x7C,   //  0xF8 248
  0x3C, 0x20, 0x3C, 0x20, 0x7C,   //  0xF9 249
  0x04, 0x7C, 0x50, 0x50, 0x20,   //  0xFA 250
  0x7C, 0x50, 0x20, 0x00, 0x7C,   //  0xFB 251
  0x00, 0x7C, 0x50, 0x50, 0x20,   //  0xFC 252
  0x28, 0x44, 0x54, 0x54, 0x38,   //  0xFD 253
  0x7C, 0x10, 0x38, 0x44, 0x38,   //  0xFE 254
  0x48, 0x54, 0x34, 0x14, 0x7C    //  0xFF 255
};

//**************************************************************************************************

LedMatrix::LedMatrix(uint8_t _cs, uint8_t _din, uint8_t _clk, uint8_t _cnt) : 
  pin_cs(_cs), pin_din(_din), pin_clk(_clk), mod_cnt(_cnt), uselock(0)
{
  bitmap = (uint8_t*) malloc(_cnt << 3);

  pinMode(pin_cs, OUTPUT);
  pinMode(pin_clk, OUTPUT);
  pinMode(pin_din, OUTPUT);

  digitalWrite(pin_cs, HIGH);
  digitalWrite(pin_clk, LOW);
  digitalWrite(pin_din, LOW);

  SendCmd(LEDMATRIX_CMD_INTENSITY, 0x0F);
  SendCmd(LEDMATRIX_CMD_DECODEMODE, 0x00);
  SendCmd(LEDMATRIX_CMD_SCANLIMIT, 0x07);
  SendCmd(LEDMATRIX_CMD_DISPLAYTEST, 0x00);

  Fill(0x00);

  SendCmd(LEDMATRIX_CMD_SHUTDOWN, 0x01);
}

//**************************************************************************************************

void LedMatrix::SendCmd(uint8_t _command, uint8_t _argument)
{
  uint8_t data, value = (((uint16_t) _command) << 8) | _argument;

  digitalWrite(pin_cs, LOW);

  _command = mod_cnt;

  while(_command--)
  {
    data = value;
    _argument = 16;

    while(_argument--)
    {
      digitalWrite(pin_din, (data & 0x8000 ? HIGH : LOW));
      digitalWrite(pin_clk, HIGH);

      data <<= 1;

      digitalWrite(pin_clk, LOW);
    }
  }

  digitalWrite(pin_cs, HIGH);
}

//**************************************************************************************************

void LedMatrix::SendLine(uint8_t _line)
{
  uint8_t offset, command, modnum = mod_cnt, mask = (0x80 >> _line);
  uint16_t index = 0;

  digitalWrite(pin_cs, LOW);

  _line += LEDMATRIX_CMD_DIGIT0;

  while(modnum--)
  {
    offset = 8;
    command = _line;

    while(offset--)
    {
      digitalWrite(pin_din, (command & 0x80 ? HIGH : LOW));
      digitalWrite(pin_clk, HIGH);

      command <<= 1;

      digitalWrite(pin_clk, LOW);
    }

    offset = 8;

    while(offset--)
    {
      digitalWrite(pin_din, (bitmap[index++] & mask ? HIGH : LOW));
      digitalWrite(pin_clk, HIGH);
      digitalWrite(pin_clk, LOW);
    }
  }

  digitalWrite(pin_cs, HIGH);
}

//**************************************************************************************************

void LedMatrix::SendBitmap(void)
{
  for (uint8_t line = 0; line < 8; line++)
    SendLine(line);
}

//**************************************************************************************************

uint16_t LedMatrix::Width(void)
{
  return (((uint16_t) mod_cnt) << 3);
}

//**************************************************************************************************

uint16_t LedMatrix::Height(void)
{
  return 8;
}

//**************************************************************************************************

void LedMatrix::UpdateLock(uint8_t _uselock)
{
  if (!_uselock && uselock)
    SendBitmap();

  uselock = _uselock;
}

//**************************************************************************************************

void LedMatrix::Fill(uint8_t _color)
{
  _color = (_color ? 0xFF : 0x00);
  memset(bitmap, _color, mod_cnt << 3);

  if (uselock)
    return;

  for (byte line = 0; line < 8; line++)
    SendCmd(LEDMATRIX_CMD_DIGIT0 + line, _color);
}

//**************************************************************************************************

void LedMatrix::Scroll(uint8_t _direction, int16_t _size)
{
  if (!_size) return;

  uint16_t count = (_size > 0 ? _size : -_size);

  if (_direction == 0)
  {
    if (count < (((uint16_t) mod_cnt) << 3))
    {
      uint16_t findex, tindex;

      if (_size < 0)
      {
        tindex = 0, findex = count;
        count = (((uint16_t) mod_cnt) << 3) - count;

        while(tindex < count)
          bitmap[tindex++] = bitmap[findex++];

        count = (((uint16_t) mod_cnt) << 3);

        while(tindex < count)
          bitmap[tindex++] = 0x00;
      }
      else
      {
        tindex = (((uint16_t) mod_cnt) << 3), findex = tindex - count;

        while(tindex < count)
          bitmap[--tindex] = bitmap[--findex];

        while(count--)
          bitmap[--tindex] = 0x00;
      }

      if (!uselock)
        SendBitmap();
    }
    else Fill(0x00);
  }
  else if (_direction == 1)
  {
    if (count < 8)
    {
      uint16_t findex = 0, tindex = (((uint16_t) mod_cnt) << 3);

      while(findex < tindex)
      {
        if (_size < 0) bitmap[findex++] <<= count;
        else bitmap[findex++] >>= count;
      }

      if (!uselock)
        SendBitmap();
    }
    else Fill(0x00);
  }
}

//**************************************************************************************************

void LedMatrix::DrawPixel(int16_t _left, int16_t _top, uint8_t _color)
{
  if (_left < 0 || _left >= (((int16_t) mod_cnt) << 3) || _top < 0 || _top >= 8)
    return;

  if (_color) bitmap[_left] |= (0x80 >> _top);
  else bitmap[_left] &= ~(0x80 >> _top);

  if (!uselock) SendLine((uint8_t) _top);
}

//**************************************************************************************************

void LedMatrix::DrawChar(int16_t _left, int16_t _top, uint8_t _chr)
{
  uint16_t offset = ((uint16_t) _chr) * LEDMATRIX_FONT_WIDTH;

  uselock++;

  for (uint8_t col, x = 0; x < LEDMATRIX_FONT_WIDTH; x++)
  {
    col = pgm_read_byte(font + offset + x);

    for (uint8_t y = 0; y < LEDMATRIX_FONT_HEIGHT; y++, col >>= 1)
      DrawPixel(x + _left, y + _top, (col & 0x01 ? 1 : 0));
  }

  uselock--;

  if (!uselock)
    SendBitmap();
}

//**************************************************************************************************

void LedMatrix::DrawString(int16_t _left, int16_t _top, uint8_t* _string)
{
  uselock++;

  while(*_string)
  {
    DrawChar(_left, _top, *_string++);
    _left += LEDMATRIX_FONT_WIDTH + 1;
  }

  uselock--;

  if (!uselock)
    SendBitmap();
}

//**************************************************************************************************
