#ifndef GAME_H
#define GAME_H

#include <stdint.h>
#include "engine.h"

typedef struct _sGame sGame;

// 出牌:回傳要出的牌號(必須是自己手牌裡的牌,否則判定犯規)
// me 是自己的座位編號。AI 只該讀自己的手牌 g->player[me].hand,不要偷看別人的。
typedef int32_t ( *fPick )( const sGame *g, int32_t me );

// 選列:card 比所有列的末尾都小時被呼叫,回傳要收走的列(0~3)
typedef int32_t ( *fChoose )( const sGame *g, int32_t me, int32_t card );

typedef struct _sPlayer
{
    const char  *name;
    int32_t     score;                  // 累積的牛頭數
    int32_t     hand[HAND_SIZE];        // 手牌,由小到大排序
    int32_t     hand_len;
    fPick       pick;
    fChoose     choose;
} sPlayer;

struct _sGame
{
    int32_t     n_players;
    sPlayer     player[MAX_PLAYERS];
    int32_t     table[NUM_ROWS][ROW_CAP];
    int32_t     last[MAX_PLAYERS];      // 上一回合各玩家出的牌(第一回合是 0)
    uint8_t     seen[NUM_CARDS + 1];    // 已公開過的牌(打出過、或翻在桌面上過),seen[牌號] = 1
    int32_t     round;                  // 已完成的回合數,滿 HAND_SIZE 就結束
    int32_t     cheater;                // 出不合法的牌或選不合法的列的玩家,沒有則 -1
};

// 一張牌被打出後發生的事,給介面顯示用
typedef struct _sMove
{
    int32_t         player;
    int32_t         card;
    ePlaceResult    result;             // PLACE_OK / PLACE_FULL / PLACE_LOW
    int32_t         row;                // 牌最後所在的列(收牌時是被收走並換成這張牌的那一列)
    int32_t         penalty;            // 這一步吃到的牛頭數,PLACE_OK 時為 0
} sMove;

// 開新局:發牌、手牌排序、分數歸零。之後由呼叫端填 name / pick / choose。
// n_players 不在 2~10 之間時回傳 -1,成功回傳 0。
int32_t game_init( sGame *g, int32_t n_players );

// 進行一回合:所有人各出一張,由小到大依序放牌。
// 成功時回傳這回合的步數(= 玩家數),moves 依放牌順序填入,至少要能放 MAX_PLAYERS 個。
// 有人犯規時回傳 -1,並把 g->cheater 設為該玩家。
int32_t game_play_round( sGame *g, sMove moves[] );

// 10 回合都打完了嗎?
int32_t game_is_over( const sGame *g );

#endif
