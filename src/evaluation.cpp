
#include "evaluation.h"
#include "tables.h"
#include "tt.h"
#include "movegen.h"
#include "types.h"
#include <iostream>
#include <vector>
#include <cstring>
#include <fstream>

Score mgTables[12][64] = { {0} };
Score egTables[12][64] = { {0} };

Score* mgPestoTables[6] = {
    (Score*)mgPawnTable,
    (Score*)mgKnightTable,
    (Score*)mgBishopTable,
    (Score*)mgRookTable,
    (Score*)mgQueenTable,
    (Score*)mgKingTable };

Score* egPestoTables[6] = {
    (Score*)egPawnTable,
    (Score*)mgKnightTable,
    (Score*)mgBishopTable,
    (Score*)mgRookTable,
    (Score*)mgQueenTable,
    (Score*)egKingTable };

void initTables() {

    for (Piece piece = P; piece <= K; piece++) {
        for(Square square = a8; square < noSquare; square++) {
            // White
            mgTables[piece][square] = mgPestoTables[piece][square] + mgValues[piece];
            egTables[piece][square] = egPestoTables[piece][square] + egValues[piece];
            // Black
            mgTables[piece + 6][square] = mgPestoTables[piece][flipSquare(square)] + mgValues[piece];
            egTables[piece + 6][square] = egPestoTables[piece][flipSquare(square)] + egValues[piece];
        }
    }
}



constexpr Score DOUBLEDEARLYMG = 11;
constexpr Score DOUBLEDEARLYEG = 4;

// (MG)
constexpr Score DOUBLEISOLATEDPENMG = 10;
constexpr Score ISOLATEDPENMG = 17;
constexpr Score BACKWARDPENMG = 2;
constexpr Score DOUBLEDPENMG = 15;
constexpr Score SUPPORTEDPHALANXMG = 1;
constexpr Score ADVANCABLEPHALANXMG = 10;
constexpr Score R_SUPPORTEDPHALANXMG = 4;
constexpr Score R_ADVANCABLEPHALANXMG = 1;
constexpr Score passedRankBonusMg [7] = {0, -8, -35, -28, 8, 13, 86, };
constexpr Score PASSEDPATHBONUSMG = -1;
constexpr Score SUPPORTEDPASSERMG = 32;
constexpr Score INNERSHELTERMG = 30;
constexpr Score OUTERSHELTERMG = 26;
constexpr Score BISHOPPAIRMG = 31;
constexpr Score ROOKONOPENFILEMG = 24;
constexpr Score ROOKONSEMIOPENFILEMG = 17;
constexpr Score KNIGHTONEXTOUTPOSTMG = 24;
constexpr Score BISHOPONEXTOUTPOSTMG = 34;
constexpr Score KNIGHTONINTOUTPOSTMG = 23;
constexpr Score BISHOPONINTOUTPOSTMG = 45;
constexpr Score BISHOPPAWNSMG = 1;
constexpr Score THREATSAFEPAWNMG = 61;
constexpr Score THREATPAWNPUSHMG = 23;
constexpr Score PAWNHANGINGMG = 4;
constexpr Score NONPAWNHANGINGMG = -29;
constexpr Score KINGTHREATMG = -2;
constexpr Score TEMPOMG = 27;
// (EG)
constexpr Score DOUBLEISOLATEDPENEG = 50;
constexpr Score ISOLATEDPENEG = 19;
constexpr Score BACKWARDPENEG = 17;
constexpr Score DOUBLEDPENEG = 19;
constexpr Score SUPPORTEDPHALANXEG = 6;
constexpr Score ADVANCABLEPHALANXEG = 24;
constexpr Score R_SUPPORTEDPHALANXEG = 9;
constexpr Score R_ADVANCABLEPHALANXEG = 17;
constexpr Score passedRankBonusEg [7] = {0, -71, -48, -2, 38, 129, 192, };
constexpr Score PASSEDPATHBONUSEG = 13;
constexpr Score SUPPORTEDPASSEREG = 1;
constexpr Score INNERSHELTEREG = -11;
constexpr Score OUTERSHELTEREG = -1;
constexpr Score BISHOPPAIREG = 103;
constexpr Score ROOKONOPENFILEEG = -2;
constexpr Score ROOKONSEMIOPENFILEEG = 17;
constexpr Score KNIGHTONEXTOUTPOSTEG = 33;
constexpr Score BISHOPONEXTOUTPOSTEG = 0;
constexpr Score KNIGHTONINTOUTPOSTEG = 33;
constexpr Score BISHOPONINTOUTPOSTEG = -6;
constexpr Score BISHOPPAWNSEG = -3;
constexpr Score THREATSAFEPAWNEG = 39;
constexpr Score THREATPAWNPUSHEG = 26;
constexpr Score PAWNHANGINGEG = -58;
constexpr Score NONPAWNHANGINGEG = -27;
constexpr Score KINGTHREATEG = 9;
constexpr Score TEMPOEG = 27;


constexpr Score KNIGHTATTACKOUTERRING = 9;
constexpr Score KNIGHTATTACKINNERRING = 11;
constexpr Score BISHOPATTACKOUTERRING = 8;
constexpr Score BISHOPATTACKINNERRING = 11;
constexpr Score ROOKATTACKOUTERRING = 12;
constexpr Score ROOKATTACKINNERRING = 17;
constexpr Score QUEENATTACKOUTERRING = 24;
constexpr Score QUEENATTACKINNERRING = 34;

constexpr Score NOQUEENDANGER = -65;
constexpr Score PINNEDSHELTERDANGER = 11;

constexpr Score OWNPIECEBLOCKEDPAWNMG = 0;
constexpr Score OWNPIECEBLOCKEDPAWNEG = 8;



constexpr Score WEAKUNOPPOSEDMG = 9;
constexpr Score WEAKUNOPPOSEDPENEG = 11;

constexpr Score WEAKLEVEREG = 36;

constexpr Score MG_PAWN_PASSER_BONUS = 3;
constexpr Score EG_PAWN_PASSER_BONUS = 12;

constexpr Score SUPPORTEDCONNECTEDMG = 21;

constexpr Score ROOKONQUEENFILEMG = 4;
constexpr Score ROOKONQUEENFILEEG = 8;

constexpr Score MG_ROOKCLOSEDFILE = 6;
constexpr Score EG_ROOKCLOSEDFILE = 3;
constexpr Score MG_ROOKSEMIOPENFILE = 13;
constexpr Score MG_ROOKOPENFILE = 20;
constexpr Score MG_ROOKCLEARFILE = 1;
constexpr Score EG_ROOKSEMIOPENFILE = 5;
constexpr Score EG_ROOKOPENFILE = 15;
constexpr Score EG_ROOKCLEARFILE = 1;
constexpr Score MG_ROOKSEVENTH = 2;
constexpr Score EG_ROOKSEVENTH = 1;
constexpr Score TRAPPEDROOKMG = 35;
constexpr Score TRAPPEDROOKEG = 8;

constexpr Score MG_KINGINREARSPAN = 4;
constexpr Score EG_KINGINREARSPAN = 1;

constexpr Score MG_PAWNMINORSUPPORTED = 11;
constexpr Score EG_PAWNMINORSUPPORTED = 2;
constexpr Score MG_MINORBEHINDPAWN = 11;
constexpr Score EG_MINORBEHINDPAWN = 2;

constexpr Score MG_KNIGHTKINGPROTECTOR = 6;
constexpr Score MG_BISHOPKINGPROTECTOR = 4;
constexpr Score EG_KNIGHTKINGPROTECTOR = 6;
constexpr Score EG_BISHOPKINGPROTECTOR = 6;

constexpr Score PAWNTHREATMG = 99;
constexpr Score PAWNTHREATEG = 50;

constexpr Score PAWNSHELTERVALUE = 5;
constexpr Score KNIGHTSHELTERVALUE = 2;
constexpr Score BISHOPSHELTERVALUE = 1;

constexpr Score OPENFILEONKINGMG = -18;
constexpr Score OPENFILEONKINGEG = -12;

constexpr Score PAWNSTORM = 1;

#define MOBILITYEVAL true
#define PAWNEVAL true

Score noutpostEg[5] = { 2, 6, 12, 14, 18 };
Score noutpostMg[5] = { 2, 4, 8, 9, 11 };
Score boutpostEg[5] = { 2, 5, 10, 8, 3 };
Score boutpostMg[5] = { 2, 3, 6, 4, 2 };

// Sf formula, tuned parameters
static inline constexpr Score connectedBonus(U8 rank, bool op, bool ph, bool su) {
    return connectedSeed[rank] * (2 + ph - op) + SUPPORTEDCONNECTEDMG * su;
}

static inline constexpr Score knightOutpostMg(BitBoard pAttacks, U8 pSupport){
    return pAttacks ? 0 : noutpostMg[pSupport];
}

static inline constexpr Score knightOutpostEg(BitBoard pAttacks, U8 pSupport) {
    return pAttacks ? 0 : noutpostEg[pSupport];
}

static inline constexpr Score bishopOutpostMg(BitBoard pAttacks, U8 pSupport) {
    return pAttacks ? 0 : boutpostMg[pSupport];
}

static inline constexpr Score bishopOutpostEg(BitBoard pAttacks, U8 pSupport) {
    return pAttacks ? 0 : boutpostEg[pSupport];
}

static inline constexpr BitBoard centralFiles = files(2) | files(3) | files(4) | files(5);

template <Piece pt>
//(bb, occ, pinned, mobilityArea, mobilityScore);
static inline void getMobilityFeat(const BitBoard (&bb)[12], BitBoard occCheck, const BitBoard pinned, const BitBoard mob, BitBoard &attackedByUs, BitBoard &ourMultiAttacks, S8 *features, const BitBoard kingRing, const BitBoard kingOuter, S32 &innerAttacks, S32 &outerAttacks) {
    constexpr Side us = pt < p ? WHITE : BLACK;
    // constexpr Side them = us ? BLACK : WHITE;
    BitBoard pieces = bb[pt] & ~pinned;

    // Consider diagonal xrays through our queens
    if constexpr (pt == B || pt == Q)                   occCheck ^= bb[Q];
    else if constexpr (pt == b || pt == q)              occCheck ^= bb[q];

    // Consider cross xrays through our rooks
    if constexpr (pt == R || pt == Q)                   occCheck ^= bb[R];
    else if constexpr (pt == r || pt == q)              occCheck ^= bb[r];
    
    // Iterate over all pieces of the given type
    
    while (pieces) {
        BitBoard moves;
        BitBoard mobMoves;
        Square sq = popLsb(pieces);
        if constexpr (pt == N || pt == n) {
            moves = knightAttacks[sq];
            mobMoves = moves & mob;
            U8 moveCount = popcount(mobMoves);
            features[moveCount] += us == WHITE ? 1 : -1;
        }
        else if constexpr (pt == B || pt == b) { // X-ray through our queens
            moves = getBishopAttack(sq, occCheck);
            mobMoves = moves & mob;
            U8 moveCount = popcount(mobMoves);
            features[moveCount] += us == WHITE ? 1 : -1;
        }
        else if constexpr (pt == R || pt == r) { // X-ray through our queens and rooks
            moves = getRookAttack(sq, occCheck);
            mobMoves = moves & mob;
            U8 moveCount = popcount(mobMoves);
            features[moveCount] += us == WHITE ? 1 : -1;
        }
        else if constexpr (pt == Q || pt == q) { // X-ray through our queens
            moves = getQueenAttack(sq, occCheck);
            mobMoves = moves & mob;
            U8 moveCount = popcount(mobMoves);
            features[moveCount] += us == WHITE ? 1 : -1;
        }
        innerAttacks += popcount(mobMoves & kingRing);
        outerAttacks += popcount(mobMoves & kingOuter);
        ourMultiAttacks |= attackedByUs & moves;
        attackedByUs |= moves;
    }
} 

template <Piece pt>
//(bb, occ, pinned, mobilityArea, mobilityScore);
static inline void mobility(const BitBoard *bb, BitBoard occCheck, const BitBoard pinned, const BitBoard mobilityArea, Score (&mobilityScore)[2], BitBoard &attackedByUs, BitBoard &ourMultiAttacks, const BitBoard kingRing, const BitBoard kingOuter, S32 &innerAttacks, S32 &outerAttacks){
    // constexpr Side them = us ? BLACK : WHITE;
    BitBoard pieces = bb[pt] & ~pinned; // TODO: try to exclude xray through pinned pieces

    // Consider diagonal xrays through our queens
    if constexpr (pt == B || pt == Q)                   occCheck ^= bb[Q];

    // Consider cross xrays through our rooks
    if constexpr (pt == R || pt == Q)                   occCheck ^= bb[R];
    
    // Iterate over all pieces of the given type
    
    while (pieces) {
        BitBoard moves;
        BitBoard mobMoves;
        Square sq = popLsb(pieces);
        if constexpr (pt == N) {
            moves = knightAttacks[sq];
            mobMoves = moves & mobilityArea;
            U8 moveCount = popcount(mobMoves);
            // Add the mobility score
            mobilityScore[0] += knightMobMg[moveCount];
            mobilityScore[1] += knightMobEg[moveCount];
            innerAttacks += popcount(mobMoves & kingRing) * KNIGHTATTACKINNERRING;
            outerAttacks += popcount(mobMoves & kingOuter) * KNIGHTATTACKOUTERRING;
        }
        else if constexpr (pt == B) {
            moves = getBishopAttack(sq, occCheck);
            mobMoves = moves & mobilityArea;
            U8 moveCount = popcount(mobMoves); // X-ray through our queens
            // Add the mobility score
            mobilityScore[0] += bishopMobMg[moveCount];
            mobilityScore[1] += bishopMobEg[moveCount];
            innerAttacks += popcount(mobMoves & kingRing) * BISHOPATTACKINNERRING;
            outerAttacks += popcount(mobMoves & kingOuter) * BISHOPATTACKOUTERRING;
        }
        else if constexpr (pt == R) {
            moves = getRookAttack(sq, occCheck);
            mobMoves = moves & mobilityArea;
            U8 moveCount = popcount(mobMoves); // X-ray through our queens and rooks
            // Add the mobility score
            mobilityScore[0] += rookMobMg[moveCount];
            mobilityScore[1] += rookMobEg[moveCount];
            innerAttacks += popcount(mobMoves & kingRing) * ROOKATTACKINNERRING;
            outerAttacks += popcount(mobMoves & kingOuter) * ROOKATTACKOUTERRING;
        }
        else if constexpr (pt == Q) {
            moves = getQueenAttack(sq, occCheck);
            mobMoves = moves & mobilityArea;
            U8 moveCount = popcount(mobMoves); // X-ray through our queens
            // Add the mobility score
            mobilityScore[0] += queenMobMg[moveCount];
            mobilityScore[1] += queenMobEg[moveCount];
            innerAttacks += popcount(mobMoves & kingRing) * QUEENATTACKINNERRING;
            outerAttacks += popcount(mobMoves & kingOuter) * QUEENATTACKOUTERRING;   
        }
        ourMultiAttacks |= attackedByUs & mobMoves;
        attackedByUs |= mobMoves;
    }
} 

static inline BitBoard filesFromBB(BitBoard bb){
    bb |= bb >> 8 | bb << 8;
    bb |= bb >> 16 | bb << 16;
    bb |= bb >> 32 | bb << 32;
    return bb;
}

template <bool side>
static inline BitBoard advancePathMasked(BitBoard bb, BitBoard mask){
    if constexpr (side == WHITE){
        bb |= (bb >> 8) & mask;
        bb |= (bb >> 16) & mask;
        bb |= (bb >> 32) & mask;
    }
    else {
        bb |= (bb << 8) & mask;
        bb |= (bb << 16) & mask;
        bb |= (bb << 32) & mask;
    }
    return bb;
}

template <bool side>
static inline BitBoard makePawnAttacks(BitBoard pawns){
    if constexpr (side == WHITE) return ((pawns & notFile(0)) >> 9) | ((pawns & notFile(7)) >> 7);
    else                         return ((pawns & notFile(0)) << 7) | ((pawns & notFile(7)) << 9);
}

template <bool side>
static inline BitBoard pawnSpanPawns(BitBoard movers, BitBoard blockers){
    if constexpr (side == WHITE){
        return makePawnAttacks<WHITE>(advancePathMasked<WHITE>(movers, ~blockers)) & ~blockers;
    }
    else {
        return makePawnAttacks<BLACK>(advancePathMasked<BLACK>(movers, ~blockers)) & ~blockers;
    }
}

Score pestoEval(Position *pos){
    auto const& bb = pos->bitboards;
    auto const& occ = pos->occupancies;
    // Setup the game phase
    S32 gamePhase = gamephaseInc[P] * popcount(bb[P] | bb[p]) +
                    gamephaseInc[N] * popcount(bb[N] | bb[n]) +
                    gamephaseInc[B] * popcount(bb[B] | bb[b]) +
                    gamephaseInc[R] * popcount(bb[R] | bb[r]) +
                    gamephaseInc[Q] * popcount(bb[Q] | bb[q]) +
                    gamephaseInc[K] * popcount(bb[K] | bb[k]);
    gamePhase = std::min(gamePhase, (S32)24); // If we have a lot of pieces, we don't want to go over 24

    Score mgScore = 0, egScore = 0;
    const Square whiteKing = lsb(bb[K]);
    const Square blackKing = lsb(bb[k]);

    // Mobility calculations

    // Pin, mobility and threat calculations
    BitBoard attackedBy[2] = {
        kingAttacks[whiteKing],
        kingAttacks[blackKing]
    };

    BitBoard pawnAttackedSquares[2] = {
        0,
        0
    };


    BitBoard multiAttacks[2] = {
        0ULL,
        0ULL
    };
    
    {
        // Left Attacks
        BitBoard sidePawnAttacks[2] = {
            ((bb[P] & notFile(0)) >> 9),
            ((bb[p] & notFile(0)) << 7)
        }; 
        // Update Multi attacks
        multiAttacks[WHITE] |= attackedBy[WHITE] & sidePawnAttacks[WHITE];
        multiAttacks[BLACK] |= attackedBy[BLACK] & sidePawnAttacks[BLACK];
        // Update Normal attacks
        attackedBy[WHITE] |= sidePawnAttacks[WHITE];
        attackedBy[BLACK] |= sidePawnAttacks[BLACK];
        // Update exclusive pawn attacks
        pawnAttackedSquares[WHITE] |= sidePawnAttacks[WHITE];
        pawnAttackedSquares[BLACK] |= sidePawnAttacks[BLACK];
        // Right Attacks
        sidePawnAttacks[WHITE] = ((bb[P] & notFile(7)) >> 7); 
        sidePawnAttacks[BLACK] = ((bb[p] & notFile(7)) << 9);
        // Update multi attacks
        multiAttacks[WHITE] |= attackedBy[WHITE] & sidePawnAttacks[WHITE];
        multiAttacks[BLACK] |= attackedBy[BLACK] & sidePawnAttacks[BLACK];
        // Update Normal attacks
        attackedBy[WHITE] |= sidePawnAttacks[WHITE];
        attackedBy[BLACK] |= sidePawnAttacks[BLACK];
        // Update exclusive pawn attacks
        pawnAttackedSquares[WHITE] |= sidePawnAttacks[WHITE];
        pawnAttackedSquares[BLACK] |= sidePawnAttacks[BLACK];
    }

    // Pinned mask
    BitBoard RQmask[2] = {
        bb[R] | bb[Q],
        bb[r] | bb[q]
    };
    BitBoard BQmask[2] = {
        bb[B] | bb[Q],
        bb[b] | bb[q]
    };
    BitBoard pinned[2] = {
        getPinnedPieces(occ[BOTH], occ[WHITE], whiteKing, RQmask[BLACK], BQmask[BLACK]),
        getPinnedPieces(occ[BOTH], occ[BLACK], blackKing, RQmask[WHITE], BQmask[WHITE])
    };

    const BitBoard blockedPawns[2] = {
        bb[P] & (occ[BOTH] >> 8),
        bb[p] & (occ[BOTH] << 8)
    };

    BitBoard underDevelopedPawns[2] = {
        bb[P] & (ranks(6) | ranks(5)),
        bb[p] & (ranks(1) | ranks(2))
    };

    const BitBoard mobilityArea[2] = {
        // Mobility area.
        // Squares in the mobility area are:
        // 1. Free of our pieces
        // 2. Not defended by enemy pawns

        // Calculate a negative mask for the mobility area, and then invert it
        (blockedPawns[WHITE] | underDevelopedPawns[WHITE] | pawnAttackedSquares[BLACK] | pinned[WHITE] | bb[K] | bb[Q]) ^ 0xFFFFFFFFFFFFFFFF,
        (blockedPawns[BLACK] | underDevelopedPawns[BLACK] | pawnAttackedSquares[WHITE] | pinned[BLACK] | bb[k] | bb[q]) ^ 0xFFFFFFFFFFFFFFFF
    };

    // Calculate the mobility scores (index by phase and color)
    Score mobilityScore[2] = {0,0};
    const BitBoard pawnSpan[2] = {
        advancePathMasked<WHITE>(bb[P], ~(bb[p] | pawnAttackedSquares[BLACK])),
        advancePathMasked<BLACK>(bb[p], ~(bb[P] | pawnAttackedSquares[WHITE]))
    };

    const BitBoard outpostSquares[2] = {
        (ranks(2) | ranks(3) | ranks(4)) & pawnAttackedSquares[WHITE] & ~(makePawnAttacks<BLACK>(pawnSpan[BLACK]) | bb[p]) & notFile(0) & notFile(7),
        (ranks(5) | ranks(4) | ranks(3)) & pawnAttackedSquares[BLACK] & ~(makePawnAttacks<WHITE>(pawnSpan[WHITE]) | bb[P]) & notFile(0) & notFile(7)
    };

    const BitBoard nonPawns[2] = {
        bb[N] | bb[B] | RQmask[WHITE],
        bb[n] | bb[b] | RQmask[BLACK],
    };

    const BitBoard kingRing[2] = {
        kingAttacks[whiteKing],
        kingAttacks[blackKing]
    };

    const BitBoard kingOuter[2] = {
        fiveSquare[whiteKing],
        fiveSquare[blackKing]
    };

    S32 innerAttacks[2] = {0,0};
    S32 outerAttacks[2] = {0,0};

    // Black mobility
    mobility<N>(bb+6, occ[BOTH], pinned[BLACK], mobilityArea[BLACK], mobilityScore, attackedBy[BLACK], multiAttacks[BLACK], kingRing[WHITE], kingOuter[WHITE], innerAttacks[BLACK], outerAttacks[BLACK]);
    mobility<B>(bb+6, occ[BOTH], pinned[BLACK], mobilityArea[BLACK], mobilityScore, attackedBy[BLACK], multiAttacks[BLACK], kingRing[WHITE], kingOuter[WHITE], innerAttacks[BLACK], outerAttacks[BLACK]);
    mobility<R>(bb+6, occ[BOTH], pinned[BLACK], mobilityArea[BLACK], mobilityScore, attackedBy[BLACK], multiAttacks[BLACK], kingRing[WHITE], kingOuter[WHITE], innerAttacks[BLACK], outerAttacks[BLACK]);
    mobility<Q>(bb+6, occ[BOTH], pinned[BLACK], mobilityArea[BLACK], mobilityScore, attackedBy[BLACK], multiAttacks[BLACK], kingRing[WHITE], kingOuter[WHITE], innerAttacks[BLACK], outerAttacks[BLACK]);

    // Flip
    mobilityScore[0] *= -1;
    mobilityScore[1] *= -1;

    // White mobility
    mobility<N>(bb, occ[BOTH], pinned[WHITE], mobilityArea[WHITE], mobilityScore, attackedBy[WHITE], multiAttacks[WHITE], kingRing[BLACK], kingOuter[BLACK], innerAttacks[WHITE], outerAttacks[WHITE]);
    mobility<B>(bb, occ[BOTH], pinned[WHITE], mobilityArea[WHITE], mobilityScore, attackedBy[WHITE], multiAttacks[WHITE], kingRing[BLACK], kingOuter[BLACK], innerAttacks[WHITE], outerAttacks[WHITE]);
    mobility<R>(bb, occ[BOTH], pinned[WHITE], mobilityArea[WHITE], mobilityScore, attackedBy[WHITE], multiAttacks[WHITE], kingRing[BLACK], kingOuter[BLACK], innerAttacks[WHITE], outerAttacks[WHITE]);
    mobility<Q>(bb, occ[BOTH], pinned[WHITE], mobilityArea[WHITE], mobilityScore, attackedBy[WHITE], multiAttacks[WHITE], kingRing[BLACK], kingOuter[BLACK], innerAttacks[WHITE], outerAttacks[WHITE]);

    // Weak pieces
    BitBoard weakPieces[2] = {
        occ[WHITE] & ~pawnAttackedSquares[WHITE] & ((attackedBy[BLACK] & ~attackedBy[WHITE]) | (multiAttacks[BLACK] & ~multiAttacks[WHITE])),
        occ[BLACK] & ~pawnAttackedSquares[BLACK] & ((attackedBy[WHITE] & ~attackedBy[BLACK]) | (multiAttacks[WHITE] & ~multiAttacks[BLACK]))
    };

    // Hanging enemies
    BitBoard hanging[2] = {
        weakPieces[WHITE] & (~attackedBy[WHITE] | (nonPawns[WHITE] & multiAttacks[BLACK])),
        weakPieces[BLACK] & (~attackedBy[BLACK] | (nonPawns[BLACK] & multiAttacks[WHITE]))
    };

    // King Threats (eg op-ness hopefully) (also, cap to 1)
    BitBoard kingThreats[2] = {
        weakPieces[BLACK] & kingAttacks[WHITE],
        weakPieces[WHITE] & kingAttacks[BLACK]
    };

    // Malus for directly doubled, undefended pawns
    const BitBoard protectedPawns[2] = {
        bb[P] & pawnAttackedSquares[WHITE],
        bb[p] & pawnAttackedSquares[BLACK]
    };

    const BitBoard pawnBlockage[2] = {
        bb[p] | bb[P] | pawnAttackedSquares[BLACK],
        bb[P] | bb[p] | pawnAttackedSquares[WHITE]
    };

    BitBoard doubledPawns[2] = { bb[P] & (bb[P] << 8), bb[p] & (bb[p] >> 8) };
    BitBoard pawnFiles[2] = { filesFromBB(bb[P]), filesFromBB(bb[p]) };
    // White pawns
    BitBoard pieces = bb[P];
    while (pieces){
        // Evaluate isolated, backward, doubled pawns and connected pawns
        const Square sq = popLsb(pieces);
        const BitBoard sqb = squareBB(sq);
        const U8 rank = 7 - rankOf(sq);
        bool doubled = doubledPawns[WHITE] & squareBB(sq);
        bool isolated = !(isolatedPawnMask[sq] & bb[P]);
        bool pawnOpposed = !(pawnFiles[BLACK] & sqb);
        bool supported = protectedPawns[WHITE] & sqb;
        bool advancable = !(pawnBlockage[WHITE] & north(sqb));
        bool phal = phalanx[sq] & bb[P];
        bool candidatePassed = !(passedPawnMask[WHITE][sq] & bb[p]);
        
        bool backward = !( // If the pawn
            (backwardPawnMask[WHITE][sq] & bb[P]) // If the pawn is doesn't have behind or adjacent pawns in adjacent files
            || advancable // If the pawn can't advance (blocked by enemy pawn or enemy pawn capture square)
        );
        // Check if the pawn is doubled
        if (isolated){
            if (doubled && pawnOpposed){
                mgScore -= DOUBLEISOLATEDPENMG;
                egScore -= DOUBLEISOLATEDPENEG;
            }
            else{
                mgScore -= ISOLATEDPENMG;
                egScore -= ISOLATEDPENEG;
            }
        }
        else if (backward){
            mgScore -= BACKWARDPENMG;
            egScore -= BACKWARDPENEG;
        }
        if (doubled && !supported){
            mgScore -= DOUBLEDPENMG;
            egScore -= DOUBLEDPENEG;
        }
        else if (supported && phal){
            mgScore += SUPPORTEDPHALANXMG;
            mgScore += R_SUPPORTEDPHALANXMG * (rank - 2);
            egScore += SUPPORTEDPHALANXEG;
            egScore += R_SUPPORTEDPHALANXEG * (rank - 2);
        }
        else if (advancable && phal){
            mgScore += ADVANCABLEPHALANXMG;
            mgScore += R_ADVANCABLEPHALANXMG * (rank - 2);
            egScore += ADVANCABLEPHALANXEG;
            egScore += R_ADVANCABLEPHALANXEG * (rank - 2);
        }
        if (candidatePassed){
            BitBoard passedPath = advancePathMasked<WHITE>(sqb, ~bb[BOTH]);
            // Give bonus for how close the pawn is to the promotion square
            mgScore += passedRankBonusMg[rank];
            egScore += passedRankBonusEg[rank];
            // Give bonus for how many squares the pawn can advance
            mgScore += popcount(passedPath) * PASSEDPATHBONUSMG;
            egScore += popcount(passedPath) * PASSEDPATHBONUSEG;
            // Bonus for connected or supported passed pawns
            if (supported){
                mgScore += SUPPORTEDPASSERMG;
                egScore += SUPPORTEDPASSEREG;
            }
        }
    }

    // Black pawns
    pieces = bb[p];
    while (pieces){
        // Evaluate isolated, backward, doubled pawns and connected pawns
        const Square sq = popLsb(pieces);
        const BitBoard sqb = squareBB(sq);
        const U8 rank = rankOf(sq);
        bool doubled = doubledPawns[BLACK] & squareBB(sq);
        bool isolated = !(isolatedPawnMask[sq] & bb[p]);
        bool pawnOpposed = !(pawnFiles[WHITE] & sqb);
        bool supported = protectedPawns[BLACK] & sqb;
        bool advancable = !(pawnBlockage[BLACK] & south(sqb));
        bool phal = phalanx[sq] & bb[p];
        bool candidatePassed = !(passedPawnMask[BLACK][sq] & bb[P]);
        
        bool backward = !( // If the pawn
            (backwardPawnMask[BLACK][sq] & bb[p]) // If the pawn is doesn't have behind or adjacent pawns in adjacent files
            || advancable // If the pawn can't advance (blocked by enemy pawn or enemy pawn capture square)
        );
        // Check if the pawn is doubled
        if (isolated){
            if (doubled && pawnOpposed){
                mgScore += DOUBLEISOLATEDPENMG;
                egScore += DOUBLEISOLATEDPENEG;
            }
            else{
                mgScore += ISOLATEDPENMG;
                egScore += ISOLATEDPENEG;
            }
        }
        else if (backward){
            mgScore += BACKWARDPENMG;
            egScore += BACKWARDPENEG;
        }
        if (doubled && !supported){
            mgScore += DOUBLEDPENMG;
            egScore += DOUBLEDPENEG;
        }
        else if (supported && phal){
            mgScore -= SUPPORTEDPHALANXMG;
            mgScore -= R_SUPPORTEDPHALANXMG * (rank - 2);
            egScore -= SUPPORTEDPHALANXEG;
            egScore -= R_SUPPORTEDPHALANXEG * (rank - 2);
        }
        else if (advancable && phal){
            mgScore -= ADVANCABLEPHALANXMG;
            mgScore -= R_ADVANCABLEPHALANXMG * (rank - 2);
            egScore -= ADVANCABLEPHALANXEG;
            egScore -= R_ADVANCABLEPHALANXEG * (rank - 2);
        }
        if (candidatePassed){
            BitBoard passedPath = advancePathMasked<BLACK>(sqb, ~bb[BOTH]);
            // Give bonus for how close the pawn is to the promotion square
            mgScore -= passedRankBonusMg[rank];
            egScore -= passedRankBonusEg[rank];
            // Give bonus for how many squares the pawn can advance
            mgScore -= popcount(passedPath) * PASSEDPATHBONUSMG;
            egScore -= popcount(passedPath) * PASSEDPATHBONUSEG;
            // Bonus for connected or supported passed pawns
            if (supported){
                mgScore -= SUPPORTEDPASSERMG;
                egScore -= SUPPORTEDPASSEREG;
            }
        }
    }

    // Calculate king safety
    // King shield. The inner shield is direcly in front of the king so it should be at least supported by the king itself
    BitBoard innerShelters[2] = {
        kingShelter[WHITE][whiteKing],
        kingShelter[BLACK][blackKing]
    };
    // The outer shield is the squares in front of the inner shield. We only consider supported squares
    BitBoard outerShelters[2] = {
        north(innerShelters[WHITE]) & pawnAttackedSquares[WHITE],
        south(innerShelters[BLACK]) & pawnAttackedSquares[BLACK]
    };

    // Get pawns
    innerShelters[WHITE] &= bb[P];
    outerShelters[WHITE] &= bb[P];
    innerShelters[BLACK] &= bb[p];
    outerShelters[BLACK] &= bb[p];

    // Add the shelter bonus
    const Score innerShelterDiff = popcount(innerShelters[WHITE]) - popcount(innerShelters[BLACK]);
    const Score outerShelterDiff = popcount(outerShelters[WHITE]) - popcount(outerShelters[BLACK]);
    mgScore += INNERSHELTERMG * innerShelterDiff;
    egScore += INNERSHELTEREG * innerShelterDiff;
    mgScore += OUTERSHELTERMG * outerShelterDiff;
    egScore += OUTERSHELTEREG * outerShelterDiff;

    // Add bonus for bishop pairs
    mgScore += ((popcount(bb[B])>=2) - (popcount(bb[b])>=2)) * BISHOPPAIRMG;
    egScore += ((popcount(bb[B])>=2) - (popcount(bb[b])>=2)) * BISHOPPAIREG;

    // Add bonus for rooks on open files
    const BitBoard openFiles = ~(pawnFiles[WHITE] | pawnFiles[BLACK]);
    const BitBoard semiOpenFor[2] = {
        ~pawnFiles[WHITE],
        ~pawnFiles[BLACK]
    };
    const Score openDiff = popcount(bb[R] & openFiles) - popcount(bb[r] & openFiles);
    const Score semiOpenDiff = popcount(bb[R] & semiOpenFor[WHITE]) - popcount(bb[r] & semiOpenFor[BLACK]);
    mgScore += openDiff * ROOKONOPENFILEMG;
    egScore += openDiff * ROOKONOPENFILEEG;
    mgScore += semiOpenDiff * ROOKONSEMIOPENFILEMG;
    egScore += semiOpenDiff * ROOKONSEMIOPENFILEEG;

    // Add outpost squares bonus for knights and bishops
    const Score extKnightOutpostDiff = popcount(outpostSquares[WHITE] & (files(1) | files(6)) & bb[N]) - popcount(outpostSquares[BLACK] & (files(1) | files(6)) & bb[n]);
    mgScore += extKnightOutpostDiff * KNIGHTONEXTOUTPOSTMG;
    egScore += extKnightOutpostDiff * KNIGHTONEXTOUTPOSTEG;
    const Score extBishopOutpostDiff = popcount(outpostSquares[WHITE] & (files(1) | files(6)) & bb[B]) - popcount(outpostSquares[BLACK] & (files(1) | files(6)) & bb[b]);
    mgScore += extBishopOutpostDiff * BISHOPONEXTOUTPOSTMG;
    egScore += extBishopOutpostDiff * BISHOPONEXTOUTPOSTEG;
    const Score intKnightOutpostDiff = popcount(outpostSquares[WHITE] & (notFile(1) & notFile(6)) & bb[N]) - popcount(outpostSquares[BLACK] & (notFile(1) & notFile(6)) & bb[n]);
    mgScore += intKnightOutpostDiff * KNIGHTONINTOUTPOSTMG;
    egScore += intKnightOutpostDiff * KNIGHTONINTOUTPOSTEG;
    const Score intBishopOutpostDiff = popcount(outpostSquares[WHITE] & (notFile(1) & notFile(6)) & bb[B]) - popcount(outpostSquares[BLACK] & (notFile(1) & notFile(6)) & bb[b]);
    mgScore += intBishopOutpostDiff * BISHOPONINTOUTPOSTMG;
    egScore += intBishopOutpostDiff * BISHOPONINTOUTPOSTEG;

    // Add bonus for bishop-pawn concordance
    const Score bishopPawnsDiff =
        popcount(bb[B] & squaresOfColor[WHITE]) * popcount(squaresOfColor[WHITE] & bb[P]) * (popcount(blockedPawns[WHITE] & centralFiles) + !(pawnAttackedSquares[WHITE] & squaresOfColor[WHITE] & bb[B])) -
        popcount(bb[b] & squaresOfColor[WHITE]) * popcount(squaresOfColor[WHITE] & bb[p]) * (popcount(blockedPawns[BLACK] & centralFiles) + !(pawnAttackedSquares[BLACK] & squaresOfColor[WHITE] & bb[b])) +
        popcount(bb[B] & squaresOfColor[BLACK]) * popcount(squaresOfColor[BLACK] & bb[P]) * (popcount(blockedPawns[WHITE] & centralFiles) + !(pawnAttackedSquares[WHITE] & squaresOfColor[BLACK] & bb[B])) -
        popcount(bb[b] & squaresOfColor[BLACK]) * popcount(squaresOfColor[BLACK] & bb[p]) * (popcount(blockedPawns[BLACK] & centralFiles) + !(pawnAttackedSquares[BLACK] & squaresOfColor[BLACK] & bb[b])) ;
    
    mgScore += bishopPawnsDiff * BISHOPPAWNSMG;
    egScore += bishopPawnsDiff * BISHOPPAWNSEG;

    // Threats
    const BitBoard threatSafePawns[2] = {
        makePawnAttacks<WHITE>(bb[P] & (attackedBy[WHITE] | ~attackedBy[BLACK])) & nonPawns[BLACK],
        makePawnAttacks<BLACK>(bb[p] & (attackedBy[BLACK] | ~attackedBy[WHITE])) & nonPawns[WHITE],
    };

    BitBoard pushes[2] = {
        north(bb[P]) & ~(occ[BOTH] | pawnAttackedSquares[BLACK]),
        south(bb[p]) & ~(occ[BOTH] | pawnAttackedSquares[WHITE]),
    };
    pushes[WHITE] |= north(pushes[WHITE] & files(5)) & ~(occ[BOTH] | pawnAttackedSquares[BLACK]);
    pushes[BLACK] |= north(pushes[BLACK] & files(2)) & ~(occ[BOTH] | pawnAttackedSquares[WHITE]);

    const BitBoard threatPawnPush[2] = {
        makePawnAttacks<WHITE>(pushes[WHITE] & (attackedBy[WHITE] | ~attackedBy[BLACK])) & nonPawns[BLACK],
        makePawnAttacks<BLACK>(pushes[BLACK] & (attackedBy[BLACK] | ~attackedBy[WHITE])) & nonPawns[WHITE],
    };

    const Score safePawnThreatDiff = popcount(threatSafePawns[WHITE]) - popcount(threatSafePawns[BLACK]);
    const Score pawnPushThreatDiff = popcount(threatPawnPush[WHITE]) - popcount(threatPawnPush[BLACK]);

    mgScore += THREATSAFEPAWNMG * safePawnThreatDiff;
    egScore += THREATSAFEPAWNEG * safePawnThreatDiff;
    mgScore += THREATPAWNPUSHMG * pawnPushThreatDiff;
    egScore += THREATPAWNPUSHMG * pawnPushThreatDiff;

    const Score pawnHangingDiff = popcount(hanging[WHITE] & bb[P]) - popcount(hanging[BLACK] & bb[p]);
    const Score nonPawnHangingDiff = popcount(hanging[WHITE] & nonPawns[WHITE]) - popcount(hanging[BLACK] & nonPawns[BLACK]);
    mgScore += PAWNHANGINGMG * pawnHangingDiff;
    egScore += PAWNHANGINGEG * pawnHangingDiff;
    mgScore += NONPAWNHANGINGMG * nonPawnHangingDiff;
    egScore += NONPAWNHANGINGEG * nonPawnHangingDiff;

    const Score kingThreatsDiff = (!!kingThreats[WHITE]) - (!!kingThreats[BLACK]);
    mgScore += KINGTHREATMG * kingThreatsDiff;
    egScore += KINGTHREATEG * kingThreatsDiff;

    // Calculate the total score
    mgScore += mobilityScore[0];
    egScore += mobilityScore[1];

    // Add kingDanger index eval
    // Calcuate danger score
    S32 dangerIndex[2] = {
        innerAttacks[WHITE] + outerAttacks[WHITE], 
        innerAttacks[BLACK] + outerAttacks[BLACK]
    };

    dangerIndex[WHITE] += NOQUEENDANGER * (!bb[Q]);
    dangerIndex[BLACK] += NOQUEENDANGER * (!bb[q]);

    dangerIndex[WHITE] += popcount(pinned[BLACK] & innerShelters[BLACK]) * PINNEDSHELTERDANGER;
    dangerIndex[BLACK] += popcount(pinned[WHITE] & innerShelters[WHITE]) * PINNEDSHELTERDANGER;

    dangerIndex[WHITE] = std::max(0,std::min(63, dangerIndex[WHITE] >> 3));
    dangerIndex[BLACK] = std::max(0,std::min(63, dangerIndex[BLACK] >> 3));

    mgScore += kingSafety[dangerIndex[WHITE]] - kingSafety[dangerIndex[BLACK]];
    egScore += kingSafety[dangerIndex[WHITE]] - kingSafety[dangerIndex[BLACK]];


    // Calculate mg and eg scores
    const Score sign = 1 - 2 * pos->side;
    mgScore += sign * TEMPOMG;
    egScore += sign * TEMPOEG;
    
    return sign * 
        (
            (pos->psqtScore[0] + mgScore) * gamePhase +
            (pos->psqtScore[1] + egScore) * (24 - gamePhase)
        ) / 24;
}

std::vector<Score> getCurrentEvalWeights(){
    std::vector<Score> weights;
    // Add the material weights
    for (Piece piece = P; piece <= Q; piece++){
        weights.push_back(mgValues[piece]);
    }
    // Add the psqt weights
    for (Piece piece = P; piece <= K; piece++){
        for (Square square = a8; square < noSquare; square++){
            weights.push_back(mgTables[piece][square]);
        }
    }
    // Add the mobility weights
    for (U8 mobcount = 0; mobcount < 9; mobcount++){
        weights.push_back(knightMobMg[mobcount]);
    }
    for (U8 mobcount = 0; mobcount < 14; mobcount++){
        weights.push_back(bishopMobMg[mobcount]);
    }
    for (U8 mobcount = 0; mobcount < 15; mobcount++){
        weights.push_back(rookMobMg[mobcount]);
    }
    for (U8 mobcount = 0; mobcount < 28; mobcount++){
        weights.push_back(queenMobMg[mobcount]);
    }
    // Add the pawn evaluation weights
    weights.push_back(DOUBLEISOLATEDPENMG);
    weights.push_back(ISOLATEDPENMG);
    weights.push_back(BACKWARDPENMG);
    weights.push_back(DOUBLEDPENMG);
    weights.push_back(SUPPORTEDPHALANXMG);
    weights.push_back(R_SUPPORTEDPHALANXMG);
    weights.push_back(ADVANCABLEPHALANXMG);
    weights.push_back(R_ADVANCABLEPHALANXMG);
    // Add the passed pawn bonus list
    for (U8 rank = 0; rank < 7; rank++){
        weights.push_back(passedRankBonusMg[rank]);
    }
    weights.push_back(PASSEDPATHBONUSMG);
    weights.push_back(SUPPORTEDPASSERMG);

    // Add the shelter weights
    weights.push_back(INNERSHELTERMG);
    weights.push_back(OUTERSHELTERMG);
    
    // Add the bishop pair bonus
    weights.push_back(BISHOPPAIRMG);

    // Add the rook and queen on open file bonus
    weights.push_back(ROOKONOPENFILEMG);
    weights.push_back(ROOKONSEMIOPENFILEMG);

    // Now, outpost
    weights.push_back(KNIGHTONEXTOUTPOSTMG);
    weights.push_back(BISHOPONEXTOUTPOSTMG);
    weights.push_back(KNIGHTONINTOUTPOSTMG);
    weights.push_back(BISHOPONINTOUTPOSTMG);

    // Now, bishop pawns
    weights.push_back(BISHOPPAWNSMG);

    // Now, threats
    weights.push_back(THREATSAFEPAWNMG);
    weights.push_back(THREATPAWNPUSHMG);
    weights.push_back(PAWNHANGINGMG);
    weights.push_back(NONPAWNHANGINGMG);
    weights.push_back(KINGTHREATMG);

    // Add the tempo bonus
    weights.push_back(TEMPOMG);

    // Now, EG
    // Add the material weights
    for (Piece piece = P; piece <= K; piece++){
        weights.push_back(egValues[piece]);
    }
    // Add the psqt weights
    for (Piece piece = P; piece <= K; piece++){
        for (Square square = a8; square < noSquare; square++){
            weights.push_back(egTables[piece][square]);
        }
    }
    // Add the mobility weights
    for (U8 mobcount = 0; mobcount < 9; mobcount++){
        weights.push_back(knightMobEg[mobcount]);
    }
    for (U8 mobcount = 0; mobcount < 14; mobcount++){
        weights.push_back(bishopMobEg[mobcount]);
    }
    for (U8 mobcount = 0; mobcount < 15; mobcount++){
        weights.push_back(rookMobEg[mobcount]);
    }
    for (U8 mobcount = 0; mobcount < 28; mobcount++){
        weights.push_back(queenMobEg[mobcount]);
    }
    // Add the pawn evaluation weights
    weights.push_back(DOUBLEISOLATEDPENEG);
    weights.push_back(ISOLATEDPENEG);
    weights.push_back(BACKWARDPENEG);
    weights.push_back(DOUBLEDPENEG);
    weights.push_back(SUPPORTEDPHALANXEG);
    weights.push_back(R_SUPPORTEDPHALANXEG);
    weights.push_back(ADVANCABLEPHALANXEG);
    weights.push_back(R_ADVANCABLEPHALANXEG);
    // Add the passed pawn bonus list
    for (U8 rank = 0; rank < 7; rank++){
        weights.push_back(passedRankBonusEg[rank]);
    }
    weights.push_back(PASSEDPATHBONUSEG);
    weights.push_back(SUPPORTEDPASSEREG);

    // Add the shelter weights
    weights.push_back(INNERSHELTEREG);
    weights.push_back(OUTERSHELTEREG);

    // Add the bishop pair bonus
    weights.push_back(BISHOPPAIREG);

    // Add the rook and queen on open file bonus
    weights.push_back(ROOKONOPENFILEEG);
    weights.push_back(ROOKONSEMIOPENFILEEG);

    // Now, outpost
    weights.push_back(KNIGHTONEXTOUTPOSTEG);
    weights.push_back(BISHOPONEXTOUTPOSTEG);
    weights.push_back(KNIGHTONINTOUTPOSTEG);
    weights.push_back(BISHOPONINTOUTPOSTEG);

    // Now, bishop pawns
    weights.push_back(BISHOPPAWNSEG);
    
    // Now, threats
    weights.push_back(THREATSAFEPAWNEG);
    weights.push_back(THREATPAWNPUSHEG);
    weights.push_back(PAWNHANGINGEG);
    weights.push_back(NONPAWNHANGINGEG);
    weights.push_back(KINGTHREATEG);

    // Add the tempo bonus
    weights.push_back(TEMPOEG);

    return weights;
}
    
// Returns the evaluation features tensor
// This will be used to tune the evaluation function
void getEvalFeaturesTensor(Position *pos, S8* tensor, S32 tensorSize){

    auto const& bb = pos->bitboards;
    auto const& occ = pos->occupancies;
    auto us = pos->side;
    Square whiteKing = lsb(bb[K]);
    Square blackKing = lsb(bb[k]);

    // Calculate the game phase
    S32 gamePhase = gamephaseInc[P] * popcount(bb[P] | bb[p]) +
                    gamephaseInc[N] * popcount(bb[N] | bb[n]) +
                    gamephaseInc[B] * popcount(bb[B] | bb[b]) +
                    gamephaseInc[R] * popcount(bb[R] | bb[r]) +
                    gamephaseInc[Q] * popcount(bb[Q] | bb[q]) +
                    gamephaseInc[K] * popcount(bb[K] | bb[k]);
    
    gamePhase = std::min(gamePhase, (S32)24); // If we have a lot of pieces, we don't want to go over 24
    // Add the game phase
    tensor[0] = gamePhase;
    ++tensor;

    // Add the material weights
    for (Piece piece = P; piece <= Q; piece++){
        tensor[piece] = popcount(pos->bitboards[piece]) - popcount(pos->bitboards[piece + 6]);
    }
    tensor += 5;



    // Add the psqt weights
    for (Square square = a8; square < noSquare; square++){
        Piece piece = pos->pieceOn(square);
        if (piece == NOPIECE) continue;
        Square sq = piece < p ? square : flipSquare(square);
        tensor[64 * (piece % 6) + sq] += piece < p ? 1 : -1;
    }
    tensor += 64 * 6;
    
    // Pin, mobility and threat calculations
    BitBoard attackedBy[2] = {
        kingAttacks[whiteKing],
        kingAttacks[blackKing]
    };

    BitBoard pawnAttackedSquares[2] = {
        0,
        0
    };


    BitBoard multiAttacks[2] = {
        0ULL,
        0ULL
    };
    
    {
        // Left Attacks
        BitBoard sidePawnAttacks[2] = {
            ((bb[P] & notFile(0)) >> 9),
            ((bb[p] & notFile(0)) << 7)
        }; 
        // Update Multi attacks
        multiAttacks[WHITE] |= attackedBy[WHITE] & sidePawnAttacks[WHITE];
        multiAttacks[BLACK] |= attackedBy[BLACK] & sidePawnAttacks[BLACK];
        // Update Normal attacks
        attackedBy[WHITE] |= sidePawnAttacks[WHITE];
        attackedBy[BLACK] |= sidePawnAttacks[BLACK];
        // Update exclusive pawn attacks
        pawnAttackedSquares[WHITE] |= sidePawnAttacks[WHITE];
        pawnAttackedSquares[BLACK] |= sidePawnAttacks[BLACK];
        // Right Attacks
        sidePawnAttacks[WHITE] = ((bb[P] & notFile(7)) >> 7); 
        sidePawnAttacks[BLACK] = ((bb[p] & notFile(7)) << 9);
        // Update multi attacks
        multiAttacks[WHITE] |= attackedBy[WHITE] & sidePawnAttacks[WHITE];
        multiAttacks[BLACK] |= attackedBy[BLACK] & sidePawnAttacks[BLACK];
        // Update Normal attacks
        attackedBy[WHITE] |= sidePawnAttacks[WHITE];
        attackedBy[BLACK] |= sidePawnAttacks[BLACK];
        // Update exclusive pawn attacks
        pawnAttackedSquares[WHITE] |= sidePawnAttacks[WHITE];
        pawnAttackedSquares[BLACK] |= sidePawnAttacks[BLACK];
    }

    // Pinned mask
    BitBoard RQmask[2] = {
        bb[R] | bb[Q],
        bb[r] | bb[q]
    };
    BitBoard BQmask[2] = {
        bb[B] | bb[Q],
        bb[b] | bb[q]
    };
    BitBoard pinned[2] = {
        getPinnedPieces(occ[BOTH], occ[WHITE], whiteKing, RQmask[BLACK], BQmask[BLACK]),
        getPinnedPieces(occ[BOTH], occ[BLACK], blackKing, RQmask[WHITE], BQmask[WHITE])
    };

    BitBoard blockedPawns[2] = {
        bb[P] & (occ[BOTH] >> 8),
        bb[p] & (occ[BOTH] << 8)
    };

    BitBoard underDevelopedPawns[2] = {
        bb[P] & (ranks(6) | ranks(5)),
        bb[p] & (ranks(1) | ranks(2))
    };

    BitBoard mobilityArea[2] = {
        // Mobility area.
        // Squares in the mobility area are:
        // 1. Free of our pieces
        // 2. Not defended by enemy pawns

        // Calculate a negative mask for the mobility area, and then invert it
        (blockedPawns[WHITE] | underDevelopedPawns[WHITE] | pawnAttackedSquares[BLACK] | pinned[WHITE] | bb[K] | bb[Q]) ^ 0xFFFFFFFFFFFFFFFF,
        (blockedPawns[BLACK] | underDevelopedPawns[BLACK] | pawnAttackedSquares[WHITE] | pinned[BLACK] | bb[k] | bb[q]) ^ 0xFFFFFFFFFFFFFFFF
    };
    
    BitBoard nonPawns[2] = {
        bb[N] | bb[B] | bb[R] | bb[Q],
        bb[n] | bb[b] | bb[r] | bb[q],
    };

    const BitBoard kingRing[2] = {
        kingAttacks[whiteKing],
        kingAttacks[blackKing]
    };

    const BitBoard kingOuter[2] = {
        fiveSquare[whiteKing],
        fiveSquare[blackKing]
    };

    S32 innerAttacks[2][5] = {{0}}; 
    S32 outerAttacks[2][5] = {{0}};

    innerAttacks[WHITE][P] = popcount(pawnAttackedSquares[WHITE] & kingRing[BLACK]);
    innerAttacks[BLACK][P] = popcount(pawnAttackedSquares[BLACK] & kingRing[WHITE]);
    outerAttacks[WHITE][P] = popcount(pawnAttackedSquares[WHITE] & kingOuter[BLACK]);
    outerAttacks[BLACK][P] = popcount(pawnAttackedSquares[BLACK] & kingOuter[WHITE]);

    // Calculate the mobility scores (index by phase and color)
    getMobilityFeat<N>(bb, occ[BOTH], pinned[WHITE], mobilityArea[WHITE], attackedBy[WHITE], multiAttacks[WHITE], tensor, kingRing[WHITE], kingOuter[WHITE], innerAttacks[WHITE][N], outerAttacks[WHITE][N]);
    getMobilityFeat<n>(bb, occ[BOTH], pinned[BLACK], mobilityArea[BLACK], attackedBy[BLACK], multiAttacks[BLACK], tensor, kingRing[BLACK], kingOuter[BLACK], innerAttacks[BLACK][N], outerAttacks[BLACK][N]);
    tensor += 9;
    getMobilityFeat<B>(bb, occ[BOTH], pinned[WHITE], mobilityArea[WHITE], attackedBy[WHITE], multiAttacks[WHITE], tensor, kingRing[WHITE], kingOuter[WHITE], innerAttacks[WHITE][B], outerAttacks[WHITE][B]);
    getMobilityFeat<b>(bb, occ[BOTH], pinned[BLACK], mobilityArea[BLACK], attackedBy[BLACK], multiAttacks[BLACK], tensor, kingRing[BLACK], kingOuter[BLACK], innerAttacks[BLACK][B], outerAttacks[BLACK][B]);
    tensor += 14;
    getMobilityFeat<R>(bb, occ[BOTH], pinned[WHITE], mobilityArea[WHITE], attackedBy[WHITE], multiAttacks[WHITE], tensor, kingRing[WHITE], kingOuter[WHITE], innerAttacks[WHITE][R], outerAttacks[WHITE][R]);
    getMobilityFeat<r>(bb, occ[BOTH], pinned[BLACK], mobilityArea[BLACK], attackedBy[BLACK], multiAttacks[BLACK], tensor, kingRing[BLACK], kingOuter[BLACK], innerAttacks[BLACK][R], outerAttacks[BLACK][R]);
    tensor += 15;
    getMobilityFeat<Q>(bb, occ[BOTH], pinned[WHITE], mobilityArea[WHITE], attackedBy[WHITE], multiAttacks[WHITE], tensor, kingRing[WHITE], kingOuter[WHITE], innerAttacks[WHITE][Q], outerAttacks[WHITE][Q]);
    getMobilityFeat<q>(bb, occ[BOTH], pinned[BLACK], mobilityArea[BLACK], attackedBy[BLACK], multiAttacks[BLACK], tensor, kingRing[BLACK], kingOuter[BLACK], innerAttacks[BLACK][Q], outerAttacks[BLACK][Q]);
    tensor += 28;

    // Weak pieces
    BitBoard weakPieces[2] = {
        occ[WHITE] & ~pawnAttackedSquares[WHITE] & ((attackedBy[BLACK] & ~attackedBy[WHITE]) | (multiAttacks[BLACK] & ~multiAttacks[WHITE])),
        occ[BLACK] & ~pawnAttackedSquares[BLACK] & ((attackedBy[WHITE] & ~attackedBy[BLACK]) | (multiAttacks[WHITE] & ~multiAttacks[BLACK]))
    };

    // Hanging enemies
    BitBoard hanging[2] = {
        weakPieces[WHITE] & (~attackedBy[WHITE] | (nonPawns[WHITE] & multiAttacks[BLACK])),
        weakPieces[BLACK] & (~attackedBy[BLACK] | (nonPawns[BLACK] & multiAttacks[WHITE]))
    };

    // King Threats (eg op-ness hopefully) (also, cap to 1)
    BitBoard kingThreats[2] = {
        weakPieces[BLACK] & kingAttacks[WHITE],
        weakPieces[WHITE] & kingAttacks[BLACK]
    };

    // Add the pawn evaluation weights
    // Malus for directly doubled, undefended pawns
    BitBoard protectedPawns[2] = {
        bb[P] & pawnAttackedSquares[WHITE],
        bb[p] & pawnAttackedSquares[BLACK]
    };

    BitBoard pawnBlockage[2] = {
        bb[p] | bb[P] | pawnAttackedSquares[BLACK],
        bb[P] | bb[p] | pawnAttackedSquares[WHITE]
    };

    BitBoard doubledPawns[2] = { bb[P] & (bb[P] << 8), bb[p] & (bb[p] >> 8) };
    BitBoard pawnFiles[2] = { filesFromBB(bb[P]), filesFromBB(bb[p]) };
    // White pawns
    BitBoard pieces = bb[P];
    while (pieces){
        // Evaluate isolated, backward, doubled pawns and connected pawns
        Square sq = popLsb(pieces);
        BitBoard sqb = squareBB(sq);
        U8 rank = 7 - rankOf(sq);
        bool doubled = doubledPawns[WHITE] & squareBB(sq);
        bool isolated = !(isolatedPawnMask[sq] & bb[P]);
        bool pawnOpposed = !(pawnFiles[BLACK] & sqb);
        bool supported = protectedPawns[WHITE] & sqb;
        bool advancable = !(pawnBlockage[WHITE] & north(sqb));
        bool phal = phalanx[sq] & bb[P];
        bool candidatePassed = !(passedPawnMask[WHITE][sq] & bb[p]);
        
        bool backward = !( // If the pawn
            (backwardPawnMask[WHITE][sq] & bb[P]) // If the pawn is doesn't have behind or adjacent pawns in adjacent files
            || advancable // If the pawn can't advance (blocked by enemy pawn or enemy pawn capture square)
        );
        // Check if the pawn is doubled
        if (doubled && isolated && pawnOpposed) tensor[0]--;

        else if (isolated) tensor[1]--;
        else if (backward) tensor[2]--;
        if (doubled && !supported) tensor[3]--;
        else if (supported && phal) {
            tensor[4]++;
            tensor[5] += rank - 2;
        }
        else if (advancable && phal) {
            tensor[6]++;
            tensor[7] += rank - 2;
        }
        if (candidatePassed){
            BitBoard passedPath = advancePathMasked<WHITE>(sqb, ~bb[BOTH]);
            // Give bonus for how close the pawn is to the promotion square
            tensor[8 + rank] += 1;
            tensor[8 + 7] += popcount(passedPath);
            // Bonus for connected or supported passed pawns
            if (supported) tensor[8 + 7 + 1]++;
        }
    }
    

    // Black pawns
    pieces = bb[p];
    while (pieces){
        // Evaluate isolated, backward, doubled pawns and connected pawns
        Square sq = popLsb(pieces);
        BitBoard sqb = squareBB(sq);
        U8 rank = rankOf(sq);
        bool doubled = doubledPawns[BLACK] & squareBB(sq);
        bool isolated = !(isolatedPawnMask[sq] & bb[p]);
        bool pawnOpposed = !(pawnFiles[WHITE] & sqb);
        bool supported = protectedPawns[BLACK] & sqb;
        bool advancable = !(pawnBlockage[BLACK] & south(sqb));
        bool phal = phalanx[sq] & bb[p];
        bool candidatePassed = !(passedPawnMask[BLACK][sq] & bb[P]);
        
        bool backward = !( // If the pawn
            (backwardPawnMask[BLACK][sq] & bb[p]) // If the pawn is doesn't have behind or adjacent pawns in adjacent files
            || advancable // If the pawn can't advance (blocked by enemy pawn or enemy pawn capture square)
        );
        // Check if the pawn is doubled
        if (doubled && isolated && pawnOpposed) tensor[0]++;
        else if (isolated) tensor[1]++;
        else if (backward) tensor[2]++;
        if (doubled && !supported) tensor[3]++;
        else if (supported && phal) {
            tensor[4]--;
            tensor[5] -= rank - 2;
        }
        else if (advancable && phal) {
            tensor[6]--;
            tensor[7] -= rank - 2;
        }
        if (candidatePassed){
            BitBoard passedPath = advancePathMasked<BLACK>(sqb, ~bb[BOTH]);
            // Give bonus for how close the pawn is to the promotion square
            tensor[8 + rank]--;
            tensor[8 + 7] -= popcount(passedPath);
            // Bonus for connected or supported passed pawns
            if (supported) tensor[8 + 7 + 1]--;
        }
    }
    tensor += 8 + 7 + 2;

    // Calculate king safety
    // King shield. The inner shield is direcly in front of the king so it should be at least supported by the king itself
    BitBoard innerShelters[2] = {
        kingShelter[WHITE][whiteKing],
        kingShelter[BLACK][blackKing]
    };
    // The outer shield is the squares in front of the inner shield. We only consider supported squares
    BitBoard outerShelters[2] = {
        north(innerShelters[WHITE]) & pawnAttackedSquares[WHITE],
        south(innerShelters[BLACK]) & pawnAttackedSquares[BLACK]
    };

    // Get pawns
    innerShelters[WHITE] &= bb[P];
    innerShelters[BLACK] &= bb[p];
    outerShelters[WHITE] &= bb[P];
    outerShelters[BLACK] &= bb[p];

    // Add the shelter bonus
    tensor[0] += popcount(innerShelters[WHITE]) - popcount(innerShelters[BLACK]);
    tensor[1] += popcount(outerShelters[WHITE]) - popcount(outerShelters[BLACK]);
    tensor += 2;

    // Add the bishop pairs
    tensor[0] += (popcount(bb[B])>=2) - (popcount(bb[b])>=2);
    tensor++;

    // Add the rooks on open files
    BitBoard openFiles = ~(pawnFiles[WHITE] | pawnFiles[BLACK]);
    BitBoard semiOpenFor[2] = {
        ~pawnFiles[WHITE],
        ~pawnFiles[BLACK]
    };
    tensor[0] += (popcount(bb[R] & openFiles) - popcount(bb[r] & openFiles));
    tensor[1] += (popcount(bb[R] & semiOpenFor[WHITE]) - popcount(bb[r] & semiOpenFor[BLACK]));
    tensor += 2;

    // Outpost calculations
    BitBoard pawnSpan[2] = {
        advancePathMasked<WHITE>(bb[P], ~(bb[p] | pawnAttackedSquares[BLACK])),
        advancePathMasked<BLACK>(bb[p], ~(bb[P] | pawnAttackedSquares[WHITE]))
    };

    BitBoard outpostSquares[2] = {
        (ranks(2) | ranks(3) | ranks(4)) & pawnAttackedSquares[WHITE] & ~(makePawnAttacks<BLACK>(pawnSpan[BLACK]) | bb[p]) & notFile(0) & notFile(7),
        (ranks(5) | ranks(4) | ranks(3)) & pawnAttackedSquares[BLACK] & ~(makePawnAttacks<WHITE>(pawnSpan[WHITE]) | bb[P]) & notFile(0) & notFile(7)
    };

    Score extKnightOutpostDiff = popcount(outpostSquares[WHITE] & (files(1) | files(6)) & bb[N]) - popcount(outpostSquares[BLACK] & (files(1) | files(6)) & bb[n]);
    tensor[0] += extKnightOutpostDiff;
    Score extBishopOutpostDiff = popcount(outpostSquares[WHITE] & (files(1) | files(6)) & bb[B]) - popcount(outpostSquares[BLACK] & (files(1) | files(6)) & bb[b]);
    tensor[1] += extBishopOutpostDiff;
    Score intKnightOutpostDiff = popcount(outpostSquares[WHITE] & (notFile(1) & notFile(6)) & bb[N]) - popcount(outpostSquares[BLACK] & (notFile(1) & notFile(6)) & bb[n]);
    tensor[2] += intKnightOutpostDiff;
    Score intBishopOutpostDiff = popcount(outpostSquares[WHITE] & (notFile(1) & notFile(6)) & bb[B]) - popcount(outpostSquares[BLACK] & (notFile(1) & notFile(6)) & bb[b]);
    tensor[3] += intBishopOutpostDiff;
    tensor += 4;

    // Add bonus for bishop-pawn concordance (technically broken in two same colored bishop cases, but kek)
    Score bishopPawnsDiff =
        popcount(bb[B] & squaresOfColor[WHITE]) * popcount(squaresOfColor[WHITE] & bb[P]) * (popcount(blockedPawns[WHITE] & centralFiles) + !(pawnAttackedSquares[WHITE] & squaresOfColor[WHITE] & bb[B])) -
        popcount(bb[b] & squaresOfColor[WHITE]) * popcount(squaresOfColor[WHITE] & bb[p]) * (popcount(blockedPawns[BLACK] & centralFiles) + !(pawnAttackedSquares[BLACK] & squaresOfColor[WHITE] & bb[b])) +
        popcount(bb[B] & squaresOfColor[BLACK]) * popcount(squaresOfColor[BLACK] & bb[P]) * (popcount(blockedPawns[WHITE] & centralFiles) + !(pawnAttackedSquares[WHITE] & squaresOfColor[BLACK] & bb[B])) -
        popcount(bb[b] & squaresOfColor[BLACK]) * popcount(squaresOfColor[BLACK] & bb[p]) * (popcount(blockedPawns[BLACK] & centralFiles) + !(pawnAttackedSquares[BLACK] & squaresOfColor[BLACK] & bb[b])) ;
    
    tensor[0] += bishopPawnsDiff;

    tensor++;

    // Threats
    BitBoard threatSafePawns[2] = {
        makePawnAttacks<WHITE>(bb[P] & (attackedBy[WHITE] | ~attackedBy[BLACK])) & nonPawns[BLACK],
        makePawnAttacks<BLACK>(bb[p] & (attackedBy[BLACK] | ~attackedBy[WHITE])) & nonPawns[WHITE],
    };

    BitBoard pushes[2] = {
        north(bb[P]) & ~(occ[BOTH] | pawnAttackedSquares[BLACK]),
        south(bb[p]) & ~(occ[BOTH] | pawnAttackedSquares[WHITE]),
    };
    pushes[WHITE] |= north(pushes[WHITE] & files(5)) & ~(occ[BOTH] | pawnAttackedSquares[BLACK]);
    pushes[BLACK] |= north(pushes[BLACK] & files(2)) & ~(occ[BOTH] | pawnAttackedSquares[WHITE]);

    BitBoard threatPawnPush[2] = {
        makePawnAttacks<WHITE>(pushes[WHITE] & (attackedBy[WHITE] | ~attackedBy[BLACK])) & nonPawns[BLACK],
        makePawnAttacks<BLACK>(pushes[BLACK] & (attackedBy[BLACK] | ~attackedBy[WHITE])) & nonPawns[WHITE],
    };

    Score safePawnThreatDiff = popcount(threatSafePawns[WHITE]) - popcount(threatSafePawns[BLACK]);
    Score pawnPushThreatDiff = popcount(threatPawnPush[WHITE]) - popcount(threatPawnPush[BLACK]);

    tensor[0] = safePawnThreatDiff;
    tensor[1] = pawnPushThreatDiff;
    tensor += 2;

    const Score pawnHangingDiff = popcount(hanging[WHITE] & bb[P]) - popcount(hanging[BLACK] & bb[p]);
    const Score nonPawnHangingDiff = popcount(hanging[WHITE] & nonPawns[WHITE]) - popcount(hanging[BLACK] & nonPawns[BLACK]);
    tensor[0] = pawnHangingDiff;
    tensor[1] = nonPawnHangingDiff;
    tensor += 2;

    const Score kingThreatsDiff = (!!kingThreats[WHITE]) - (!!kingThreats[BLACK]);
    tensor[0] = kingThreatsDiff;
    ++tensor;

    // Add the tempo bonus
    tensor[0] += us == WHITE ? 1 : -1;
    tensor++;

    // Now, king index eval.
    Score noQueenDanger[2] = {!bb[Q], !bb[q]};
    S32 pinnedShelter[2] = {popcount(pinned[BLACK] & innerShelters[BLACK]), popcount(pinned[WHITE] & innerShelters[WHITE])};

#define KINGSAFETYCOLOREDPARAMS 24
    tensor[P] = innerAttacks[WHITE][P];
    tensor[N] = innerAttacks[WHITE][N];
    tensor[B] = innerAttacks[WHITE][B];
    tensor[R] = innerAttacks[WHITE][R];
    tensor[Q] = innerAttacks[WHITE][Q];
    tensor += 5;
    tensor[P] = outerAttacks[WHITE][P];
    tensor[N] = outerAttacks[WHITE][N];
    tensor[B] = outerAttacks[WHITE][B];
    tensor[R] = outerAttacks[WHITE][R];
    tensor[Q] = outerAttacks[WHITE][Q];
    tensor += 5;
    tensor[0] = noQueenDanger[WHITE];
    tensor[1] = pinnedShelter[WHITE];
    tensor += 2;

    tensor[P] = innerAttacks[BLACK][P];
    tensor[N] = innerAttacks[BLACK][N];
    tensor[B] = innerAttacks[BLACK][B];
    tensor[R] = innerAttacks[BLACK][R];
    tensor[Q] = innerAttacks[BLACK][Q];
    tensor += 5;
    tensor[P] = outerAttacks[BLACK][P];
    tensor[N] = outerAttacks[BLACK][N];
    tensor[B] = outerAttacks[BLACK][B];
    tensor[R] = outerAttacks[BLACK][R];
    tensor[Q] = outerAttacks[BLACK][Q];
    tensor += 5;
    tensor[0] = noQueenDanger[BLACK];
    tensor[1] = pinnedShelter[BLACK];
    tensor += 2;

    // Also assert the last element we wrote is the penultimate element
    // assert(tensorStart + tensorSize - 2 == tensor);
}


/**
 * @brief The convertToFeatures function converts a set of positions to a set of features
 * @param filename The filename of the epd file
 * @param output The output file to write the features to
 */
void convertToFeatures(std::string filename, std::string output) {
    std::ifstream file(filename);
    // Check if the file is open
    if (!file.is_open()){
        std::cout << "Error: Could not open file " << filename << std::endl;
        return;
    }
    // Open output in binary mode
    std::ofstream out(output, std::ios::binary);
    // Get the count of weights by getting the current eval weights
    auto weights = getCurrentEvalWeights();
    U32 weightCount = weights.size();
    std::cout << "Allocating for " << weightCount << " weights: " << 1 + weightCount / 2 + 1 << " bytes per entry" << std::endl;
    U32 entrySize = 1 + weightCount / 2 + 1; // The entry size is the 1 (gamephase) + number of weights divided by 2 + 1, since we add the result at the end, but we don't need to write the features twice for each side
    entrySize += KINGSAFETYCOLOREDPARAMS;
    std::cout << "Additional colored params: " << KINGSAFETYCOLOREDPARAMS / 2 << " x 2 = " << KINGSAFETYCOLOREDPARAMS << std::endl;
    // Create buffer to store the features
    S8* features = new S8[entrySize];
    // For each line in the file
    std::string line;
    Position pos;
    U64 counter = 0;
    while (std::getline(file, line)){
        // Clear the features
        memset(features, 0, entrySize);
        // Parse the line. It contains the FEN string and the result. They are separated by a ";", and the result is either w,d,b (white, draw, black)
        std::vector<std::string> tokens;

        std::string token;
        std::istringstream tokenStream(line);
        while (std::getline(tokenStream, token, ';')){
            tokens.push_back(token);
        }
        // Parse the position
        if (pos.parseFEN((char*)tokens[0].c_str())){
            // Get the features
            getEvalFeaturesTensor(&pos, features, entrySize);

            // Assert we filled the features correctly. This is checked by checking the penultimate element of the features and checking if it is 1 or -1
            assert(features[entrySize - 2] == 1 || features[entrySize - 2] == -1);
            // Get the result
            if (tokens[1] == "w") features[entrySize - 1] = 1;
            else if (tokens[1] == "d") features[entrySize - 1] = 0;
            else features[entrySize - 1] = -1;
            // Write the features to the output file
            out.write((char*)features, entrySize);
        }
        // Increment the counter and report progress each 16384 positions
        if (++counter % 16384 == 0){
            std::cout << "Processed " << counter << " positions...\r";
        }
    }
    // Report the final progress
    std::cout << "Processed " << counter << " positions." << std::endl;
    // Close the file
    out.close();
    // Delete the features
    delete[] features;
}