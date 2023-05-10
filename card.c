#include "card.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>



uint8_t Card_get_rank(Card_t* card) 
{
    return (card -> data) >> 2;
}

uint8_t Card_get_suit(Card_t* card) 
{
    return (card -> data) & 3 /*binary 11*/;
}

void Card_set_rank(Card_t* card, uint8_t rank) 
{
    (card -> data) = (card -> data) | (rank << 2);
}

void Card_set_suit(Card_t* card, uint8_t suit) 
{
    (card -> data) = (card -> data) | suit;
}

void init_list(CardList_t* list) {
    list -> head = NULL;
    list -> len = 0;
}


void add_card(CardList_t* list, Card_t* previous, Card_t* card) {
    // for the first time
    if(list -> head == NULL)
    {
        list -> head = card;
    } else {
        previous -> next = card;
    }
    list -> len ++;
}


void fill_deck(CardList_t* list) {
    //create and send 52 card to the deck
    Card_t * previous = NULL;
    Card_t * tmp = NULL;

    for(int i=2; i<=14; i++)
    {
        for(int j=0; j<4; j++)
        {
            int rank = i << 2;
            int suite = j;
            tmp = (Card_t *)calloc(sizeof(*tmp),1);
            tmp -> data = rank + suite;
            tmp -> next = NULL;

            add_card(list, previous, tmp);
            previous = tmp;
        }
    }
    //last card node
    previous = NULL;
}

void drew_and_add_card(CardList_t* deck, CardList_t * hand)
{
    //check if there is card in the deck
    if(deck->len < 1 )
    {
        printf("there is no enough card in the deck!!\n");
        return;
    }

    int rand_num = rand_num = 1 + rand() % (deck -> len );

    Card_t * pos = deck -> head;
    Card_t * prev_pos = NULL;
    // find the random card in the deck
    for(int i=1; i<rand_num; i++)
    {
        prev_pos = pos;
        pos = pos->next;
    }

    // find the last position in the player/dealer card list
    // if the first card was selected
    // cut the card from the decka and reduce the list size 
    if(prev_pos == NULL) {
        deck-> head = deck -> head -> next;
    } else {
        prev_pos -> next = pos -> next;
    }
    
    deck-> len --;
    pos -> next = NULL;

    Card_t* last_pos = hand -> head;
    for(; last_pos != NULL && last_pos -> next != NULL; last_pos = last_pos -> next);
    add_card(hand, last_pos, pos);
}


void print_rank(Card_t* card) {
    if(card==NULL){
        printf("there is no card! the value is null");
        return;
    }
    uint8_t rank_value = Card_get_rank(card);
    switch (rank_value) {
    case 11:
        printf("Jack");
        break;
    case 12:
        printf("Queen");
        break;
    case 13:
        printf("King");
        break;
    case 14:
        printf("Ace");
        break;
    default:
        printf("%d",rank_value);
        break;
    }
}


void print_suit(Card_t* card) {
    if(card == NULL)
    {
        printf("the is no card!\n the value is null");
        return;
    }
    uint8_t suit_value=Card_get_suit(card);
    //suit in [Spades, Hearts, Clubs and Diamonds]
    switch (suit_value)
    {
    case 0:
        printf(" of Spades");
        break;
    case 1:
        printf(" of Hearts");
        break;
    case 2:
        printf(" of Clubs");
        break;
    case 3:
        printf(" of Diamonds");
        break;
    default:
        printf("-Error!");
        break;
    }
}

int get_points(Card_t* card) {
    if(card == NULL)
    {
        printf("the is no card!\n the value is null");
        return -1;
    }
    uint8_t rank_value=Card_get_rank(card);
    if(rank_value == 14) {
        return 1;
    } else if (rank_value > 10 && rank_value < 14) {
        return 10;
    } else {
        return rank_value;
    }
}

int total_rank(CardList_t * list)
{
    int total_rank = 0;
    if(list == NULL  || list->head == NULL)
    {
        return 0;
    }

    Card_t* pos = list->head;
    int num_of_ace = 0;
    for(; pos!=NULL; pos=pos->next)
    {
        int rank=get_points(pos);
        //printf("\ntmp= %d\n",tmp);
        if(rank==1) {
            num_of_ace++;
        }
        total_rank+=rank;
    }
    for(int i = 0; num_of_ace > 0 && (total_rank + 10) <= 21; i++, num_of_ace--) {
        total_rank += 10;
    }
    return total_rank;
}
