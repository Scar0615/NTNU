#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "agent.h"
#include "contest_agent.h"
#include "engine.h"

// 擂台:讓參賽的 AI 兩兩對打,每組打 N 局,分數低者贏一局。
// 每位參賽者都用 Agent Contest 規定的四個函式,跟助教的評測程式介面一致。

typedef struct _sContestant
{
    const char  *name;
    void        ( *setup )( int32_t id );
    void        ( *deal )( const int32_t cards[10] );
    int32_t     ( *pick )( const int32_t table[4][5], const int32_t score[2], const int32_t last[2] );
    int32_t     ( *choose )( const int32_t table[4][5], int32_t card );
} sContestant;

// ---- 參賽者 1:Level 1(隨機出牌) ----

static int32_t rnd_hand[HAND_SIZE];
static int32_t rnd_len;

static void rnd_setup( int32_t id )
{
    (void)id;
    rnd_len = 0;
}

static void rnd_deal( const int32_t cards[10] )
{
    memcpy( rnd_hand, cards, sizeof( rnd_hand ) );
    rnd_len = HAND_SIZE;
}

static int32_t rnd_pick( const int32_t table[4][5], const int32_t score[2], const int32_t last[2] )
{
    (void)table;
    (void)score;
    (void)last;

    int32_t i = rand() % rnd_len;
    int32_t card = rnd_hand[i];
    rnd_hand[i] = rnd_hand[--rnd_len];
    return card;
}

static int32_t rnd_choose( const int32_t table[4][5], int32_t card )
{
    (void)card;
    return agent_cheapest_row( table );
}

// ---- 一局比賽 ----

static int32_t in_hand( const int32_t *hand, int32_t len, int32_t card )
{
    for( int32_t k = 0 ; k < len ; k++ )
        if( hand[k] == card )
            return k;
    return -1;
}

// 打一局。回傳勝者(0 / 1),平手回傳 -1。score 是最後分數。
// 有人出不合法的牌或選不合法的列就直接判輸,並把 *cheater 設為該座位(否則 -1)。
static int32_t play_game( const sContestant *c[2], int32_t score[2], int32_t *cheater )
{
    int32_t hands[2][HAND_SIZE];
    int32_t hand_len[2] = { HAND_SIZE, HAND_SIZE };
    int32_t table[4][5];
    int32_t last[2] = { 0, 0 };

    *cheater = -1;
    score[0] = score[1] = 0;
    engine_deal( hands, 2, table );

    for( int32_t p = 0 ; p < 2 ; p++ )
        c[p]->setup( p );
    for( int32_t p = 0 ; p < 2 ; p++ )
        c[p]->deal( hands[p] );

    for( int32_t round = 0 ; round < HAND_SIZE ; round++ )
    {
        int32_t played[2];

        for( int32_t p = 0 ; p < 2 ; p++ )
        {
            played[p] = c[p]->pick( table, score, last );
            if( in_hand( hands[p], hand_len[p], played[p] ) < 0 )
            {
                *cheater = p;
                return 1 - p;
            }
        }
        for( int32_t p = 0 ; p < 2 ; p++ )
        {
            int32_t idx = in_hand( hands[p], hand_len[p], played[p] );
            hands[p][idx] = hands[p][--hand_len[p]];
            last[p] = played[p];
        }

        int32_t first = ( played[0] < played[1] ) ? 0 : 1;
        for( int32_t turn = 0 ; turn < 2 ; turn++ )
        {
            int32_t p = ( turn == 0 ) ? first : 1 - first;
            int32_t row;
            ePlaceResult res = engine_place( table, played[p], &row );

            if( res == PLACE_LOW )
            {
                row = c[p]->choose( table, played[p] );
                if( row < 0 || row >= NUM_ROWS )
                {
                    *cheater = p;
                    return 1 - p;
                }
            }
            if( res != PLACE_OK )
                score[p] += engine_collect( table, row, played[p] );
        }
    }

    if( score[0] < score[1] )
        return 0;
    if( score[0] > score[1] )
        return 1;
    return -1;
}

int main( int argc, char *argv[] )
{
    int32_t games = 1000;

    if( argc > 1 )
        games = atoi( argv[1] );
    if( games < 1 )
    {
        printf( "用法:%s [每組對戰局數]\n", argv[0] );
        return 1;
    }

    srand( time( NULL ) );

    const sContestant contestants[] = {
        { "Level1-隨機", rnd_setup, rnd_deal, rnd_pick, rnd_choose },
        { "Level2-算風險", CSIE_40847016S_setup, CSIE_40847016S_deal, CSIE_40847016S_pick, CSIE_40847016S_choose },
    };
    const int32_t n = sizeof( contestants ) / sizeof( contestants[0] );

    int32_t total_wins[16] = {0};

    printf( "=== Agent Contest:每組 %d 局,分數低者贏 ===\n", games );

    // 注意:每位參賽者用 static 變數記手牌,所以不能自己打自己
    for( int32_t i = 0 ; i < n - 1 ; i++ )
    {
        for( int32_t j = i + 1 ; j < n ; j++ )
        {
            const sContestant *pair[2] = { &contestants[i], &contestants[j] };
            int32_t wins[2] = {0}, ties = 0, cheats[2] = {0};
            double sum[2] = {0};

            for( int32_t k = 0 ; k < games ; k++ )
            {
                int32_t score[2], cheater;
                int32_t winner = play_game( pair, score, &cheater );

                if( winner < 0 )
                    ties++;
                else
                    wins[winner]++;
                if( cheater >= 0 )
                    cheats[cheater]++;
                sum[0] += score[0];
                sum[1] += score[1];
            }

            total_wins[i] += wins[0];
            total_wins[j] += wins[1];

            printf( "\n%s  vs  %s\n", pair[0]->name, pair[1]->name );
            printf( "  勝場: %d / %d(平手 %d)    勝率: %.1f%% / %.1f%%\n",
                    wins[0], wins[1], ties, 100.0 * wins[0] / games, 100.0 * wins[1] / games );
            printf( "  平均牛頭: %.2f / %.2f\n", sum[0] / games, sum[1] / games );
            if( cheats[0] || cheats[1] )
                printf( "  犯規次數: %d / %d\n", cheats[0], cheats[1] );
        }
    }

    printf( "\n--- 總勝場 ---\n" );
    for( int32_t i = 0 ; i < n ; i++ )
        printf( "  %s: %d\n", contestants[i].name, total_wins[i] );

    return 0;
}
