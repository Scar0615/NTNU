#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "engine.h"

// README.md「放牌規則」範例裡的桌面
static void example_table( int32_t table[NUM_ROWS][ROW_CAP] )
{
    int32_t t[NUM_ROWS][ROW_CAP] = {
        {  8, 12,  0,  0,  0 },
        { 23, 31,  0,  0,  0 },
        { 40,  0,  0,  0,  0 },
        { 52, 58, 62, 66, 70 }
    };
    memcpy( table, t, sizeof( t ) );
}

static void test_score( void )
{
    assert( engine_score( 55 ) == 7 );
    assert( engine_score( 11 ) == 5 && engine_score( 99 ) == 5 );
    assert( engine_score( 10 ) == 3 && engine_score( 100 ) == 3 );
    assert( engine_score( 5 ) == 2 && engine_score( 95 ) == 2 );
    assert( engine_score( 1 ) == 1 && engine_score( 104 ) == 1 );

    int32_t total = 0;
    for( int32_t c = 1 ; c <= NUM_CARDS ; c++ )
        total += engine_score( c );
    assert( total == 171 );     // 104 張牌總共 171 個牛頭
}

static void test_place_and_collect( void )
{
    int32_t table[NUM_ROWS][ROW_CAP];
    int32_t row;

    // 35:比 31 大、比 40 小 -> 接在列1
    example_table( table );
    assert( engine_place( table, 35, &row ) == PLACE_OK );
    assert( row == 1 && engine_row_len( table, 1 ) == 3 && table[1][2] == 35 );

    // 75:最接近的是列3 的 70,但已滿 -> 第 6 張,收走 1+1+1+5+3 = 11 分
    example_table( table );
    assert( engine_place( table, 75, &row ) == PLACE_FULL );
    assert( row == 3 && engine_row_len( table, 3 ) == 5 );     // 還沒放入
    assert( engine_collect( table, row, 75 ) == 11 );
    assert( engine_row_len( table, 3 ) == 1 && table[3][0] == 75 );

    // 5:比所有列末尾都小 -> 自己選列2(只有 40,3 分)
    example_table( table );
    assert( engine_place( table, 5, &row ) == PLACE_LOW );
    assert( row == -1 );
    assert( engine_collect( table, 2, 5 ) == 3 );
    assert( engine_row_len( table, 2 ) == 1 && table[2][0] == 5 );
    assert( table[2][1] == 0 );                                // 舊牌有清乾淨
}

static void test_deal( void )
{
    for( int32_t n = MIN_PLAYERS ; n <= MAX_PLAYERS ; n++ )
    {
        int32_t hands[MAX_PLAYERS][HAND_SIZE];
        int32_t table[NUM_ROWS][ROW_CAP];
        int32_t seen[NUM_CARDS + 1] = {0};

        assert( engine_deal( hands, n, table ) == 0 );

        for( int32_t p = 0 ; p < n ; p++ )
            for( int32_t k = 0 ; k < HAND_SIZE ; k++ )
                seen[hands[p][k]]++;
        for( int32_t r = 0 ; r < NUM_ROWS ; r++ )
        {
            assert( engine_row_len( table, r ) == 1 );
            seen[table[r][0]]++;
        }

        int32_t dealt = 0;
        for( int32_t c = 1 ; c <= NUM_CARDS ; c++ )
        {
            assert( seen[c] <= 1 );         // 沒有重複的牌
            dealt += seen[c];
        }
        assert( seen[0] == 0 );
        assert( dealt == n * HAND_SIZE + NUM_ROWS );
    }

    int32_t hands[MAX_PLAYERS][HAND_SIZE];
    int32_t table[NUM_ROWS][ROW_CAP];
    assert( engine_deal( hands, 1, table ) == -1 );
    assert( engine_deal( hands, 11, table ) == -1 );
}

// 兩人隨機對打完整一局,檢查牌數與牛頭數守恆
static void test_full_game( void )
{
    int32_t hands[2][HAND_SIZE];
    int32_t table[NUM_ROWS][ROW_CAP];
    engine_deal( hands, 2, table );

    int32_t expect_heads = 0;
    for( int32_t p = 0 ; p < 2 ; p++ )
        for( int32_t k = 0 ; k < HAND_SIZE ; k++ )
            expect_heads += engine_score( hands[p][k] );
    for( int32_t r = 0 ; r < NUM_ROWS ; r++ )
        expect_heads += engine_score( table[r][0] );

    int32_t score[2] = {0};
    int32_t collected_cards = 0;

    for( int32_t round = 0 ; round < HAND_SIZE ; round++ )
    {
        // 隨機出牌(第 round 回合手上還剩 HAND_SIZE - round 張,已出的換到最後)
        int32_t played[2];
        for( int32_t p = 0 ; p < 2 ; p++ )
        {
            int32_t left = HAND_SIZE - round;
            int32_t pick = rand() % left;
            played[p] = hands[p][pick];
            hands[p][pick] = hands[p][left - 1];
        }

        // 小的先放
        for( int32_t turn = 0 ; turn < 2 ; turn++ )
        {
            int32_t p = ( played[0] < played[1] ) ? turn : 1 - turn;
            int32_t card = played[p];
            int32_t row;
            ePlaceResult res = engine_place( table, card, &row );

            if( res == PLACE_LOW )
            {
                // 選牛頭最少的一列
                int32_t best = 0, best_heads = 1000;
                for( int32_t r = 0 ; r < NUM_ROWS ; r++ )
                {
                    int32_t heads = 0;
                    for( int32_t c = 0 ; c < engine_row_len( table, r ) ; c++ )
                        heads += engine_score( table[r][c] );
                    if( heads < best_heads )
                    {
                        best_heads = heads;
                        best = r;
                    }
                }
                row = best;
            }

            if( res != PLACE_OK )
            {
                collected_cards += engine_row_len( table, row );
                score[p] += engine_collect( table, row, card );
            }
        }
    }

    int32_t table_cards = 0, table_heads = 0;
    for( int32_t r = 0 ; r < NUM_ROWS ; r++ )
    {
        table_cards += engine_row_len( table, r );
        for( int32_t c = 0 ; c < engine_row_len( table, r ) ; c++ )
            table_heads += engine_score( table[r][c] );
    }

    assert( table_cards + collected_cards == 2 * HAND_SIZE + NUM_ROWS );
    assert( score[0] + score[1] + table_heads == expect_heads );
}

int main( void )
{
    srand( time( NULL ) );

    test_score();
    test_place_and_collect();
    test_deal();
    for( int32_t g = 0 ; g < 1000 ; g++ )
        test_full_game();

    printf( "All tests passed.\n" );
    return 0;
}
