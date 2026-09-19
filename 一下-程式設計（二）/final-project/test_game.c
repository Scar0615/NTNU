#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "agent.h"
#include "game.h"

static int32_t pick_first( const sGame *g, int32_t me )
{
    return g->player[me].hand[0];
}

static int32_t pick_bogus( const sGame *g, int32_t me )
{
    (void)g;
    (void)me;
    return 999;
}

static int32_t choose_row2( const sGame *g, int32_t me, int32_t card )
{
    (void)g;
    (void)me;
    (void)card;
    return 2;
}

static int32_t choose_bogus( const sGame *g, int32_t me, int32_t card )
{
    (void)g;
    (void)me;
    (void)card;
    return 7;
}

// README.md「放牌規則」範例裡的桌面,兩位玩家各兩張手牌
static void scripted_game( sGame *g )
{
    int32_t t[NUM_ROWS][ROW_CAP] = {
        {  8, 12,  0,  0,  0 },
        { 23, 31,  0,  0,  0 },
        { 40,  0,  0,  0,  0 },
        { 52, 58, 62, 66, 70 }
    };

    assert( game_init( g, 2 ) == 0 );
    memcpy( g->table, t, sizeof( t ) );

    g->player[0].hand[0] = 35;  g->player[0].hand[1] = 90;  g->player[0].hand_len = 2;
    g->player[1].hand[0] = 5;   g->player[1].hand[1] = 75;  g->player[1].hand_len = 2;

    for( int32_t p = 0 ; p < 2 ; p++ )
    {
        g->player[p].pick = pick_first;
        g->player[p].choose = choose_row2;
    }
}

static void test_scripted( void )
{
    sGame g;
    sMove moves[MAX_PLAYERS];

    scripted_game( &g );

    // 第 1 回合:玩家0 出 35、玩家1 出 5。5 先放,比所有列都小 -> 收列2(3 分)。35 再接在列1。
    assert( game_play_round( &g, moves ) == 2 );
    assert( moves[0].player == 1 && moves[0].card == 5 );
    assert( moves[0].result == PLACE_LOW && moves[0].row == 2 && moves[0].penalty == 3 );
    assert( moves[1].player == 0 && moves[1].card == 35 );
    assert( moves[1].result == PLACE_OK && moves[1].row == 1 && moves[1].penalty == 0 );
    assert( g.player[0].score == 0 && g.player[1].score == 3 );
    assert( g.last[0] == 35 && g.last[1] == 5 );
    assert( g.player[0].hand_len == 1 && g.player[1].hand_len == 1 );
    assert( g.table[1][2] == 35 && g.table[2][0] == 5 && g.table[2][1] == 0 );

    // 第 2 回合:玩家0 出 90、玩家1 出 75。75 先放,列3 已滿 -> 收 11 分。90 再接在新的列3。
    assert( game_play_round( &g, moves ) == 2 );
    assert( moves[0].player == 1 && moves[0].card == 75 );
    assert( moves[0].result == PLACE_FULL && moves[0].row == 3 && moves[0].penalty == 11 );
    assert( moves[1].player == 0 && moves[1].card == 90 );
    assert( moves[1].result == PLACE_OK && moves[1].row == 3 );
    assert( g.player[0].score == 0 && g.player[1].score == 14 );
    assert( g.table[3][0] == 75 && g.table[3][1] == 90 );
    assert( g.round == 2 && g.cheater == -1 );
}

static void test_cheaters( void )
{
    sGame g;
    sMove moves[MAX_PLAYERS];

    // 出不在手牌裡的牌
    scripted_game( &g );
    g.player[1].pick = pick_bogus;
    assert( game_play_round( &g, moves ) == -1 );
    assert( g.cheater == 1 );

    // 選了不存在的列
    scripted_game( &g );
    g.player[1].choose = choose_bogus;      // 玩家1 出 5,會走到 choose
    assert( game_play_round( &g, moves ) == -1 );
    assert( g.cheater == 1 );
}

// Level 2 應該避開會成為第 6 張的牌
static void test_safe_agent_avoids_full_row( void )
{
    int32_t table[NUM_ROWS][ROW_CAP] = {
        { 10,  0,  0,  0,  0 },
        { 20, 21, 22, 23, 24 },     // 已滿(3+1+5+1+1 = 11 牛頭),落在 25~49 的牌會變成第 6 張
        { 50,  0,  0,  0,  0 },
        { 90,  0,  0,  0,  0 }
    };
    int32_t hand[2] = { 30, 60 };
    uint8_t known[NUM_CARDS + 1] = {0};

    for( int32_t r = 0 ; r < NUM_ROWS ; r++ )
        for( int32_t c = 0 ; c < engine_row_len( table, r ) ; c++ )
            known[table[r][c]] = 1;
    known[30] = known[60] = 1;

    assert( agent_safe_pick_core( table, hand, 2, known, 2 ) == 60 );
    assert( agent_cheapest_row( table ) == 0 );     // 列 0、2、3 都是單張 3 牛頭,同分取編號小的
}

// 隨機對打整局,檢查各種守恆
static void test_random_games( void )
{
    for( int32_t n = MIN_PLAYERS ; n <= MAX_PLAYERS ; n++ )
    {
        for( int32_t game = 0 ; game < 200 ; game++ )
        {
            sGame g;
            sMove moves[MAX_PLAYERS];

            assert( game_init( &g, n ) == 0 );

            int32_t expect_heads = 0;
            for( int32_t p = 0 ; p < n ; p++ )
            {
                // 偶數座位用 Level 2、奇數座位用 Level 1,兩種 AI 都要出合法的牌
                g.player[p].pick = ( p % 2 == 0 ) ? agent_safe_pick : agent_random_pick;
                g.player[p].choose = ( p % 2 == 0 ) ? agent_safe_choose : agent_min_heads_choose;
                for( int32_t k = 0 ; k < HAND_SIZE ; k++ )
                {
                    expect_heads += engine_score( g.player[p].hand[k] );
                    if( k > 0 )
                        assert( g.player[p].hand[k - 1] < g.player[p].hand[k] );    // 手牌有排序
                }
            }
            for( int32_t r = 0 ; r < NUM_ROWS ; r++ )
                expect_heads += engine_score( g.table[r][0] );

            while( !game_is_over( &g ) )
            {
                assert( game_play_round( &g, moves ) == n );
                for( int32_t i = 1 ; i < n ; i++ )
                    assert( moves[i - 1].card < moves[i].card );    // 由小到大放
            }

            int32_t total_score = 0, table_heads = 0;
            for( int32_t p = 0 ; p < n ; p++ )
            {
                assert( g.player[p].hand_len == 0 );
                total_score += g.player[p].score;
            }
            for( int32_t r = 0 ; r < NUM_ROWS ; r++ )
                table_heads += engine_row_score( g.table, r );

            // 大家吃到的牛頭 + 桌面剩下的牛頭 = 一開始所有牌的牛頭
            assert( total_score + table_heads == expect_heads );
        }
    }
}

int main( void )
{
    srand( time( NULL ) );

    test_scripted();
    test_cheaters();
    test_safe_agent_avoids_full_row();
    test_random_games();

    printf( "All game tests passed.\n" );
    return 0;
}
