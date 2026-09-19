#ifndef ENGINE_H
#define ENGINE_H

#include <stdint.h>

#define NUM_CARDS   (104)
#define NUM_ROWS    (4)
#define ROW_CAP     (5)     // 每列最多 5 張,第 6 張要收牌
#define HAND_SIZE   (10)
#define MIN_PLAYERS (2)
#define MAX_PLAYERS (10)

// engine_place() 的結果
typedef enum _ePlaceResult
{
    PLACE_OK,       // 已接在某列後面
    PLACE_FULL,     // 該列已有 5 張,這張是第 6 張 -> 呼叫 engine_collect() 收走該列
    PLACE_LOW       // 比所有列的末尾都小 -> 玩家自己選一列,再呼叫 engine_collect()
} ePlaceResult;

// 發牌:洗 104 張牌,發給 n_players 個人各 10 張,再翻 4 張當桌面 4 列的第一張。
// table 的空位是 0。呼叫前請先在 main 呼叫一次 srand()。
// n_players 不在 2~10 之間時回傳 -1,成功回傳 0。
int32_t engine_deal( int32_t hands[][HAND_SIZE], int32_t n_players, int32_t table[NUM_ROWS][ROW_CAP] );

// 預測落點:card 會接在哪一列(末尾比它小、且最接近的那列)。比所有列都小時回傳 -1。
// 不會改動桌面;該列已滿時仍回傳那一列(實際放牌時會變成 PLACE_FULL)。
int32_t engine_find_row( const int32_t table[NUM_ROWS][ROW_CAP], int32_t card );

// 放牌:把 card 接在「末尾比它小、且差距最小」的那一列後面。
// PLACE_OK   :已放好,*row 是放進的那一列
// PLACE_FULL :*row 是已滿的那一列,card 尚未放入
// PLACE_LOW  :*row 是 -1,card 尚未放入
ePlaceResult engine_place( int32_t table[NUM_ROWS][ROW_CAP], int32_t card, int32_t *row );

// 收牌:收走第 row 列的所有牌,card 變成該列新的第一張。回傳收走的牛頭總數。
int32_t engine_collect( int32_t table[NUM_ROWS][ROW_CAP], int32_t row, int32_t card );

// 算分:一張牌的牛頭數。55 -> 7,11 的倍數 -> 5,10 的倍數 -> 3,5 的倍數 -> 2,其他 -> 1
int32_t engine_score( int32_t card );

// 輔助:第 row 列目前有幾張牌(非 0 的格數)
int32_t engine_row_len( const int32_t table[NUM_ROWS][ROW_CAP], int32_t row );

// 輔助:第 row 列所有牌的牛頭總數(收走這列會吃到的分數)
int32_t engine_row_score( const int32_t table[NUM_ROWS][ROW_CAP], int32_t row );

#endif
