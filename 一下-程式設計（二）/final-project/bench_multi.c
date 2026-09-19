#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "agent.h"
#include "game.h"

// 多人局測試:1 個 Level 2(座位 0)對 n-1 個 Level 1,看 Level 2 是否穩定領先。
// 「單獨最低」是指 Level 2 的牛頭數嚴格小於所有人,平手不算。

#define GAMES (20000)

int main( void )
{
    srand( time( NULL ) );

    const int32_t sizes[] = { 2, 3, 4, 7, 10 };

    for( int32_t s = 0 ; s < (int32_t)( sizeof( sizes ) / sizeof( sizes[0] ) ) ; s++ )
    {
        int32_t n = sizes[s];
        double smart_sum = 0.0;
        double random_sum = 0.0;
        int32_t sole_lowest = 0;

        for( int32_t k = 0 ; k < GAMES ; k++ )
        {
            sGame g;
            sMove moves[MAX_PLAYERS];

            game_init( &g, n );
            for( int32_t p = 0 ; p < n ; p++ )
            {
                g.player[p].pick = ( p == 0 ) ? agent_safe_pick : agent_random_pick;
                g.player[p].choose = ( p == 0 ) ? agent_safe_choose : agent_min_heads_choose;
            }

            while( !game_is_over( &g ) )
                game_play_round( &g, moves );

            int32_t beaten = 0;
            smart_sum += g.player[0].score;
            for( int32_t p = 1 ; p < n ; p++ )
            {
                random_sum += g.player[p].score;
                if( g.player[0].score < g.player[p].score )
                    beaten++;
            }
            if( beaten == n - 1 )
                sole_lowest++;
        }

        printf( "n=%2d  Level2 平均 %.2f | 隨機平均 %.2f | Level2 單獨最低 %.1f%%(隨機水準 %.1f%%)\n",
                n, smart_sum / GAMES, random_sum / GAMES / ( n - 1 ),
                100.0 * sole_lowest / GAMES, 100.0 / n );
    }

    return 0;
}
