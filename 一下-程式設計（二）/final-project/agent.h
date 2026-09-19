#ifndef AGENT_H
#define AGENT_H

#include "game.h"

// ---- Level 1 ----

// 從手牌隨機出一張
int32_t agent_random_pick( const sGame *g, int32_t me );

// 比所有列都小時,收走牛頭最少的那一列(同分取編號小的)
int32_t agent_min_heads_choose( const sGame *g, int32_t me, int32_t card );

// ---- Level 2 ----

// 對每張手牌估算「期望被罰的牛頭數」,出最小的那張。
// known[牌號] = 1 表示這張牌已知(自己的手牌、桌面上的牌、已打出過的牌),用來估計對手手上的牌。
// 只吃純資料、不依賴 sGame,遊戲與 contest 介面共用這個核心。
int32_t agent_safe_pick_core( const int32_t table[NUM_ROWS][ROW_CAP],
                              const int32_t hand[], int32_t hand_len,
                              const uint8_t known[NUM_CARDS + 1],
                              int32_t n_players );

// 牛頭數最少的那一列
int32_t agent_cheapest_row( const int32_t table[NUM_ROWS][ROW_CAP] );

// 給遊戲用的包裝
int32_t agent_safe_pick( const sGame *g, int32_t me );
int32_t agent_safe_choose( const sGame *g, int32_t me, int32_t card );

#endif
