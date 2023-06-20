/*
Uno       =   A4 (SDA), A5 (SCL)
Mega2560  =   20 (SDA), 21 (SCL)
Leonardo  =   2 (SDA), 3 (SCL)
esp32     =   D21(SDA), D22(SCL)
*/
#include <LiquidCrystal_I2C.h>                            
//--------------------------------------------------------------------------------------------------------------------
void matrix();                      
int cursor_p,                                            //Variables globales
    caract_1,                             
    caract_2, //Si tienes mas lineas en tu pantalla lcd agregalas, yo tengo 4 entonces agregue hasta el 4
    caract_3, 
    caract_4;       
//--------------------------------------------------------------------------------------------------------------------
                          
LiquidCrystal_I2C disp(0x27,20,4);                       //Modulo I2C  Si no te funciona intenta cambiar a 0x3f  0x20  0x27

//--------------------------------------------------------------------------------------------------------------------              
void setup(){
  disp.begin(20,4);                                      //pantalla LCD 16 x 2 o 20 x 4... 
  disp.init();
  disp.backlight();
  disp.clear();
} //end setup
 
void loop()
{   
     matrix();                 

} //end loop

void matrix()         //Parte del efecto             
{

   randomSeed(millis() * analogRead(5));             
 //--------------------------------------------------------------------------------------------------------------------                                              
   cursor_p = random(20);                            //agrega numero random a la columna 
   caract_1 = random(256);
   caract_2 = random(198);
   caract_3 = random(170);
   caract_4 = random(210);
//--------------------------------------------------------------------------------------------------------------------
   disp.setCursor(cursor_p,0);                       //Posicionar en linea 1, en coluna aleatória
   disp.write(caract_1);                             //Imprime caracteres aleatórios 
  
   disp.setCursor(cursor_p,1);                       
   disp.write(caract_2);

   disp.setCursor(cursor_p,2);                       
   disp.write(caract_3);

   disp.setCursor(cursor_p,3);                       
   disp.write(caract_4);  
//-------------------------------------------------------------------------------------------------------------------- 
   delay(47);                                     //Tiempo con el que se tarda en imprimir el siguiente numero   
} //end matrix
