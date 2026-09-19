#include <string.h>
#include "game.h"

static int32_t find_in_hand( const sPlayer *p, int32_t card )
{
    for( int32_t k = 0 ; k < p->hand_len ; k++ )
        if( p->hand[k] == card )
            return k;
    return -1;
}

static void remove_from_hand( sPlayer *p, int32_t idx )
{
    for( int32_t k = idx ; k < p->hand_len - 1 ; k++ )
        p->hand[k] = p->hand[k + 1];
    p->hand_len--;
}

int32_t game_init( sGame *g, int32_t n_players )
{
    int32_t hands[MAX_PLAYERS][HAND_SIZE];

    if( engine_deal( hands, n_players, g->table ) != 0 )
        return -1;

    memset( g->last, 0, sizeof( g->last ) );
    memset( g->seen, 0, sizeof( g->seen ) );
    for( int32_t r = 0 ; r < NUM_ROWS ; r++ )
        g->seen[g->table[r][0]] = 1;
    g->n_players = n_players;
    g->round = 0;
    g->cheater = -1;

    for( int32_t p = 0 ; p < n_players ; p++ )
    {
        sPlayer *pl = &g->player[p];

        pl->name = "";
        pl->score = 0;
        pl->pick = NULL;
        pl->choose = NULL;
        pl->hand_len = HAND_SIZE;

        // 插入排序,手牌由小到大
        for( int32_t k = 0 ; k < HAND_SIZE ; k++ )
        {
            int32_t card = hands[p][k];
            int32_t j = k - 1;
            while( j >= 0 && pl->hand[j] > card )
            {
                pl->hand[j + 1] = pl->hand[j];
                j--;
            }
            pl->hand[j + 1] = card;
        }
    }

    return 0;
}

int32_t game_play_round( sGame *g, sMove moves[] )
{
    int32_t n = g->n_players;

    // 1. 所有人同時出牌:先全部選完、驗證,再一起從手牌拿掉
    for( int32_t p = 0 ; p < n ; p++ )
    {
        int32_t card = g->player[p].pick( g, p );
        if( find_in_hand( &g->player[p], card ) < 0 )
        {
            g->cheater = p;
            return -1;
        }
        moves[p].player = p;
        moves[p].card = card;
    }

    for( int32_t p = 0 ; p < n ; p++ )
    {
        remove_from_hand( &g->player[p], find_in_hand( &g->player[p], moves[p].card ) );
        g->last[p] = moves[p].card;
        g->seen[moves[p].card] = 1;
    }

    // 2. 依牌號由小到大排序,決定放牌順序
    for( int32_t i = 1 ; i < n ; i++ )
    {
        sMove key = moves[i];
        int32_t j = i - 1;
        while( j >= 0 && moves[j].card > key.card )
        {
            moves[j + 1] = moves[j];
            j--;
        }
        moves[j + 1] = key;
    }

    // 3. 依序放牌
    for( int32_t i = 0 ; i < n ; i++ )
    {
        sMove *m = &moves[i];
        sPlayer *pl = &g->player[m->player];
        int32_t row;

        m->result = engine_place( g->table, m->card, &row );
        m->penalty = 0;

        if( m->result == PLACE_LOW )
        {
            row = pl->choose( g, m->player, m->card );
            if( row < 0 || row >= NUM_ROWS )
            {
                g->cheater = m->player;
                return -1;
            }
        }

        if( m->result != PLACE_OK )
        {
            m->penalty = engine_collect( g->table, row, m->card );
            pl->score += m->penalty;
        }
        m->row = row;
    }

    g->round++;
    return n;
}

int32_t game_is_over( const sGame *g )
{
    return g->round >= HAND_SIZE;
}
