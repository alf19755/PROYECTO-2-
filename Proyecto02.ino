//***************************************************************************************************************************************
/* JUEGO ALTAIR AVATAR
 */
//***************************************************************************************************************************************
#include <SPI.h>
#include <SD.h>
#include <stdint.h>
#include <stdbool.h>
#include <TM4C123GH6PM.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/rom_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"

#include "bitmaps.h"
#include "font.h"
#include "lcd_registers.h"

#define LCD_RST PD_0
#define LCD_CS PD_1
#define LCD_RS PD_2
#define LCD_WR PD_3
#define LCD_RD PE_1
int DPINS[] = {PB_0, PB_1, PB_2, PB_3, PB_4, PB_5, PB_6, PB_7};
#define BOTONDJ1 PA_7
#define BOTONIJ1 PA_6
#define BOTONDJ2 PE_2
#define BOTONIJ2 PE_3
#define INICIO PC_4
#define NIV1 PC_5
 //COSAS SD
//BOTON MENU
#define BOTONSTART PUSH2 //PE_2 //PUSH2
#define BOTON1PLAYER PUSH2// PA_7 //PUSH2
#define BOTONMULTIPLAYER PUSH1//PA_6 //PUSH2
File dataFile;
const int chipSelect = 12; //cs PIN
bool start= false;
bool modoMultijugadorSeleccionado = false;
bool modoUnJugadorSeleccionado = false;


//FIN SD

const int ledPin = RED_LED;
bool botonPresionado = false; // Variable para rastrear si el botón está presionado
int x = 50;
int ax = 2;
int y = 0;
int in = 186;
int ina = 100;
bool botonAnterior = HIGH; // Variable para rastrear el estado anterior del botón
int maxaang = 278;
int maxalta = 220;
int minaang = 50;
int minalta = 25;

int agarrollave1 = 0;
int agarrollave2 = 0;

int altairabrio = 0;
int aangabrio = 0;

int nivel1 = 1;
int nivel2 = 0;

int tiempo = 60;                                                         
//***************************************************************************************************************************************
// Functions Prototypes
//***************************************************************************************************************************************
void LCD_Init(void);
void LCD_CMD(uint8_t cmd);
void LCD_DATA(uint8_t data);
void SetWindows(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);
void LCD_Clear(unsigned int c);                                                                 //void LCD_Clear(cortina color "c");
void H_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c);
void V_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c);
void Rect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c);     
void FillRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c);  //void FillRect(Columna, Fila, tamaño letra, color letra, color subrayado);
void LCD_Print(String text, int x, int y, int fontSize, int color, int background);

void LCD_Bitmap(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned char bitmap[]);
void LCD_Sprite(int x, int y, int width, int height, unsigned char bitmap[],int columns, int index, char flip, char offset);

//SD
int ASCII_HEX(int a);
void mapeo_SD(char doc[]);


extern uint8_t Aangsalto[];
extern uint8_t Boton[];
extern uint8_t sBoton[];
extern uint8_t Llave[];
extern uint8_t Aangparado[];
extern uint8_t pared[];
extern uint8_t puertacerrada[];
extern uint8_t altairparado[];
extern uint8_t altairderecha[];
extern uint8_t altairizquierda[];
extern uint8_t cuadrito[];

extern uint8_t puertaabierta1[];
extern uint8_t puertaabierta2[];
extern uint8_t puertaabierta3[];

extern uint8_t aangazul[];
extern uint8_t altairazul[];

const char* tiempos[] = {"30", "29", "28", "27", "26", "25", "24", "23", "22", "21", "20", "19", "18", "17", "16", "15", 
"14", "13", "12", "11", "10", "9", "8", "7", "6", "5", "4", "3", "2", "1", "0"};

int tiempoIndex = 0;



void setup() {
  //***************************************************************************************************************************************
// Inicialización
//***************************************************************************************************************************************

  // Configura el botón como entrada con pull-up
  pinMode(BOTONDJ1, INPUT_PULLUP);
  pinMode(BOTONIJ1, INPUT_PULLUP);
  pinMode(BOTONDJ2, INPUT_PULLUP);
  pinMode(BOTONIJ2, INPUT_PULLUP);
  pinMode(BOTON1PLAYER, INPUT_PULLUP);
  pinMode(BOTONMULTIPLAYER, INPUT_PULLUP);
  
  // Configura el LED como salida
  pinMode(ledPin, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  //PANTALLA INICIO
  SysCtlClockSet(SYSCTL_SYSDIV_2_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);
  Serial.begin(9600);
  GPIOPadConfigSet(GPIO_PORTB_BASE, 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD_WPU);
  Serial.println("Inicio");


  //breve declaracion sd
    // Configura el LED como salida
  //pinMode(ledPin, OUTPUT);
 //CONFIG SD
 SPI.setModule(0);         //Indespensable declarar el SPI a usar en TIVA C
  delay(2500);              //Tiempo para abrir la terminal serial
//Configuracioni para lectura y escritura de lcd

  Serial.print("\nInitializing SD card...\n");
  pinMode(12, OUTPUT);     // change this to 53 on a mega or 12 on a Tiva C
  if (!SD.begin(chipSelect)) {        //Inicio la comunicacion a la SD pero en config. de lectura/escritura
    Serial.println("Card failed, or not present");
    return;
  }
  Serial.println("card initialized.");

//********************************************PANTALLA START*************************************************
  
  LCD_Init();
  LCD_Clear(0x00);                      




  //------------pantalla nintendo-------
  //FillRect(0, 0, 319, 206, 0x00);       //PINTAR  EL FONDO
  //DEFINICIÓN DE TEXTOS
  String text0 = "  Nintendo ";
  //String text1 = "Altair's Creed";
  String text2 = "and Aang's Legacy";
  //IMPRIMIR TEXTO
  LCD_Print(text0, 60, 107, 2, 0xffff, 0x00);
  delay(1000);
    //------------pantalla start-------

  //AÑADIR BLOQUES SOBRE FONDO  
  //LCD_Bitmap(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned char bitmap[]);
  LCD_Clear(0x1907); 
  
  
 mapeo_SD("font.txt");
//  mapeo_SD("cast.txt");
 // mapeo_SD("out.txt");
  delay(1000);
  String text1 = "Altair's Creed";
  LCD_Print(text1, 50, 100, 2, 0xffff, 0x2104);
  LCD_Print(text2, 30, 120, 2, 0xffff, 0x2104);
  delay(1000);



//PANTALLA START 
   
 // Variable de estado para el parpadeo
bool parpadeo = true;


// Inicio del parpadeo
while (parpadeo) {
  //PULSE START
  int estadoBoton = digitalRead(BOTONSTART);
  if (estadoBoton == HIGH) {
    String textstart = "Pulse start";

    LCD_Print(textstart, 75, 180, 2, 0xffff, 0x00);
    delay(500); // Ajusta el tiempo de parpadeo aquí
    LCD_Print(textstart, 75, 180, 2, 0x00, 0x00);
    delay(500); // Ajusta el tiempo de parpadeo aquí
  } else {
    // El botón se activa
    parpadeo = false; // Detiene el parpadeo si el botón se presiona
    start=true;       //Enciende la bandera start

    //--------------SELECCIÓN DE MODO DE JUEGO ----------------------------

     FillRect(0, 0, 319, 239, 0x41E7);       //PINTAR  EL FONDO  
     LCD_Print("Seleccione",80, 10, 2, 0xffff, 0x00);
     LCD_Print("Modo de juego", 50, 25, 2, 0xffff, 0x00);

     LCD_Print("- 1 JUGADOR", 30, 90, 2, 0xffff, 0xD000);
     LCD_Print("- MULTIJUGADOR", 30, 120, 2, 0xffff, 0x015A);
     
     while (!modoMultijugadorSeleccionado && !modoUnJugadorSeleccionado) {
    // Leer el estado de los botones
    int estadoBotonMultijugador = digitalRead(BOTONMULTIPLAYER);
    int estadoBotonUnJugador = digitalRead(BOTON1PLAYER);

    // MULTIJUGADOR
    if (estadoBotonMultijugador == LOW) {
      modoMultijugadorSeleccionado = true;
      
       digitalWrite(ledPin, HIGH);

       
      break;  // Sal del bucle, se seleccionó multijugador
    } 

    
    //1 JUGADOR
     else if (estadoBotonUnJugador == LOW) {
      modoUnJugadorSeleccionado = true;
      digitalWrite(BLUE_LED, HIGH);
      

     
  
      
      
      break;  // Sal del bucle, se seleccionó un jugador

    
  }

  
}
      
      mapeo_SD("avat.txt");
      LCD_Print("Seleccione",80, 10, 2, 0xffff, 0x00);
      LCD_Print("Su personaje", 50, 25, 2, 0xffff, 0x00);
      delay(2000);


//ELEMENTOS DE LA PANTALLA

  LCD_Clear(0x1907); 
       FillRect(0, 0, 319, 239, 0x41E7);       //PINTAR  EL FONDO  

     for(int x = 0; x < 318; x++){
      LCD_Bitmap(x, 231, 16, 8, Calabozo);
      x += 16;
   }

   for(int x = 0; x < 318; x++){
      LCD_Bitmap(x, 160, 16, 8, Calabozo);
      x += 16;
   }

   for(int x = 0; x < 318; x++){
      LCD_Bitmap(x, 87, 16, 8, Calabozo);
      x += 16;
   }

//inicio aang

   LCD_Bitmap(50, 183, 13, 49, Aangparado);

//inicio altair

   LCD_Bitmap(25, 100, 28, 60, altairparado);

//Paredes

  int yPosition1 = 168;  // Define la posición inicial en el eje Y

  for (int i = 0; i < 240; i += 60) {
    LCD_Bitmap(35, yPosition1, 8, 16, pared);  // Dibuja una línea de bitmaps en la posición actual de Y
    yPosition1 += 16;  // Aumenta la posición en Y para la próxima línea de bitmaps
  }

  int yPosition2 = 95;  // Define la posición inicial en el eje Y

  for (int i = 0; i < 240; i += 60) {
    LCD_Bitmap(260, yPosition2, 8, 16, pared);  // Dibuja una línea de bitmaps en la posición actual de Y
    yPosition2 += 16;  // Aumenta la posición en Y para la próxima línea de bitmaps
  }

////////////////////////////LLAVE///////////////////////////////////

//AANG
LCD_Bitmap(12, 195, 14, 16, Llave);

//ALTAIR
LCD_Bitmap(280, 120, 14, 16, Llave);

///////////////////////////PUERTA///////////////////////////////////

//AANG
LCD_Bitmap(302, 200, 16, 32, puertacerrada);
//ALTAIR
LCD_Bitmap(2, 129, 16, 32, puertacerrada);
  
 

  
}
}
}

//***************************************************************************************************************************************
// Loop Infinito
//***************************************************************************************************************************************
void loop() {
//
if  (modoMultijugadorSeleccionado==false){
  // Lee el estado del botón
  int estadoBotondj1 = digitalRead(BOTONDJ1);
  int estadoBotonij1 = digitalRead(BOTONIJ1);
  int estadoBotondj2 = digitalRead(BOTONDJ2);
  int estadoBotonij2 = digitalRead(BOTONIJ2);

//***************************************************************************************************************************************
// NIVEL 1
//***************************************************************************************************************************************

if(nivel1 == 1){

  digitalWrite(NIV1, HIGH);

///////////////////////////TIEMPO/////////////////////////////////////

int temp = 0;
const char* texttiempo = "TIME: ";
LCD_Print(texttiempo, 5, 5, 1, 0xffff, 0x41E7);

LCD_Print(tiempos[0], 45, 5, 1, 0xffff, 0x41E7);

////////////////////////////BOTON////////////////////////////////////

//AANG//

if (x < 125){
  LCD_Bitmap(150, 215, 16, 16, Boton);
  maxalta = 220;
  int yPosition2 = 95;  // Define la posición inicial en el eje Y

  for (int i = 0; i < 240; i += 60) {
    LCD_Bitmap(260, yPosition2, 8, 16, pared);  // Dibuja una línea de bitmaps en la posición actual de Y
    yPosition2 += 16;  // Aumenta la posición en Y para la próxima línea de bitmaps
  }
}
if (x > 125 && x < 159) {
  LCD_Bitmap(150, 224, 16, 7, sBoton);
  maxalta = 265;
}
if (x > 160){
  LCD_Bitmap(150, 215, 16, 16, Boton);
  maxalta = 220;
  int yPosition2 = 95;  // Define la posición inicial en el eje Y

  for (int i = 0; i < 240; i += 60) {
    LCD_Bitmap(260, yPosition2, 8, 16, pared);  // Dibuja una línea de bitmaps en la posición actual de Y
    yPosition2 += 16;  // Aumenta la posición en Y para la próxima línea de bitmaps
  }
}

//ALTAIR//

if (ax < 165){
  LCD_Bitmap(200, 145, 16, 16, Boton);
  minaang = 50;
  int yPosition1 = 168;  // Define la posición inicial en el eje Y

  for (int i = 0; i < 240; i += 60) {
    LCD_Bitmap(35, yPosition1, 8, 16, pared);  // Dibuja una línea de bitmaps en la posición actual de Y
    yPosition1 += 16;  // Aumenta la posición en Y para la próxima línea de bitmaps
  }
}
if (ax > 165 && ax < 220) {
  LCD_Bitmap(200, 153, 16, 7, sBoton);
  minaang = 2;
}
if (ax > 220){
  LCD_Bitmap(200, 145, 16, 16, Boton);
  minaang = 50;
  int yPosition1 = 168;  // Define la posición inicial en el eje Y

  for (int i = 0; i < 240; i += 60) {
    LCD_Bitmap(35, yPosition1, 8, 16, pared);  // Dibuja una línea de bitmaps en la posición actual de Y
    yPosition1 += 16;  // Aumenta la posición en Y para la próxima línea de bitmaps
  }
}

///////////////////////AGARRARON LLAVE///////////////////////////////

if(ax > 220){

agarrollave2 = 1;
LCD_Bitmap(280, 120, 14, 16, cuadrito);
  
}

if(x < 50){

agarrollave1 = 1;
LCD_Bitmap(12, 195, 14, 16, cuadrito);
  
}

////////////////////////ABRIR PUERTAS////////////////////////////////

if (agarrollave1 == 1 && x == 278){

LCD_Bitmap(302, 200, 16, 31, puertaabierta1);
delay(500);
LCD_Bitmap(302, 200, 16, 31, puertaabierta2);
delay(500);
LCD_Bitmap(302, 200, 15, 30, puertaabierta3);

agarrollave1 = 0;
aangabrio = 1;
  
}

if (agarrollave2 == 1 && ax == 25){

LCD_Bitmap(2, 129, 16, 31, puertaabierta1);
delay(500);
LCD_Bitmap(2, 129, 16, 31, puertaabierta2);
delay(500);
LCD_Bitmap(2, 129, 15, 30, puertaabierta3);

agarrollave2 = 0;
altairabrio = 1;
  
}


////////////////////////ABRIR PUERTAS////////////////////////////////

if (aangabrio == 1 && altairabrio == 1){

LCD_Clear (0x00);
mapeo_SD("sot.txt");
delay(1000);

const char* textnivel1 = "NIVEL 1 SUPERADO";
LCD_Print(textnivel1, 35, 120, 2, 0xffff, 0x00);
nivel1 = 0;
nivel2 = 1;

delay(1500);
  
}


////////////////////CAMINAR DERECHA AANG////////////////////////////

    // Si el estado del botón ha cambiado
    if (estadoBotondj1 == LOW) {
      
      // El botón se acaba de presionar
      digitalWrite(ledPin, HIGH);

      // Verifica si el muñeco puede moverse a la derecha
      if (x < maxaang) {
        x++;
        Movimiento_x_d(AangCam);
      }
    } 
    
    else {
      // El botón se acaba de soltar
      digitalWrite(ledPin, LOW);
    }

////////////////////CAMINAR DERECHA ALTAIR///////////////////////////

    // Si el estado del botón ha cambiado
    if (estadoBotondj2 == LOW) {
      
      // El botón se acaba de presionar
      digitalWrite(ledPin, HIGH);

      // Verifica si el muñeco puede moverse a la derecha
      if (ax < maxalta) {
        ax++;
        Movimiento_d_a(altairderecha);
      }
    } 
    
    else {
      // El botón se acaba de soltar
      digitalWrite(ledPin, LOW);
    }

///////////////////////CAMINAR IZQUIERDA//////////////////////////////

    // Si el estado del botón ha cambiado
    if (estadoBotonij1 == LOW) {
      
      // El botón se acaba de presionar
      digitalWrite(ledPin, HIGH);

      // Verifica si el muñeco puede moverse a la derecha
      if (x > minaang) {
        x--;
        Movimiento_x_i(AangCamI);
      }
    } 
    
    else {
      // El botón se acaba de soltar
      digitalWrite(ledPin, LOW);
    }

///////////////////CAMINAR IZQUIERDA ALTAIR//////////////////////////

    // Si el estado del botón ha cambiado
    if (estadoBotonij2 == LOW) {
      
      // El botón se acaba de presionar
      digitalWrite(ledPin, HIGH);

      // Verifica si el muñeco puede moverse a la derecha
      if (ax > minalta) {
        ax--;
        Movimiento_i_a(altairizquierda);
      }
    } 
    
    else {
      // El botón se acaba de soltar
      digitalWrite(ledPin, LOW);
    }

///////////////////////SALTAR IZQUIERDA//////////////////////////////

/*    // Si el estado del botón ha cambiado
    if (estadoBotonsj1 == LOW) {
      
      // El botón se acaba de presionar
      digitalWrite(ledPin, HIGH);

    if(x > 50){
      for(y = 0; y < 67; y++){
          
          Movimiento_y_i(Aangsalto);
          
        }

        in = y + 90;
        
      } 
    }
    
    else {
      // El botón se acaba de soltar
      digitalWrite(ledPin, LOW);
    }
*/
/////////////////////////CAÍDA AANG////////////////////////////////

}

//***************************************************************************************************************************************
// NIVEL 2
//***************************************************************************************************************************************

if(nivel2 == 1){

  FillRect(0, 0, 319, 239, 0x66D8);       //PINTAR  EL FONDO
  const char* textnivel2 = "NIVEL 2";
  LCD_Print(textnivel2, 120, 110, 2, 0xffff, 0x66D8);

  LCD_Bitmap(135, 160, 13, 49, aangazul);

  LCD_Bitmap(170, 150, 28, 60, altairazul);
  mapeo_SD("cast.txt");
  delay(1000);

 
  

///////////////////////////TIEMPO/////////////////////////////////////

  int temp = 0;
  const char* texttiempo = "TIME: ";
  LCD_Print(texttiempo, 5, 5, 1, 0xffff, 0x66D8);
  
  LCD_Print(tiempos[0], 45, 5, 1, 0xffff, 0x66D8);

  delay(5000);

}


  
  
  
  
  }
  
  
  }

//***************************************************************************************************************************************
// Funciones del juego
//***************************************************************************************************************************************

void Movimiento_x_d(unsigned char* nombre){
  
      delay(2);
      int anim2 = (x /5) % 4;
  
      LCD_Sprite(x, in - y, 34, 43, nombre, 4, anim2, 0, 1);
      V_line(x - 1, in - y, 43, 0x41E7);
  
  }

void Movimiento_d_a(unsigned char* nombre){
  
      delay(2);
      int anim4 = (ax /5) % 4;
  
      LCD_Sprite(ax, ina, 44, 58, nombre, 4, anim4, 0, 1);
      V_line(ax - 1, ina, 58, 0x41E7);
  
  }

void Movimiento_x_i(unsigned char* nombre){
  
        int anim2 = (x/5)%4;
    
        LCD_Sprite(x, in, 34, 43, nombre, 4, anim2, 0, 1);
        V_line( x + 34, in, 43, 0x41E7);
  
  }

void Movimiento_i_a(unsigned char* nombre){
  
        int anim2 = (ax/5)%4;
    
        LCD_Sprite(ax, ina, 44, 58, nombre, 4, anim2, 0, 1);
        V_line( ax + 44, ina, 58, 0x41E7);
  
  }

//(int x, int y, int width, int height, unsigned char bitmap[],int columns, int index, char flip, char offset)

void Movimiento_y_i(unsigned char* nombre){
  
        int anim2 = (y/30)%3;
    
        LCD_Sprite(x, (in - 10) - y, 42, 54, nombre, 3, anim2, 0, 1);
        H_line(x, in - y, 54, 0x41E7);
  
  }

//***************************************************************************************************************************************
// Función para inicializar LCD
//***************************************************************************************************************************************
void LCD_Init(void) {
  pinMode(LCD_RST, OUTPUT);
  pinMode(LCD_CS, OUTPUT);
  pinMode(LCD_RS, OUTPUT);
  pinMode(LCD_WR, OUTPUT);
  pinMode(LCD_RD, OUTPUT);
  for (uint8_t i = 0; i < 8; i++){
    pinMode(DPINS[i], OUTPUT);
  }
  //****************************************
  // Secuencia de Inicialización
  //****************************************
  digitalWrite(LCD_CS, HIGH);
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_WR, HIGH);
  digitalWrite(LCD_RD, HIGH);
  digitalWrite(LCD_RST, HIGH);
  delay(5);
  digitalWrite(LCD_RST, LOW);
  delay(20);
  digitalWrite(LCD_RST, HIGH);
  delay(150);
  digitalWrite(LCD_CS, LOW);
  //****************************************
  LCD_CMD(0xE9);  // SETPANELRELATED
  LCD_DATA(0x20);
  //****************************************
  LCD_CMD(0x11); // Exit Sleep SLEEP OUT (SLPOUT)
  delay(100);
  //****************************************
  LCD_CMD(0xD1);    // (SETVCOM)
  LCD_DATA(0x00);
  LCD_DATA(0x71);
  LCD_DATA(0x19);
  //****************************************
  LCD_CMD(0xD0);   // (SETPOWER) 
  LCD_DATA(0x07);
  LCD_DATA(0x01);
  LCD_DATA(0x08);
  //****************************************
  LCD_CMD(0x36);  // (MEMORYACCESS)
  LCD_DATA(0x40|0x80|0x20|0x08); // LCD_DATA(0x19);
  //****************************************
  LCD_CMD(0x3A); // Set_pixel_format (PIXELFORMAT)
  LCD_DATA(0x05); // color setings, 05h - 16bit pixel, 11h - 3bit pixel
  //****************************************
  LCD_CMD(0xC1);    // (POWERCONTROL2)
  LCD_DATA(0x10);
  LCD_DATA(0x10);
  LCD_DATA(0x02);
  LCD_DATA(0x02);
  //****************************************
  LCD_CMD(0xC0); // Set Default Gamma (POWERCONTROL1)
  LCD_DATA(0x00);
  LCD_DATA(0x35);
  LCD_DATA(0x00);
  LCD_DATA(0x00);
  LCD_DATA(0x01);
  LCD_DATA(0x02);
  //****************************************
  LCD_CMD(0xC5); // Set Frame Rate (VCOMCONTROL1)
  LCD_DATA(0x04); // 72Hz
  //****************************************
  LCD_CMD(0xD2); // Power Settings  (SETPWRNORMAL)
  LCD_DATA(0x01);
  LCD_DATA(0x44);
  //****************************************
  LCD_CMD(0xC8); //Set Gamma  (GAMMASET)
  LCD_DATA(0x04);
  LCD_DATA(0x67);
  LCD_DATA(0x35);
  LCD_DATA(0x04);
  LCD_DATA(0x08);
  LCD_DATA(0x06);
  LCD_DATA(0x24);
  LCD_DATA(0x01);
  LCD_DATA(0x37);
  LCD_DATA(0x40);
  LCD_DATA(0x03);
  LCD_DATA(0x10);
  LCD_DATA(0x08);
  LCD_DATA(0x80);
  LCD_DATA(0x00);
  //****************************************
  LCD_CMD(0x2A); // Set_column_address 320px (CASET)
  LCD_DATA(0x00);
  LCD_DATA(0x00);
  LCD_DATA(0x01);
  LCD_DATA(0x3F);
  //****************************************
  LCD_CMD(0x2B); // Set_page_address 480px (PASET)
  LCD_DATA(0x00);
  LCD_DATA(0x00);
  LCD_DATA(0x01);
  LCD_DATA(0xE0);
//  LCD_DATA(0x8F);
  LCD_CMD(0x29); //display on 
  LCD_CMD(0x2C); //display on

  LCD_CMD(ILI9341_INVOFF); //Invert Off
  delay(120);
  LCD_CMD(ILI9341_SLPOUT);    //Exit Sleep
  delay(120);
  LCD_CMD(ILI9341_DISPON);    //Display on
  digitalWrite(LCD_CS, HIGH);
}
//***************************************************************************************************************************************
// Función para enviar comandos a la LCD - parámetro (comando)
//***************************************************************************************************************************************
void LCD_CMD(uint8_t cmd) {
  digitalWrite(LCD_RS, LOW);
  digitalWrite(LCD_WR, LOW);
  GPIO_PORTB_DATA_R = cmd;
  digitalWrite(LCD_WR, HIGH);
}
//***************************************************************************************************************************************
// Función para enviar datos a la LCD - parámetro (dato)
//***************************************************************************************************************************************
void LCD_DATA(uint8_t data) {
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_WR, LOW);
  GPIO_PORTB_DATA_R = data;
  digitalWrite(LCD_WR, HIGH);
}
//***************************************************************************************************************************************
// Función para definir rango de direcciones de memoria con las cuales se trabajara (se define una ventana)
//***************************************************************************************************************************************
void SetWindows(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2) {
  LCD_CMD(0x2a); // Set_column_address 4 parameters
  LCD_DATA(x1 >> 8);
  LCD_DATA(x1);   
  LCD_DATA(x2 >> 8);
  LCD_DATA(x2);   
  LCD_CMD(0x2b); // Set_page_address 4 parameters
  LCD_DATA(y1 >> 8);
  LCD_DATA(y1);   
  LCD_DATA(y2 >> 8);
  LCD_DATA(y2);   
  LCD_CMD(0x2c); // Write_memory_start
}
//***************************************************************************************************************************************
// Función para borrar la pantalla - parámetros (color)
//***************************************************************************************************************************************
void LCD_Clear(unsigned int c){  
  unsigned int x, y;
  LCD_CMD(0x02c); // write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW);   
  SetWindows(0, 0, 319, 239); // 479, 319);
  for (x = 0; x < 320; x++)
    for (y = 0; y < 240; y++) {
      LCD_DATA(c >> 8); 
      LCD_DATA(c); 
    }
  digitalWrite(LCD_CS, HIGH);
} 
//***************************************************************************************************************************************
// Función para dibujar una línea horizontal - parámetros ( coordenada x, cordenada y, longitud, color)
//*************************************************************************************************************************************** 
void H_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c) {  
  unsigned int i, j;
  LCD_CMD(0x02c); //write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW);
  l = l + x;
  SetWindows(x, y, l, y);
  j = l;// * 2;
  for (i = 0; i < l; i++) {
      LCD_DATA(c >> 8); 
      LCD_DATA(c); 
  }
  digitalWrite(LCD_CS, HIGH);
}
//***************************************************************************************************************************************
// Función para dibujar una línea vertical - parámetros ( coordenada x, cordenada y, longitud, color)
//*************************************************************************************************************************************** 
void V_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c) {  
  unsigned int i,j;
  LCD_CMD(0x02c); //write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW);
  l = l + y;
  SetWindows(x, y, x, l);
  j = l; //* 2;
  for (i = 1; i <= j; i++) {
    LCD_DATA(c >> 8); 
    LCD_DATA(c);
  }
  digitalWrite(LCD_CS, HIGH);  
}
//***************************************************************************************************************************************
// Función para dibujar un rectángulo - parámetros ( coordenada x, cordenada y, ancho, alto, color)
//***************************************************************************************************************************************
void Rect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c) {
  H_line(x  , y  , w, c);
  H_line(x  , y+h, w, c);
  V_line(x  , y  , h, c);
  V_line(x+w, y  , h, c);
}
//***************************************************************************************************************************************
// Función para dibujar un rectángulo relleno - parámetros ( coordenada x, cordenada y, ancho, alto, color)
//***************************************************************************************************************************************
/*void FillRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c) {
  unsigned int i;
  for (i = 0; i < h; i++) {
    H_line(x  , y  , w, c);
    H_line(x  , y+i, w, c);
  }
}
*/

void FillRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c) {
  LCD_CMD(0x02c); // write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW); 
  
  unsigned int x2, y2;
  x2 = x+w;
  y2 = y+h;
  SetWindows(x, y, x2-1, y2-1);
  unsigned int k = w*h*2-1;
  unsigned int i, j;
  for (int i = 0; i < w; i++) {
    for (int j = 0; j < h; j++) {
      LCD_DATA(c >> 8);
      LCD_DATA(c);
      
      //LCD_DATA(bitmap[k]);    
      k = k - 2;
     } 
  }
  digitalWrite(LCD_CS, HIGH);
}
//***************************************************************************************************************************************
// Función para dibujar texto - parámetros ( texto, coordenada x, cordenada y, color, background) 
//***************************************************************************************************************************************
void LCD_Print(String text, int x, int y, int fontSize, int color, int background) {
  int fontXSize ;
  int fontYSize ;
  
  if(fontSize == 1){
    fontXSize = fontXSizeSmal ;
    fontYSize = fontYSizeSmal ;
  }
  if(fontSize == 2){
    fontXSize = fontXSizeBig ;
    fontYSize = fontYSizeBig ;
  }
  
  char charInput ;
  int cLength = text.length();
  Serial.println(cLength,DEC);
  int charDec ;
  int c ;
  int charHex ;
  char char_array[cLength+1];
  text.toCharArray(char_array, cLength+1) ;
  for (int i = 0; i < cLength ; i++) {
    charInput = char_array[i];
    Serial.println(char_array[i]);
    charDec = int(charInput);
    digitalWrite(LCD_CS, LOW);
    SetWindows(x + (i * fontXSize), y, x + (i * fontXSize) + fontXSize - 1, y + fontYSize );
    long charHex1 ;
    for ( int n = 0 ; n < fontYSize ; n++ ) {
      if (fontSize == 1){
        charHex1 = pgm_read_word_near(smallFont + ((charDec - 32) * fontYSize) + n);
      }
      if (fontSize == 2){
        charHex1 = pgm_read_word_near(bigFont + ((charDec - 32) * fontYSize) + n);
      }
      for (int t = 1; t < fontXSize + 1 ; t++) {
        if (( charHex1 & (1 << (fontXSize - t))) > 0 ) {
          c = color ;
        } else {
          c = background ;
        }
        LCD_DATA(c >> 8);
        LCD_DATA(c);
      }
    }
    digitalWrite(LCD_CS, HIGH);
  }
}
//***************************************************************************************************************************************
// Función para dibujar una imagen a partir de un arreglo de colores (Bitmap) Formato (Color 16bit R 5bits G 6bits B 5bits)
//***************************************************************************************************************************************
void LCD_Bitmap(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned char bitmap[]){  
  LCD_CMD(0x02c); // write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW); 
  
  unsigned int x2, y2;
  x2 = x+width;
  y2 = y+height;
  SetWindows(x, y, x2-1, y2-1);
  unsigned int k = 0;
  unsigned int i, j;

  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      LCD_DATA(bitmap[k]);
      LCD_DATA(bitmap[k+1]);
      //LCD_DATA(bitmap[k]);    
      k = k + 2;
     } 
  }
  digitalWrite(LCD_CS, HIGH);
}
//***************************************************************************************************************************************
// Función para dibujar una imagen sprite - los parámetros columns = número de imagenes en el sprite, index = cual desplegar, flip = darle vuelta
//***************************************************************************************************************************************
void LCD_Sprite(int x, int y, int width, int height, unsigned char bitmap[],int columns, int index, char flip, char offset){
  LCD_CMD(0x02c); // write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW); 

  unsigned int x2, y2;
  x2 =   x+width;
  y2=    y+height;
  SetWindows(x, y, x2-1, y2-1);
  int k = 0;
  int ancho = ((width*columns));
  if(flip){
  for (int j = 0; j < height; j++){
      k = (j*(ancho) + index*width -1 - offset)*2;
      k = k+width*2;
     for (int i = 0; i < width; i++){
      LCD_DATA(bitmap[k]);
      LCD_DATA(bitmap[k+1]);
      k = k - 2;
     } 
  }
  }else{
     for (int j = 0; j < height; j++){
      k = (j*(ancho) + index*width + 1 + offset)*2;
     for (int i = 0; i < width; i++){
      LCD_DATA(bitmap[k]);
      LCD_DATA(bitmap[k+1]);
      k = k + 2;
     } 
  }
    
    
    }
  digitalWrite(LCD_CS, HIGH);
}


//*****************************************************************************
// Retorno de numero decimal a partir de hexadecimal
//*****************************************************************************
int ASCII_HEX(int a) {
  switch (a) {
    case 48:
      return 0;
    case 49:
      return 1;
    case 50:
      return 2;
    case 51:
      return 3;
    case 52:
      return 4;
    case 53:
      return 5;
    case 54:
      return 6;
    case 55:
      return 7;
    case 56:
      return 8;
    case 57:
      return 9;
    case 97:
      return 10;
    case 98:
      return 11;
    case 99:
      return 12;
    case 100:
      return 13;
    case 101:
      return 14;
    case 102:
      return 15;
  }
}
//*****************************************************************************
// leer la sd espacio por espacio
//*****************************************************************************
void mapeo_SD(char doc[]) {
  dataFile = SD.open(doc, FILE_READ);
  int hex1 = 0;
  int val1 = 0;
  int val2 = 0;
  int mapear = 0;
  int vertical = 0;
  unsigned char maps[640];

  if (dataFile) {
    //Serial.println("Abriendo el archivo");
    while (dataFile.available() ) {
      mapear = 0;
      while (mapear < 640) {
        hex1 = dataFile.read();
        if (hex1 == 120) {
          val1 = dataFile.read();
          val2 = dataFile.read();
          val1 = ASCII_HEX(val1);
          val2 = ASCII_HEX(val2);
          maps[mapear] = val1 * 16 + val2;
          mapear++;
        }
      }
      LCD_Bitmap(0, vertical, 320, 1, maps);
      vertical++;
    }

    dataFile.close();
    Serial.println("cierro");
  } else {
    Serial.println("error opening el doc");
    dataFile.close();
  }
}
