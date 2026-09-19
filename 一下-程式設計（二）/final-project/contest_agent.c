#include <string.h>
#include "agent.h"
#include "contest_agent.h"

// contest 介面不會告訴 AI 它的手牌與看過哪些牌,所以要自己記
static int32_t hand[HAND_SIZE];
static int32_t hand_len;
static uint8_t known[NUM_CARDS + 1];

void CSIE_40847016S_setup( int32_t id )
{
    (void)id;

    hand_len = 0;
    memset( known, 0, sizeof( known ) );
}

void CSIE_40847016S_deal( const int32_t cards[10] )
{
    for( int32_t k = 0 ; k < HAND_SIZE ; k++ )
    {
        hand[k] = cards[k];
        known[cards[k]] = 1;
    }
    hand_len = HAND_SIZE;
}

int32_t CSIE_40847016S_pick( const int32_t table[4][5], const int32_t score[2], const int32_t last[2] )
{
    (void)score;

    // 桌面上的牌、上一回合雙方出的牌,都是公開資訊
    for( int32_t r = 0 ; r < NUM_ROWS ; r++ )
        for( int32_t c = 0 ; c < ROW_CAP ; c++ )
            if( table[r][c] != 0 )
                known[table[r][c]] = 1;
    for( int32_t i = 0 ; i < 2 ; i++ )
        if( last[i] != 0 )
            known[last[i]] = 1;

    int32_t card = agent_safe_pick_core( table, hand, hand_len, known, 2 );

    // 把出掉的牌從手牌拿掉
    for( int32_t k = 0 ; k < hand_len ; k++ )
    {
        if( hand[k] == card )
        {
            hand[k] = hand[hand_len - 1];
            hand_len--;
            break;
        }
    }

    return card;
}

int32_t CSIE_40847016S_choose( const int32_t table[4][5], int32_t card )
{
    (void)card;

    return agent_cheapest_row( table );
}
