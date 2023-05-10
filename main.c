
#include "card.h"
#include <time.h>
#include <stdbool.h>
#include <stdio.h>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

// the function get the cash and the current bet of the player and ask the player how much to bet and update the current bet if seccess return 1 else return 0
int chose_bet(int *cash, int *pot, bool * is_first_game);

//the function print the detail of current status of the game like  current cards of each player, How many cards are left and the total point of each player
void show_status(CardList_t * deck, CardList_t * dealer, CardList_t *player, bool expose_dealer_cards, int * cash, int * pot);

// the function get pointer to the deck, pointer to the list card of the player and the dealer and the money of the player and the current bet,  
//The function asks for hit or stand as long as the player has not passed 21 or chooses to 's' to stand, if the player lose the function return 0 if not return 1
int hit_or_stand_player(CardList_t* deck, CardList_t* player, CardList_t* dealer,int * cash,  int* pot);

//The function handles the drawing of the dealer's cards, until he reaches 17 or higher, and calculates whether he lost or won
void dealer_drew(CardList_t* deck, CardList_t* player, CardList_t* dealer, int* cash, int* pot);

// The function asks the player if he wants to continue betting or stop, the function return 0 if the player select to stop the fame or 1 if he wants to continue betting
int bet_again();

int main(void) {

    srand(time(NULL));
    CardList_t deck;
    init_list(&deck);
    fill_deck(&deck);

    CardList_t dealer_hand;
    init_list(&dealer_hand);

    CardList_t player_hand;
    init_list(&player_hand);

    int cash = 1000;
    int pot = 0;
    bool is_first_game=true;

   
    while(1) {

        if(chose_bet(&cash, &pot,&is_first_game) < 0) {
            break;
        }
            
        printf("\n2 card for your and on 2 card for the dealer:\n");
        drew_and_add_card(&deck, &dealer_hand);
        drew_and_add_card(&deck, &dealer_hand);

        drew_and_add_card(&deck, &player_hand);
        drew_and_add_card(&deck, &player_hand);
        show_status(&deck,&dealer_hand, &player_hand, false,&cash, &pot);

        //Black Jack check
        printf("\n\n");
        int players_score = total_rank(&player_hand);
        if(players_score == 21)
        {
            reset_cards(&deck,&dealer_hand,&player_hand);
            printf("Black Jack --> you win in this round  %d \n\n",(int)(pot*2.5-pot) );
            cash+=pot*1.5;
            continue;
        }
        else if(players_score > 21 )
        {
            reset_cards(&deck, &dealer_hand, &player_hand);
            printf("you ”Bust” --> you los in this round  %d\n\n",pot);
            pot=0;
            continue;
        }
        else
        {
           if(!hit_or_stand_player(&deck, &player_hand, &dealer_hand, &cash, &pot))
                continue;
            dealer_drew(&deck, &player_hand, &dealer_hand, &cash, &pot);
        }

        reset_cards(&deck,&dealer_hand,&player_hand);


    }

    //free all cards
    Card_t * delete_pos=NULL,*  next_pos= deck.head;
    while(next_pos!=NULL)
    {
        delete_pos=next_pos;
        next_pos=next_pos->next;
        free(delete_pos);
    }

    return 0;
}


//functions

int chose_bet(int *cash, int *pot, bool * is_first_game)
{

    if(*pot + *cash < 10)
    {
        printf("your total money less then 10$, sorry game over!\n");
        return -1;
    }
    if(*is_first_game)
    {
        printf("Wellcom to Black-Jack-Game\n");
    }
    else
    {
        if(bet_again()==0)
            return -1;
    }

    if(*pot!=0)
    {
        printf("your  cash= %d , and current pot= %d$\nplease Type how much you want to add to pot or type 0 to keep it without change: ",*cash,*pot  );
        int add_pot;
        scanf("%d",&add_pot);
        while (getchar() != '\n');
        while((add_pot+*pot)<10 || (add_pot+*pot)>*cash || (add_pot+*pot)%10!=0)
        {
            if((add_pot+*pot)<10) {
                printf("the bets cann't be under 10$\n");
            }
            else if((add_pot+*pot)>*cash)
                printf("you have only %d$\n",*cash);
            else 
            {
                printf("Bets must be made in multiples of 10$\n");
            }
            //system("clear");
            printf("your  cash= %d , and current pot= %d$\nplease Type how much you want to add to pot or type 0 to keep it without change: ",*cash,*pot  );
            scanf("%d",&add_pot);
            while(getchar() !='\n');
        }
        *cash -= add_pot;
        *pot += add_pot;   
        *is_first_game=false;
        return 0;
    }

    else
    {
        printf("\n\n");
        printf("your  cash= %d , and current pot= %d$\nplease type how much money to bet: ",*cash,*pot  );
        int new_pot;
        scanf("%d",&new_pot);
        while (getchar() != '\n');
        

        while(new_pot<10 || new_pot>*cash || new_pot%10!=0)
        {
            if(new_pot<10) {
                printf("the bets cann't be under 10$\n");
            }
            else if(new_pot>*cash)
                printf("you have only %d$\n",*cash);
            else 
            {
                printf("Bets must be made in multiples of 10$\n");
            }
            //system("clear");
            printf("please place your bets again \(10$ - %d$: \)", *cash/10*10);
            scanf("%d",&new_pot);
            while(getchar() !='\n');
        }
        *cash -= new_pot;
        *pot = new_pot;   
        *is_first_game=false;
        return 0;
    }
}

int hit_or_stand_player(CardList_t* deck, CardList_t* player, CardList_t* dealer,int * cash,  int* pot) 
{
    int players_score = total_rank(player);
    while(1) 
    {
        //ask Hit or Stand

        //Input validation
        char choice = 'a';
        while (choice != 'h' && choice != 's' && choice != 'H' && choice != 'S')
        {
            printf("Hit or Stand?\npress 'h' for hit or 's' to stand:\n");
            scanf(" %c",&choice);
            while (getchar() != '\n');
            if (choice != 'h' && choice != 's' && choice != 'H' && choice != 'S') {
                printf("Unknown command\n");
            }
        }

        //In case player choose "hit"
        
        if((choice == 'h' || choice == 'H') && players_score <21)
        {
            drew_and_add_card(deck, player);
            show_status(deck,dealer, player, false,cash,pot);

            players_score = total_rank(player);
            if(players_score > 21) 
            {
                printf("you ”Bust” --> you los in this round   %d\n\n", *pot);
                *pot = 0;
                reset_cards(deck, dealer, player);
                return 0;
            }
            else if(players_score == 21)
            {
                return 1;
            }

    //In case player choose "stand"

        } else if(choice == 's' || choice == 'S') {
            return 1;
        }
    }
    return 1;
}

void dealer_drew(CardList_t* deck, CardList_t* player, CardList_t* dealer, int* cash, int* pot){
    show_status(deck,dealer, player, true,cash,pot);

    //dealer drew
    int dealer_score = total_rank(dealer);
    int player_score = total_rank(player);
    while (dealer_score<17)
    {
        drew_and_add_card(deck, dealer);
        show_status(deck,dealer, player, true,cash,pot);
        dealer_score = total_rank(dealer);
    }
    
    //player win
    if(dealer_score > 21  || dealer_score < player_score)
    {
        printf("dealer ”Bust” --> you win in this round  %d$\n\n", *pot);
        *cash += *pot;
    }

    //dealer win
    else if(dealer_score > player_score)
    {
        printf("you lose --> you lose in this round  %d$\n\n", *pot);
        *pot = 0;
    }

    //push
    else
    {
        printf("”push” --> you and the dealer have the same point value\n");
    }
}

void reset_cards(CardList_t * deck, CardList_t * dealer, CardList_t *player)
{
    while(dealer!=NULL && dealer->head!=NULL)
    {
        Card_t * pos = dealer->head;
        //cut the the first card from the dealer hand
        dealer->head=dealer->head->next;
        dealer->len--;

        //conect the card to the head of the deck 
        pos->next = deck->head;
        deck->head=pos;
        deck->len++;
    }

    while(player!=NULL && player->head!=NULL)
    {
        Card_t * pos = player->head;
        //cut the the first card from the player hand
        player->head=player->head->next;
        player->len--;

        //conect the card to the head of the deck 
        pos->next = deck->head;
        deck->head=pos;
        deck->len++;
    }

    //chang the flage is_first_game to false
}

void show_status(CardList_t * deck, CardList_t * dealer, CardList_t *player, bool expose_dealer_cards, int * cash, int * pot)
{
    system("clear");

    //money status
    printf("=========================================Black-Jack-Game===============================\n");
    printf("cash = %d $               pot = %d$               cards in deck = %d/52\n",*cash,*pot,deck->len);
    printf("___________               ___________             _________________  \n\n");   
    // dealer part
    printf("-----------------------------------dealer cards-------------------------------------\n\n");   
    //printf("dealer: ");
    if(expose_dealer_cards) {
        for(Card_t* pos = dealer->head; pos!=NULL; pos=pos->next)
        {
            print_rank(pos);
            print_suit(pos);
            printf("    ");
        }
    } else if (dealer->head != NULL) {
        print_rank(dealer->head);
        print_suit(dealer->head);
        printf("    ????????");
    }
        //dealer points
    printf("\n\n\npoints: ");
    if(!expose_dealer_cards)
    {
        printf("%d + ???",get_points(dealer->head));
    }
    else
    {
        printf("%d",total_rank(dealer));
    }
    printf("\n");

    //player part
    printf("-----------------------------------your cards-------------------------------------\n\n");   
    //printf("player: ");
    for(Card_t* pos = player->head; pos!=NULL; pos=pos->next)
    {
        print_rank(pos);
        print_suit(pos);
        printf("    ");
    }
    printf("\n\n");


    //player points
    printf("\npoints: %d",total_rank(player));
    printf("\n");
    printf("=========================================Black-Jack-Game===============================\n");

}



int bet_again() 
{
       
        char choice = 'a';
        while (choice != 'n' && choice != 'N' && choice != 'y' && choice != 'Y')
        {
            printf("\ndo you want to bet again? (yes/no)\n press 'y' to yes or 'n' to no: ");
            scanf(" %c",&choice);
            while (getchar() != '\n');
            if (choice != 'n' && choice != 'N' && choice != 'Y' && choice != 'y') {
                printf("Unknown command\n");
            }
        }
        //In case player choose "yes"
        
        if(choice == 'y' || choice == 'Y')
        {
            return 1;
        }
        else
        {
            return 0;
        }
}