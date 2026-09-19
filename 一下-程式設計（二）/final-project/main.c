#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "agent.h"
#include "game.h"

static void print_table( const sGame *g )
{
    for( int32_t r = 0 ; r < NUM_ROWS ; r++ )
    {
        printf( "  第 %d 列 |", r + 1 );
        for( int32_t c = 0 ; c < ROW_CAP ; c++ )
        {
            if( c < engine_row_len( g->table, r ) )
                printf( " %3d", g->table[r][c] );
            else
                printf( "   -" );
        }
        printf( "  | 共 %d 牛頭\n", engine_row_score( g->table, r ) );
    }
}

static void print_scores( const sGame *g )
{
    printf( "分數:" );
    for( int32_t p = 0 ; p < g->n_players ; p++ )
        printf( "  %s %d", g->player[p].name, g->player[p].score );
    printf( "\n" );
}

// 讀一行並轉成整數。遇到 EOF(Ctrl+D)就結束程式;不是整數時回傳 -1。
static int32_t read_number( const char *prompt )
{
    char line[64];
    int32_t value;

    printf( "%s", prompt );
    if( fgets( line, sizeof( line ), stdin ) == NULL )
    {
        printf( "\n bye!\n" );
        exit( 0 );
    }
    if( sscanf( line, "%d", &value ) != 1 )
        return -1;
    return value;
}

static int32_t human_pick( const sGame *g, int32_t me )
{
    const sPlayer *p = &g->player[me];

    printf( "\n" );
    print_scores( g );
    print_table( g );
    printf( "你的手牌:" );
    for( int32_t k = 0 ; k < p->hand_len ; k++ )
        printf( " %d(%d)", p->hand[k], engine_score( p->hand[k] ) );
    printf( "    (括號內是牛頭數)\n" );

    while( 1 )
    {
        int32_t card = read_number( "請輸入要出的牌:" );
        for( int32_t k = 0 ; k < p->hand_len ; k++ )
            if( p->hand[k] == card )
                return card;
        printf( "你手上沒有這張牌,請重新輸入。\n" );
    }
}

static int32_t human_choose( const sGame *g, int32_t me, int32_t card )
{
    (void)me;

    printf( "\n你出的 %d 比所有列的最後一張都小,必須收走一列:\n", card );
    print_table( g );

    while( 1 )
    {
        int32_t row = read_number( "請輸入要收走的列(1~4):" );
        if( row >= 1 && row <= NUM_ROWS )
            return row - 1;
        printf( "請輸入 1 到 4。\n" );
    }
}

static void print_move( const sGame *g, const sMove *m )
{
    const char *name = g->player[m->player].name;

    switch( m->result )
    {
        case PLACE_OK:
            printf( "  %s 出 %d -> 接在第 %d 列\n", name, m->card, m->row + 1 );
            break;
        case PLACE_FULL:
            printf( "  %s 出 %d -> 第 %d 列已滿,收走 5 張,吃 %d 牛頭\n", name, m->card, m->row + 1, m->penalty );
            break;
        case PLACE_LOW:
            printf( "  %s 出 %d -> 比所有列都小,收走第 %d 列,吃 %d 牛頭\n", name, m->card, m->row + 1, m->penalty );
            break;
    }
}

int main( int argc, char *argv[] )
{
    int32_t n_computers = 1;
    int32_t level = 1;

    if( argc > 1 )
        n_computers = atoi( argv[1] );
    if( argc > 2 )
        level = atoi( argv[2] );
    if( n_computers < 1 || n_computers > MAX_PLAYERS - 1 || level < 1 || level > 2 )
    {
        printf( "用法:%s [電腦數量 1~%d] [難度 1~2]\n", argv[0], MAX_PLAYERS - 1 );
        printf( "  難度 1:隨機出牌    難度 2:計算風險、記牌\n" );
        return 1;
    }

    srand( time( NULL ) );

    sGame g;
    sMove moves[MAX_PLAYERS];
    char names[MAX_PLAYERS][16];

    game_init( &g, n_computers + 1 );

    g.player[0].name = "你";
    g.player[0].pick = human_pick;
    g.player[0].choose = human_choose;
    for( int32_t p = 1 ; p <= n_computers ; p++ )
    {
        snprintf( names[p], sizeof( names[p] ), "電腦%d", p );
        g.player[p].name = names[p];
        g.player[p].pick = ( level == 1 ) ? agent_random_pick : agent_safe_pick;
        g.player[p].choose = ( level == 1 ) ? agent_min_heads_choose : agent_safe_choose;
    }

    printf( "=== Take 6! 誰是牛頭王 ===\n" );
    printf( "共 %d 位玩家(電腦難度 %d),打 %d 回合,牛頭最少的人獲勝。\n", g.n_players, level, HAND_SIZE );

    while( !game_is_over( &g ) )
    {
        printf( "\n=========== 第 %d / %d 回合 ===========", g.round + 1, HAND_SIZE );

        if( game_play_round( &g, moves ) < 0 )
        {
            printf( "\n%s 出了不合法的牌,犯規判負。\n", g.player[g.cheater].name );
            return 1;
        }

        printf( "\n本回合結果(由小到大):\n" );
        for( int32_t i = 0 ; i < g.n_players ; i++ )
            print_move( &g, &moves[i] );
    }

    printf( "\n=========== 遊戲結束 ===========\n" );
    print_scores( &g );

    int32_t best = g.player[0].score;
    for( int32_t p = 1 ; p < g.n_players ; p++ )
        if( g.player[p].score < best )
            best = g.player[p].score;

    printf( "勝者:" );
    for( int32_t p = 0 ; p < g.n_players ; p++ )
        if( g.player[p].score == best )
            printf( " %s", g.player[p].name );
    printf( "(%d 牛頭)\n", best );

    return 0;
}
