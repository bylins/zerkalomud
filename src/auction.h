/**************************************************************************
*   File: Auction.h                                  Part of Bylins       *
*  Usage: Auction headers functions used by the MUD                       *
*                                                                         *
*                                                                         *
*  $Author$                                                        *
*  $Date$                                           *
*  $Revision$                                                       *
************************************************************************ */

#ifndef _AUCTION_HPP_
#define _AUCTION_HPP_

typedef struct auction_data AUCTION_DATA;

struct auction_data
{
	int item_id;
	OBJ_DATA *item;
	int seller_unique;
	CHAR_DATA *seller;
	int buyer_unique;
	CHAR_DATA *buyer;
	int prefect_unique;
	CHAR_DATA *prefect;
	int cost;
	int tact;
};

/* Auction functions  ****************************************************/
void showlots(CHAR_DATA * ch);
bool auction_drive(CHAR_DATA * ch, char *argument);

void message_auction(char *message, CHAR_DATA * ch);
void clear_auction(int lot);
void sell_auction(int lot);
void trans_auction(int lot);
void check_auction(CHAR_DATA * ch, OBJ_DATA * obj);
void tact_auction(void);
AUCTION_DATA *free_auction(int *lotnum);
int obj_on_auction(OBJ_DATA * obj);

#define GET_LOT(value) ((auction_lots+value))

#endif
