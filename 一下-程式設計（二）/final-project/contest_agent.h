#ifndef CONTEST_AGENT_H
#define CONTEST_AGENT_H

#include <stdint.h>

// Agent Contest 規定的四個函式,函式名稱裡的 [ID] 是學號。
void CSIE_40847016S_setup( int32_t id );
void CSIE_40847016S_deal( const int32_t cards[10] );
int32_t CSIE_40847016S_pick( const int32_t table[4][5], const int32_t score[2], const int32_t last[2] );
int32_t CSIE_40847016S_choose( const int32_t table[4][5], int32_t card );

#endif
