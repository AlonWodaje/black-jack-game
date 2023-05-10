#ifndef CARD_H
#define CARD_H
#include <stdint.h>
#include <stdlib.h>





//card struct
typedef struct Card
{
    //[1:0] are used to represent the suit, the next 4 bits [5:2] are used to represent the rank (bits[7:6] unused)
    uint8_t data;
    struct Card* next;
} Card_t;


// card list struct 
typedef struct CardList 
{
    Card_t* head;
    size_t len;
} CardList_t;


//the function get cardList pointer and initialize its length to 0, and the pointer head to null
void init_list(CardList_t* list);

//the function get cardList pointer and create in loop the  
void fill_deck(CardList_t* list);

//the function get pointer to the deck and a pointer to card list , genereted number between 1-52 and add the card from the deck list to the 
void drew_and_add_card(CardList_t* deck, CardList_t* hand);
//the function get pointer to card and print it rank
void print_rank(Card_t* card);
//the function get pointer to card and print it suit
void print_suit(Card_t* card);
//the function get pointer to card and return its point value
int get_points(Card_t* card);
//the fuction get pointer to card list and return the total points of the cards
int total_rank(CardList_t * list);
//the function get pointer to card and return the  number that represent the rank of the card
uint8_t Card_get_rank(Card_t* card);

//the function get pointer to card and return the  number that represent the suit of the card
uint8_t Card_get_suit(Card_t* card);

//the function get pointer to card and rank , and set the card rank
void Card_set_rank(Card_t* card, uint8_t rank);

//the function get pointer to card and suit , and set the card suit
void Card_set_suit(Card_t* card, uint8_t suit);

//the function get pointer to the card deck, and 2 pointer of card list the represent the player and the dealer, the function return all the card from the both player to the deck (return to the head of the deck).
void reset_cards(CardList_t * deck, CardList_t * dealer, CardList_t *player);


#endif  //CARD_H