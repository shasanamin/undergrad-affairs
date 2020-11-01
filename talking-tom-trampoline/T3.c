#include "T3.h"

#include <stdlib.h>
#define R3 PIN_D7
#define R2 PIN_D6
#define R1 PIN_D5
#define R0 PIN_D4
#define EN PIN_D3
#define C2 PIN_D2
#define C1 PIN_D1
#define C0 PIN_D0
// using pin_A1, pin_A2 (buttons) to move trampoline left and right. see update_tramp() 

int i;
int score = 0;
int rebounds = 0;
int tramp_buffer = 0;
long tom_buffer = 0;
int tramp_coord[6] = {2, 0, 3, 0, 4, 0};  // tramp_coord = [col1, row1, col2, row2, col3, row3]  col and row are interchangeable depending on how you configure your LED matrix
int tom_coord[6] = {3, 1, 3, 2, 3, 3};
int tom_movement = 0; //0=up;1=up_left;2=up_right;3=down;4=down_left;5=down_right
int tom_dead = 0;  // tom_dead is an indicator variable. when this is set to 1, it means tom did not land on trampoline and died. game over. need this to exit rtos.

int rand_num;
// this function generates a number from 0 to 2. this dictates tom's movement direction when it bounces of trampoline (would be no fun if just goes up and down)
int rand_gen() {
 regen: // regenerate from range 0 and 2
 rand_num = (int)(rand()/10000);
 if (rand_num>2) goto regen;  // unsigned int so don't need check for 0
 return rand_num;
}

int tom_fell(){
   if (tramp_coord[0] != tom_coord[0] && tramp_coord[2] != tom_coord[0] && tramp_coord[4] != tom_coord[0]){
      return 1;
   }
   return 0;  
}

void move_tramp_right(){
   if (tramp_coord[4] < 7){ // stay within bounds
      tramp_coord[0]++;
      tramp_coord[2]++;
      tramp_coord[4]++;
   }
}

void move_tramp_left(){
   if (tramp_coord[0] > 0){ // stay within bounds
      tramp_coord[0]--;
      tramp_coord[2]--;
      tramp_coord[4]--;
   }
}

void move_tom_up(){
   if (tom_coord[5] < 15){ // stay within bounds
      tom_coord[1]++;
      tom_coord[3]++;
      tom_coord[5]++;
   }
   else{
      tom_movement = 3;
   }
}

void move_tom_up_left(){
   if (tom_coord[5] < 15 && tom_coord[0] > 0){ // stay within bounds
      tom_coord[1]++;
      tom_coord[3]++;
      tom_coord[5]++;
      
      tom_coord[0]--;
      tom_coord[2]--;
      tom_coord[4]--;
   }
   else if (tom_coord[5] >= 15){  // go down_left
      tom_movement = 4;
   }
   else{ // go up_right
      tom_movement = 2;
   }
   
}

void move_tom_up_right(){
   if (tom_coord[5] < 15 && tom_coord[4] < 7){ // stay within bounds
      tom_coord[1]++;
      tom_coord[3]++;
      tom_coord[5]++;   

      tom_coord[0]++;
      tom_coord[2]++;
      tom_coord[4]++;
   }
   else if (tom_coord[5] >= 15){  // go down_right
      tom_movement = 5;
   }
   else{  // go up_left
      tom_movement = 1;
   }
}

void move_tom_down(){
   if (tom_coord[1] > 1){ // stay within bounds
      tom_coord[1]--;
      tom_coord[3]--;
      tom_coord[5]--;
   }
   else if (tom_coord[1] == 1){ // move and check whether game over
      tom_coord[1]--;
      tom_coord[3]--;
      tom_coord[5]--;
      if (tom_fell() == 1){
         tom_dead = 1;
      }
      else{
         score++;
         rebounds++;
         tom_movement = rand_gen();
      }
   }
}

void move_tom_down_left(){
   if (tom_coord[1] > 1 && tom_coord[0] > 0){ // stay within bounds
      tom_coord[1]--;
      tom_coord[3]--;
      tom_coord[5]--;   
      
      tom_coord[0]--;
      tom_coord[2]--;
      tom_coord[4]--;
   }
   else if (tom_coord[1] == 1){ // move and check whether game over
      tom_coord[1]--;
      tom_coord[3]--;
      tom_coord[5]--;
      if (tom_fell() == 1){
         tom_dead = 1;
      }
      else{
         score++;
         rebounds++;
         tom_movement = rand_gen();
      }
   }
   else{
      tom_movement = 5;   
   }
}

void move_tom_down_right(){
   if (tom_coord[1] > 1 && tom_coord[4] < 7){ // stay within bounds
      tom_coord[1]--;
      tom_coord[3]--;
      tom_coord[5]--;   
      
      tom_coord[0]++;
      tom_coord[2]++;
      tom_coord[4]++;
   }
   else if (tom_coord[1] == 1){ // move and check whether game over
      tom_coord[1]--;
      tom_coord[3]--;
      tom_coord[5]--;
      if (tom_fell() == 1){
         tom_dead = 1;
      }
      else{
         score++;
         rebounds++;
         tom_movement = rand_gen();
      }
   }
   else{
      tom_movement = 4;
   }
}


// functions to switch on particular LED light
void RLED(int i) {
 if (bit_test (i, 0)) output_high(R0); else output_low(R0);
 if (bit_test (i, 1)) output_high(R1); else output_low(R1);
 if (bit_test (i, 2)) output_high(R2); else output_low(R2);
 if (bit_test (i, 3)) output_high(R3); else output_low(R3);
}
void CLED(int i) {
 if (bit_test (i, 0)) output_high(C0); else output_low(C0);
 if (bit_test (i, 1)) output_high(C1); else output_low(C1);
 if (bit_test (i, 2)) output_high(C2); else output_low(C2);
}
void led(int x1,int y1) {
 output_high(EN);
 RLED(y1);
 CLED(7-x1);
 output_low(EN);
} 


#use rtos(timer=0, minor_cycle = 50us)
#task (rate = 100us, max = 50us)
void update_tramp(){
   tramp_buffer++;   // buffering to slow movement down to observable limit. using very high freq. clock.
   if (tramp_buffer == 180){
      tramp_buffer = 0;
      if(input(PIN_A1) == 1){ // go right  
         move_tramp_right();
      }
      else if (input(PIN_A2) == 1){ //go left
         move_tramp_left();
      }
   }
}

#task (rate = 1000us, max = 50us)
void update_tom(){
   tom_buffer++;  // buffering to slow movement down to observable limit. using very high freq. clock.
   if (tom_buffer >= (40/(rebounds*1.1))){   // this allows non-linear increase in tom's speed upon jumping off (rebounding) trampoline each time
      tom_buffer = 0;
      if (tom_movement == 0){
         move_tom_up();
      }
      else if (tom_movement == 1){
         move_tom_up_left();
      }
      else if (tom_movement == 2){
         move_tom_up_right();
      }
      else if (tom_movement == 3){
         move_tom_down();
      }
      else if (tom_movement == 4){
         move_tom_down_left();
      }
      else if (tom_movement == 5){
         move_tom_down_right();
      }
      else{
         printf("Erroneous Movement");
      }
      if (tom_dead == 1){
         rtos_terminate();
      }
   }
}

/*#task (rate = 100us, max = 50us)
void display_scene(){
   led(tramp_coord[0], tramp_coord[1]);
   led(tramp_coord[2], tramp_coord[3]);
   led(tramp_coord[4], tramp_coord[5]);
   led(tom_coord[0], tom_coord[1]);
   led(tom_coord[2], tom_coord[3]);
   led(tom_coord[4], tom_coord[5]);
}*/

// displaying all LEDs in one rtos task resulted in one LED being switched on for a disproportionate time. 
// to make it appear as if multiple LEDs on, switch on each LED in separate task
#task (rate = 100us, max = 50us)
void ds1(){
   led(tramp_coord[0], tramp_coord[1]);
}

#task (rate = 100us, max = 50us)
void ds2(){
   led(tramp_coord[2], tramp_coord[3]);
}

#task (rate = 100us, max = 50us)
void ds3(){
   led(tramp_coord[4], tramp_coord[5]);
}

#task (rate = 100us, max = 50us)
void ds4(){
   led(tom_coord[0], tom_coord[1]);
}

#task (rate = 100us, max = 50us)
void ds5(){
   led(tom_coord[2], tom_coord[3]);
}

#task (rate = 100us, max = 50us)
void ds6(){
   led(tom_coord[4], tom_coord[5]);
}

void main()
{
   setup_adc_ports(NO_ANALOGS);
   setup_adc(ADC_CLOCK_DIV_2);
   setup_psp(PSP_DISABLED);
   setup_spi(SPI_SS_DISABLED);
   setup_wdt(WDT_OFF);
   setup_timer_0(RTCC_INTERNAL);
   setup_timer_1(T1_DISABLED);
   setup_timer_2(T2_DISABLED,0,1);
   //Setup_Oscillator parameter not selected from Intr Oscillator Config tab

   // TODO: USER CODE!!
   printf("\n\rWelcome to Talking Tom Trampoline\n\r");  // communication on hyperterminal
   printf("Game ON!\n\r");
   rtos_run();
   printf("Oops, Tom Died. GAME OVER!\n\r");
   printf("Final Score: %Li", (int32) (score));
   while(1){}
}
