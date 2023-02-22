#pragma once

#include "BBmacros.h"
#include "Position.h"

/**
 * @brief The simpleEval function evaluates a given board, taking into account the material value of the board.
 * @param board The board to evaluate.
 * @return The value of the board.
 * @note This evaluation function is not very good, but it is a good starting point.
 */

constexpr Score mgValues[6] = { 103, 559, 595, 876, 1782, 0 };
constexpr Score egValues[6] = { 150, 567, 606, 946, 1809, 0 };


// PeSTo tables from Rofchade, then modified many times

const Score mgPawnTable[64] = {
     0,     0,     0,     0,     0,     0,     0,     0,
     2,     2,     8,    14,    12,    14,     5,    -3,
    -6,   -14,     7,    18,    18,     7,     0,   -16,
    -2,   -16,     9,    22,    22,     9,     3,    -5,
     1,    -9,     0,    12,    12,    -2,    -9,     1,
     0,    -5,   -10,    10,    10,   -11,    -5,     0,
    -4,     5,   -12,   -15,   -15,    -1,     8,    -4,
     0,     0,     0,     0,     0,     0,     0,     0,
};

const Score egPawnTable[64] = {
     0,     0,     0,     0,     0,     0,     0,     0,
    -7,    -4,     8,     0,    11,     5,    -3,   -13,
    -7,    -7,    -7,     3,     3,     2,    -4,    -2,
     5,    -1,    -5,    -2,    -9,    -8,    -7,    -6,
     8,     4,     3,    -3,    -3,    -3,    11,     7,
    21,    15,    16,    21,    23,     5,     5,    10,
     0,    -7,     9,    16,    19,    14,     3,     5,
     0,     0,     0,     0,     0,     0,     0,     0,
};
const Score mgKnightTable[64] = {
    -137 , -52 , -66 ,   6 ,   6 , -66 , -52 ,-137 ,
     -45 , -17 ,  67 ,  30 ,  30 ,  67 , -17 , -45 ,
      -2 ,  66 ,  83 ,  74 ,  74 ,  83 ,  66 ,  -2 ,
       6 ,  18 ,  44 ,  45 ,  45 ,  44 ,  18 ,   6 ,
     -10 ,  12 ,  18 ,  20 ,  20 ,  18 ,  12 , -10 ,
     -20 ,   8 ,  14 ,  14 ,  14 ,  14 ,   8 , -20 ,
     -24 , -34 ,   3 ,  -2 ,  -2 ,   3 , -34 , -24 ,
     -64 , -20 , -43 , -25 , -25 , -43 , -20 , -64 ,
};

const Score egKnightTable[64] = {
     -78 , -50 , -20 , -30 , -30 , -20 , -50 , -78 ,
     -38 , -16 , -25 ,  -6 ,  -6 , -25 , -16 , -38 ,
     -32 , -20 ,   0 ,   4 ,   4 ,   0 , -20 , -32 ,
     -18 ,   6 ,  16 ,  22 ,  22 ,  16 ,   6 , -18 ,
     -18 ,  -1 ,  16 ,  20 ,  20 ,  16 ,  -1 , -18 ,
     -22 , -12 ,  -2 ,  12 ,  12 ,  -2 , -12 , -22 ,
     -43 , -22 , -15 ,  -4 ,  -4 , -15 , -22 , -43 ,
     -46 , -50 , -20 , -18 , -18 , -20 , -50 , -46 ,
};

const Score mgBishopTable[64] = {
     -18 ,   6 , -62 , -31 , -31 , -62 ,   6 , -18 ,
     -36 ,  17 ,  20 ,   8 ,   8 ,  20 ,  17 , -36 ,
      -9 ,  37 ,  46 ,  38 ,  38 ,  46 ,  37 ,  -9 ,
      -3 ,   6 ,  28 ,  44 ,  44 ,  28 ,   6 ,  -3 ,
      -1 ,  12 ,  12 ,  30 ,  30 ,  12 ,  12 ,  -1 ,
       5 ,  16 ,  21 ,  14 ,  14 ,  21 ,  16 ,   5 ,
       2 ,  24 ,  18 ,   4 ,   4 ,  18 ,  24 ,   2 ,
     -27 , -21 , -13 , -17 , -17 , -13 , -21 , -27 ,
};

const Score egBishopTable[64] = {
     -19 , -19 , -10 ,  -8 ,  -8 , -10 , -19 , -19 ,
     -11 ,  -4 ,  -3 ,  -8 ,  -8 ,  -3 ,  -4 , -11 ,
       3 ,  -4 ,   3 ,  -2 ,  -2 ,   3 ,  -4 ,   3 ,
       0 ,   6 ,  11 ,  12 ,  12 ,  11 ,   6 ,   0 ,
      -8 ,   0 ,  12 ,  13 ,  13 ,  12 ,   0 ,  -8 ,
     -14 ,  -5 ,   6 ,  12 ,  12 ,   6 ,  -5 , -14 ,
     -20 , -16 ,  -8 ,   2 ,   2 ,  -8 , -16 , -20 ,
     -20 ,  -7 , -20 ,  -7 ,  -7 , -20 ,  -7 , -20 ,
};

const Score mgRookTable[64] = {
      38 ,  36 ,  20 ,  57 ,  57 ,  20 ,  36 ,  38 ,
      36 ,  29 ,  62 ,  71 ,  71 ,  62 ,  29 ,  36 ,
       6 ,  40 ,  36 ,  26 ,  26 ,  36 ,  40 ,   6 ,
     -22 , -10 ,  21 ,  25 ,  25 ,  21 , -10 , -22 ,
     -30 , -10 , -10 ,   4 ,   4 , -10 , -10 , -30 ,
     -39 , -15 ,  -8 ,  -7 ,  -7 ,  -8 , -15 , -39 ,
     -58 , -11 ,  -4 ,  -5 ,  -5 ,  -4 , -11 , -58 ,
     -22 , -25 ,   4 ,  16 ,  16 ,   4 , -25 , -22 ,
};

const Score egRookTable[64] = {
   9 ,   9 ,  15 ,  14 ,  14 ,  15 ,   9 ,   9 ,
   7 ,  10 ,   8 ,   4 ,   4 ,   8 ,  10 ,   7 ,
   2 ,   1 ,   2 ,   4 ,   4 ,   2 ,   1 ,   2 ,
   3 ,   1 ,   7 ,   2 ,   2 ,   7 ,   1 ,   3 ,
  -4 ,  -2 ,   1 ,   0 ,   0 ,   1 ,  -2 ,  -4 ,
 -10 ,  -4 ,  -8 ,  -4 ,  -4 ,  -8 ,  -4 , -10 ,
  -4 ,  -8 ,  -4 ,  -4 ,  -4 ,  -4 ,  -8 ,  -4 ,
 -14 ,   3 ,  -5 ,  -3 ,  -3 ,  -5 ,   3 , -14 ,
};

const Score mgQueenTable[64] = {
   8 ,  22 ,  36 ,  36 ,  36 ,  36 ,  22 ,   8 ,
  15 ,  -6 ,  26 ,  -8 ,  -8 ,  26 ,  -6 ,  15 ,
  22 ,  15 ,  32 ,  18 ,  18 ,  32 ,  15 ,  22 ,
 -13 , -14 ,   0 ,  -8 ,  -8 ,   0 , -14 , -13 ,
  -6 , -12 ,  -6 ,  -6 ,  -6 ,  -6 , -12 ,  -6 ,
  -4 ,   8 ,  -4 ,  -4 ,  -4 ,  -4 ,   8 ,  -4 ,
 -17 ,  -6 ,  13 ,   5 ,   5 ,  13 ,  -6 , -17 ,
 -26 , -24 , -17 ,  -2 ,  -2 , -17 , -24 , -26 ,
};

const Score egQueenTable[64] = {
   6 ,  16 ,  20 ,  27 ,  27 ,  20 ,  16 ,   6 ,
  -8 ,  25 ,  28 ,  50 ,  50 ,  28 ,  25 ,  -8 ,
  -6 ,  12 ,  22 ,  48 ,  48 ,  22 ,  12 ,  -6 ,
  20 ,  40 ,  32 ,  51 ,  51 ,  32 ,  40 ,  20 ,
   2 ,  34 ,  26 ,  39 ,  39 ,  26 ,  34 ,   2 ,
  -6 ,  -8 ,  16 ,   8 ,   8 ,  16 ,  -8 ,  -6 ,
 -27 , -30 , -26 , -16 , -16 , -26 , -30 , -27 ,
 -37 , -24 , -27 , -24 , -24 , -27 , -24 , -37 ,
};

const Score mgKingTable[64] = {
    44, 66, 33, -1, -1, 33, 66, 44,
    66, 90, 48, 24, 24, 48, 90, 66,
    92, 108, 60, 23, 23, 60, 108, 92,
    115, 134, 78, 52, 52, 78, 134, 115,
    123, 142, 103, 73, 73, 103, 142, 123,
    146, 193, 126, 90, 90, 126, 193, 146,
    208, 227, 175, 134, 134, 175, 227, 208,
    203, 245, 203, 148, 148, 203, 245, 203 };

const Score egKingTable[64] = {
     0,    33,    63,    57,    57,    63,    33,     0,
    39,    75,    99,   101,   101,    99,    75,    39,
    66,    97,   126,   131,   131,   126,    97,    66,
    77,   117,   129,   129,   129,   129,   117,    77,
    72,   124,   149,   149,   149,   149,   124,    72,
    69,   129,   138,   143,   143,   138,   129,    69,
    35,    90,    87,    98,    98,    87,    90,    35,
     8,    44,    54,    58,    58,    54,    44,     8,
};

extern Score* mgPestoTables[6];

extern Score* egPestoTables[6];

constexpr Score gamephaseInc[12] = { 0, 1, 1, 2, 4, 0, 0, 1, 1, 2, 4, 0 };
extern Score mgTables[12][64];
extern Score egTables[12][64];

constexpr Score knightMobMg[9]  = { -39, -34,  -8,  -2,   2,   8,  13,  18,  24 };
constexpr Score bishopMobMg[14] = { -30, -13,   9,  18,  25,  34,  34,  38,  39,  44,  50,  53,  58,  61, };
constexpr Score rookMobMg[15]   = { -38, -15,   0,   2,   3,   9,  13,  19,  26,  26,  26,  29,  36,  37,  43 };
constexpr Score queenMobMg[28]  = { -18, -10,  -5,  -5,  11,  16,  15,  24,  26,  34,  41,  43,  44,  45,  45,  45,  45,  46,  47,  48,  57,  66,  67,  67,  71,  73,  73,  76 };

constexpr Score knightMobEg[9]  = { -50, -36, -20, -11,   4,   8,  10,  13,  17 };
constexpr Score bishopMobEg[14] = { -38, -16,  -5,   8,  13,  25,  36,  37,  41,  46,  50,  55,  56,  62 };
constexpr Score rookMobEg[15]   = { -52, -10,  11,  27,  46,  64,  65,  78,  85,  88,  97, 102, 105, 108, 111 };
constexpr Score queenMobEg[28]  = { -31, -18,  -5,  11,  25,  34,  38,  46,  48,  60,  60,  64,  79,  81,  84,  85,  87,  89,  94,  95,  97, 108, 109, 109, 113, 118, 119, 141 };

/**
 * @brief The initTables function initializes the material - positional tables
 * @note This function is called once at the beginning of the program
 */
void initTables();

/**
 * @brief The pestoEval function evaluates the material and positional scores
 * @param pos The position to evaluate
 * @return The material and positional scores
 */
Score pestoEval(Position* pos);

/** 
 * @brief The initTropism function initializes the tropism tables
 * @note This is called at the start of the program 
 */
void initTropism();
