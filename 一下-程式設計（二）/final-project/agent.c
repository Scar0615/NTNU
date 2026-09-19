#include <stdlib.h>
#include <string.h>
#include "agent.h"

// 平手時怎麼挑牌:0 = 牌號小的先出,1 = 牌號大的先出,2 = 離該列末尾近的先出
#ifndef TIE_MODE
#define TIE_MODE (0)
#endif

// ---- Level 1 ----

int32_t agent_random_pick( const sGame *g, int32_t me )
{
    const sPlayer *p = &g->player[me];

    return p->hand[rand() % p->hand_len];
}

int32_t agent_cheapest_row( const int32_t table[NUM_ROWS][ROW_CAP] )
{
    int32_t best = 0;
    int32_t best_score = engine_row_score( table, 0 );

    for( int32_t r = 1 ; r < NUM_ROWS ; r++ )
    {
        int32_t s = engine_row_score( table, r );
        if( s < best_score )
        {
            best_score = s;
            best = r;
        }
    }

    return best;
}

int32_t agent_min_heads_choose( const sGame *g, int32_t me, int32_t card )
{
    (void)me;
    (void)card;

    return agent_cheapest_row( g->table );
}

// ---- Level 2 ----

static double ipow( double base, int32_t exp )
{
    double result = 1.0;

    while( exp-- > 0 )
        result *= base;
    return result;
}

// n 次獨立試驗、每次成功機率 p,剛好成功 m 次的機率
static double binomial( int32_t n, double p, int32_t m )
{
    if( m < 0 || m > n )
        return 0.0;

    double coef = 1.0;
    for( int32_t i = 1 ; i <= m ; i++ )
        coef = coef * ( n - m + i ) / i;

    return coef * ipow( p, m ) * ipow( 1.0 - p, n - m );
}

// 出這張牌,期望會被罰幾個牛頭
static double expected_penalty( const int32_t table[NUM_ROWS][ROW_CAP], int32_t card,
                                const uint8_t known[NUM_CARDS + 1], int32_t n_unknown,
                                int32_t n_players )
{
    int32_t row = engine_find_row( table, card );

    // 比所有列都小:一定得收一列,收最便宜的
    if( row < 0 )
        return engine_row_score( table, agent_cheapest_row( table ) );

    int32_t len = engine_row_len( table, row );
    int32_t end = table[row][len - 1];

    // 落在「該列末尾 ~ 我的牌」之間、對手可能持有的牌
    int32_t gap = 0;
    int32_t gap_heads = 0;
    for( int32_t c = end + 1 ; c < card ; c++ )
    {
        if( !known[c] )
        {
            gap++;
            gap_heads += engine_score( c );
        }
    }

    // 對手各出一張牌,是「未知牌」中的隨機一張,落進 gap 的機率約 gap / n_unknown。
    // 這列還差 m 張就滿;由小到大放牌時,剛好有 m 張落進 gap,我的牌才會是第 6 張。
    // (多於 m 張時,第 6 張是別人的牌,他去收,我反而安全)
    int32_t m = ROW_CAP - len;
    double p = ( n_unknown > 0 ) ? (double)gap / n_unknown : 0.0;
    double risk = binomial( n_players - 1, p, m );
    double avg_heads = ( gap > 0 ) ? (double)gap_heads / gap : 0.0;

    return risk * ( engine_row_score( table, row ) + m * avg_heads );
}

// 平手時用的次要排序,越小越優先
static int32_t tie_key( const int32_t table[NUM_ROWS][ROW_CAP], int32_t card )
{
#if TIE_MODE == 1
    (void)table;
    return -card;
#elif TIE_MODE == 2
    int32_t row = engine_find_row( table, card );
    if( row < 0 )
        return 1000;
    return card - table[row][engine_row_len( table, row ) - 1];
#else
    (void)table;
    return card;
#endif
}

int32_t agent_safe_pick_core( const int32_t table[NUM_ROWS][ROW_CAP],
                              const int32_t hand[], int32_t hand_len,
                              const uint8_t known[NUM_CARDS + 1],
                              int32_t n_players )
{
    int32_t n_unknown = 0;
    for( int32_t c = 1 ; c <= NUM_CARDS ; c++ )
        if( !known[c] )
            n_unknown++;

    int32_t best_card = hand[0];
    double best_cost = 1e18;
    int32_t best_key = 0;

    for( int32_t k = 0 ; k < hand_len ; k++ )
    {
        double cost = expected_penalty( table, hand[k], known, n_unknown, n_players );
        int32_t key = tie_key( table, hand[k] );

        if( cost < best_cost - 1e-9 || ( cost < best_cost + 1e-9 && key < best_key ) )
        {
            best_card = hand[k];
            best_cost = cost;
            best_key = key;
        }
    }

    return best_card;
}

int32_t agent_safe_pick( const sGame *g, int32_t me )
{
    const sPlayer *p = &g->player[me];
    uint8_t known[NUM_CARDS + 1];

    memcpy( known, g->seen, sizeof( known ) );      // 桌面上的、打出過的牌都在 seen 裡
    for( int32_t k = 0 ; k < p->hand_len ; k++ )
        known[p->hand[k]] = 1;

    return agent_safe_pick_core( g->table, p->hand, p->hand_len, known, g->n_players );
}

int32_t agent_safe_choose( const sGame *g, int32_t me, int32_t card )
{
    (void)me;
    (void)card;

    return agent_cheapest_row( g->table );
}
