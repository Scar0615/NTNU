#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "engine.h"

int32_t engine_score( int32_t card )
{
    if( card == 55 )
        return 7;
    if( card % 11 == 0 )
        return 5;
    if( card % 10 == 0 )
        return 3;
    if( card % 5 == 0 )
        return 2;
    return 1;
}

int32_t engine_row_len( const int32_t table[NUM_ROWS][ROW_CAP], int32_t row )
{
    int32_t len = 0;

    while( len < ROW_CAP && table[row][len] != 0 )
        len++;

    return len;
}

int32_t engine_row_score( const int32_t table[NUM_ROWS][ROW_CAP], int32_t row )
{
    int32_t len = engine_row_len( table, row );
    int32_t total = 0;

    for( int32_t c = 0 ; c < len ; c++ )
        total += engine_score( table[row][c] );

    return total;
}

int32_t engine_deal( int32_t hands[][HAND_SIZE], int32_t n_players, int32_t table[NUM_ROWS][ROW_CAP] )
{
    if( n_players < MIN_PLAYERS || n_players > MAX_PLAYERS )
        return -1;

    int32_t cards[NUM_CARDS];
    for( int32_t i = 0 ; i < NUM_CARDS ; i++ )
        cards[i] = i + 1;

    // Fisher-Yates 洗牌
    for( int32_t i = NUM_CARDS - 1 ; i > 0 ; i-- )
    {
        int32_t j = rand() % ( i + 1 );
        int32_t temp = cards[i];
        cards[i] = cards[j];
        cards[j] = temp;
    }

    // 前 n_players * 10 張發給玩家,接著 4 張放桌面
    for( int32_t p = 0 ; p < n_players ; p++ )
        for( int32_t k = 0 ; k < HAND_SIZE ; k++ )
            hands[p][k] = cards[p * HAND_SIZE + k];

    memset( table, 0, sizeof( int32_t ) * NUM_ROWS * ROW_CAP );
    for( int32_t r = 0 ; r < NUM_ROWS ; r++ )
        table[r][0] = cards[n_players * HAND_SIZE + r];

    return 0;
}

int32_t engine_find_row( const int32_t table[NUM_ROWS][ROW_CAP], int32_t card )
{
    // 找「末尾比 card 小、且最大」的那一列
    int32_t best = -1;
    int32_t best_end = 0;

    for( int32_t r = 0 ; r < NUM_ROWS ; r++ )
    {
        int32_t end = table[r][engine_row_len( table, r ) - 1];
        if( end < card && end > best_end )
        {
            best = r;
            best_end = end;
        }
    }

    return best;
}

ePlaceResult engine_place( int32_t table[NUM_ROWS][ROW_CAP], int32_t card, int32_t *row )
{
    int32_t best = engine_find_row( table, card );

    *row = best;
    if( best == -1 )
        return PLACE_LOW;

    int32_t len = engine_row_len( table, best );
    if( len == ROW_CAP )
        return PLACE_FULL;

    table[best][len] = card;
    return PLACE_OK;
}

int32_t engine_collect( int32_t table[NUM_ROWS][ROW_CAP], int32_t row, int32_t card )
{
    assert( row >= 0 && row < NUM_ROWS );

    int32_t total = engine_row_score( table, row );

    memset( table[row], 0, sizeof( int32_t ) * ROW_CAP );
    table[row][0] = card;

    return total;
}
