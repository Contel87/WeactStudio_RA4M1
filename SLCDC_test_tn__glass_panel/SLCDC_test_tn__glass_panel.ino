https://forum.arduino.cc/t/pilotare-un-pannello-lcd-tipo-tn-con-ra4m1/1368312/2

// With Capacitor split metod, LCD drive voltage has same level of RA4M1 Vcc (USE IT ONLY IF Vcc of RA4M1 is 3.3V)
// Capacitor split metod not require continuous current flow, and therefore the current consuption can be reduced
//#define CAPACITOR_SPLIT_METOD

// Internal voltage boosting metod allow you to choose LCD drive voltage by VLCD register
// RA4M1 enable internal voltage boost circuit that supply a constant voltage, regardless of changes in Vcc
#define INTERNAL_VOLTAGE_BOOSTING



const uint8_t char_number[] = {
        0x7d, 0x60, 0x3e, 0x7a, 0x63, 0x5b, 0x5f, 0x70, 0x7f, 0x7b /* 0-9 */
};
const uint8_t char_CIAO[] = {
        0x1d, 0x60, 0x77, 0x7d /* C-I-A-O */
};
const uint8_t char_Err[] = {
        0x1f, 0x06, 0x06 /* E-r-r */
};

void setup() {
  Serial.begin(115200);
  delay(2000);
  setupLCD();
  //lcd_display_3dig(-87);
  //display_Err(); // Visualizza la scritta "Err"
  display_CIAO(); // Scorre la parola CIAO sul display
}

void loop() {
  //display_CIAO();

}

void display_CIAO(){
  // Primo screen C
    R_SLCDC->SEG_b[1].A = char_CIAO[0] >> 4; 
    R_SLCDC->SEG_b[2].A = char_CIAO[0] & 0x0f;
    delay(500);
    // Secondo screen CI
    R_SLCDC->SEG_b[3].A = char_CIAO[0] >> 4; 
    R_SLCDC->SEG_b[4].A = char_CIAO[0] & 0x0f;
    R_SLCDC->SEG_b[1].A = char_CIAO[1] >> 4; 
    R_SLCDC->SEG_b[2].A = char_CIAO[1] & 0x0f;
    delay(500);
    // Terzo screen CIA
    R_SLCDC->SEG_b[5].A = char_CIAO[0] >> 4; 
    R_SLCDC->SEG_b[6].A = char_CIAO[0] & 0x0f;
    R_SLCDC->SEG_b[3].A = char_CIAO[1] >> 4; 
    R_SLCDC->SEG_b[4].A = char_CIAO[1] & 0x0f;
    R_SLCDC->SEG_b[1].A = char_CIAO[2] >> 4; 
    R_SLCDC->SEG_b[2].A = char_CIAO[2] & 0x0f;
    delay(500);
    //Quarto screen IAO
    R_SLCDC->SEG_b[5].A = char_CIAO[1] >> 4; 
    R_SLCDC->SEG_b[6].A = char_CIAO[1] & 0x0f;
    R_SLCDC->SEG_b[3].A = char_CIAO[2] >> 4; 
    R_SLCDC->SEG_b[4].A = char_CIAO[2] & 0x0f;
    R_SLCDC->SEG_b[1].A = char_CIAO[3] >> 4; 
    R_SLCDC->SEG_b[2].A = char_CIAO[3] & 0x0f;
    delay(500);
    // Quinto screen AO
    R_SLCDC->SEG_b[5].A = char_CIAO[2] >> 4; 
    R_SLCDC->SEG_b[6].A = char_CIAO[2] & 0x0f;
    R_SLCDC->SEG_b[3].A = char_CIAO[3] >> 4; 
    R_SLCDC->SEG_b[4].A = char_CIAO[3] & 0x0f;
    R_SLCDC->SEG_b[1].A = 0; 
    R_SLCDC->SEG_b[2].A = 0;
    delay(500);
    //Sesto screen O
    R_SLCDC->SEG_b[5].A = char_CIAO[3] >> 4; 
    R_SLCDC->SEG_b[6].A = char_CIAO[3] & 0x0f;
    R_SLCDC->SEG_b[3].A = 0; 
    R_SLCDC->SEG_b[4].A = 0;
    R_SLCDC->SEG_b[1].A = 0; 
    R_SLCDC->SEG_b[2].A = 0;
    delay(500);
    //Settimo screen vuoto
    R_SLCDC->SEG_b[5].A = 0; 
    R_SLCDC->SEG_b[6].A = 0;
    R_SLCDC->SEG_b[3].A = 0; 
    R_SLCDC->SEG_b[4].A = 0;
    R_SLCDC->SEG_b[1].A = 0; 
    R_SLCDC->SEG_b[2].A = 0;
    delay(500);
}

void display_Err(){
    R_SLCDC->SEG_b[1].A = char_Err[2] >> 4; 
    R_SLCDC->SEG_b[2].A = char_Err[2] & 0x0f;
    R_SLCDC->SEG_b[3].A = char_Err[1] >> 4; 
    R_SLCDC->SEG_b[4].A = char_Err[1] & 0x0f;
    R_SLCDC->SEG_b[5].A = char_Err[0] >> 4; 
    R_SLCDC->SEG_b[6].A = char_Err[0] & 0x0f;
    }

void lcd_display_3dig(int32_t value) { // Visualizza un valore numerico compreso tra -99 e 999
     bool negative = false; // flag che indica un valore negativo
      if((int)value<0){     // Se il valore e' negativo
      value = value * -1;   // non considera il segno meno
      negative = true; // ma tiene conto del segno meno con il flag "negative"
      }
    //uint8_t thousands = (uint8_t)((value % 10000) / 1000); // Da usare SOLTANTO su lcd a 4 cifre
    uint8_t hundreds = (uint8_t)((value % 1000) / 100); // scompone il valore in centinaia ...
    uint8_t tens= (uint8_t)((value % 100) / 10);        // decine ...
    uint8_t units= (uint8_t)((value % 10) / 1);         // e unita' ...    
      // Ogni cifra sul display è rappresentata da 2 segmenti
    R_SLCDC->SEG_b[1].A = char_number[(units)] >> 4;  // visualizza le unita' sulla cifra a destra del display
    R_SLCDC->SEG_b[2].A = char_number[(units)] & 0x0f; // la cifra a destra del display e' pilotata dai segmenti 1 & 2
    if(tens){ // Se il valore ha cifre dedimali le stampa sul display
    R_SLCDC->SEG_b[3].A = char_number[(tens)] >> 4; // visualizza le decine' sulla cifra al centro del display
    R_SLCDC->SEG_b[4].A = char_number[(tens)] & 0x0f; // la cifra centrale del display e' pilotata dai segmenti 3 & 4
    }
    if((tens == 0) && (negative == true)){ // Se il valore non ha cifre decimali ma e' negativo, stampa "-" a sinistra delle unita'
     R_SLCDC->SEG_b[3].A = 0x00; // "-" e' rappresentato con il valore 0x02
     R_SLCDC->SEG_b[4].A = 0x02; 
    }
    if(hundreds){ // Se il valore ha cifre centesimali
    R_SLCDC->SEG_b[5].A = char_number[(tens)] >> 4; // visualizza le centinaia sulla cifra a sinistra del display
    R_SLCDC->SEG_b[6].A = char_number[(tens)] & 0x0f; // la cifra a sinistra del display e' pilotata dai segmenti 5 & 6
    }
    if((hundreds == 0) && (negative == true)){ // Se il valore non ha cifre centesimali ma e' negativo, stampa "-" a sinistra delle decine'
     R_SLCDC->SEG_b[5].A = 0x00; // "-" e' rappresentato con il valore 0x02
     R_SLCDC->SEG_b[6].A = 0x02; 
    }
}

void setupLCD(){
  R_IOPORT_PinCfg(NULL, BSP_IO_PORT_03_PIN_01, IOPORT_PERIPHERAL_LCDC | IOPORT_CFG_PERIPHERAL_PIN); // SEG01
  R_IOPORT_PinCfg(NULL, BSP_IO_PORT_03_PIN_02, IOPORT_PERIPHERAL_LCDC | IOPORT_CFG_PERIPHERAL_PIN); // SEG02
  R_IOPORT_PinCfg(NULL, BSP_IO_PORT_03_PIN_03, IOPORT_PERIPHERAL_LCDC | IOPORT_CFG_PERIPHERAL_PIN); // SEG03
  R_IOPORT_PinCfg(NULL, BSP_IO_PORT_04_PIN_00, IOPORT_PERIPHERAL_LCDC | IOPORT_CFG_PERIPHERAL_PIN); // SEG04
  R_IOPORT_PinCfg(NULL, BSP_IO_PORT_04_PIN_01, IOPORT_PERIPHERAL_LCDC | IOPORT_CFG_PERIPHERAL_PIN); // SEG05
  R_IOPORT_PinCfg(NULL, BSP_IO_PORT_04_PIN_02, IOPORT_PERIPHERAL_LCDC | IOPORT_CFG_PERIPHERAL_PIN); // SEG06
  
  R_IOPORT_PinCfg(NULL, BSP_IO_PORT_01_PIN_04, IOPORT_PERIPHERAL_LCDC   | IOPORT_CFG_PERIPHERAL_PIN); // COM0
  R_IOPORT_PinCfg(NULL, BSP_IO_PORT_01_PIN_05, IOPORT_PERIPHERAL_LCDC   | IOPORT_CFG_PERIPHERAL_PIN); // COM1
  R_IOPORT_PinCfg(NULL, BSP_IO_PORT_01_PIN_06, IOPORT_PERIPHERAL_LCDC   | IOPORT_CFG_PERIPHERAL_PIN); // COM2
  R_IOPORT_PinCfg(NULL, BSP_IO_PORT_01_PIN_07, IOPORT_PERIPHERAL_LCDC   | IOPORT_CFG_PERIPHERAL_PIN); // COM3
  R_IOPORT_PinCfg(NULL, BSP_IO_PORT_01_PIN_11, IOPORT_PERIPHERAL_LCDC   | IOPORT_CFG_PERIPHERAL_PIN); // CAPH
  R_IOPORT_PinCfg(NULL, BSP_IO_PORT_01_PIN_12, IOPORT_PERIPHERAL_LCDC   | IOPORT_CFG_PERIPHERAL_PIN); // CAPL
  R_IOPORT_PinCfg(NULL, BSP_IO_PORT_01_PIN_00, IOPORT_PERIPHERAL_LCDC   | IOPORT_CFG_PERIPHERAL_PIN); // VL1
  R_IOPORT_PinCfg(NULL, BSP_IO_PORT_01_PIN_01, IOPORT_PERIPHERAL_LCDC   | IOPORT_CFG_PERIPHERAL_PIN); // VL2
  R_IOPORT_PinCfg(NULL, BSP_IO_PORT_01_PIN_03, IOPORT_PERIPHERAL_LCDC   | IOPORT_CFG_PERIPHERAL_PIN); // VL4
  
  R_MSTP->MSTPCRC_b.MSTPC4 = 0;  // Enable SLCDC module

  /*8.7.11 Segment LCDC Source Clock (LCDSRCCLK)
  The Segment LCDC source clock, LCDSRCCLK, is the operating clock for the SLCDC.
  The LCDSRCCLK is specified by the LCDSCKSEL[2:0] bits in SLCDSCKCR.
  LCDSRCCLK is output when SLCDSCKCR.LCDSCKEN is set to 1. When changing the value of
  SLCDSCKCR.LCDSCKSEL[2:0], make sure that the value of SLCDSCKCR.LCDSCKEN is 0.*/
  R_SYSTEM->PRCR = 0xA501;
  R_SYSTEM->SLCDSCKCR = 0x80;  // Enable LOCO clock as LCD source clock (see 8.2.19 Segment LCD Source Clock Control Register)
  R_SYSTEM->PRCR = 0xA501;


#ifdef INTERNAL_VOLTAGE_BOOSTING
R_SLCDC->LCDM1_b.LCDVLM = 0;
R_SLCDC->LCDM0 = 0x4D; //Internal voltage boosting method
R_SLCDC->VLCD = 0x04;  // Reference Voltage = 3V
delay(10);
#endif

#ifdef CAPACITOR_SPLIT_METOD
R_SLCDC->LCDM0 = 0x8D; //Capacitor split method 
#endif

R_SLCDC->LCDC0 = 0x07;         // 128Hz means 32Hz with 1/3 bias
R_SLCDC->LCDM1_b.VLCON = 1; //Enable voltage boosting circuit operation
  delay(10);
  R_SLCDC->LCDM1_b.SCOC = 1;
  R_SLCDC->LCDM1_b.LCDON = 1;
}

void lcd_display_close(){  
  R_SLCDC->LCDM1 = (uint8_t)(R_SLCDC->LCDM1 &(~(0xC0))); //Stop SLCD output  
  R_SLCDC->LCDM1_b.VLCON = 0; //Stop voltage boost circuit or capacitor split circuit operation  
  R_SLCDC->LCDM0_b.MDSET = 0; //Switch to external resistance method to reduce idle power consuption
  
  R_SYSTEM->PRCR = 0xA501;
  R_SYSTEM->SLCDSCKCR_b.LCDSCKEN = 0; //Disable lcd clock
  R_SYSTEM->PRCR = 0xA501;

  R_MSTP->MSTPCRC_b.MSTPC4 = 1; //Enter module-stop state for the SLCDC
}
