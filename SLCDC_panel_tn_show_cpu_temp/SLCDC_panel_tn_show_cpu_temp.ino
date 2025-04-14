// https://forum.arduino.cc/t/pilotare-un-pannello-lcd-tipo-tn-con-ra4m1/1368312/2

#include "analog.h"

// With Capacitor split metod, LCD drive voltage has same level of RA4M1 Vcc (USE IT ONLY IF Vcc of RA4M1 is 3.3V)
// Capacitor split metod not require continuous current flow, and therefore the current consuption can be reduced
//#define CAPACITOR_SPLIT_METOD

// Internal voltage boosting metod allow you to choose LCD drive voltage by VLCD register
// RA4M1 enable internal voltage boost circuit that supply a constant voltage, regardless of changes in Vcc
#define INTERNAL_VOLTAGE_BOOSTING

#define PERIOD         1000
#define SLOPE          -0.00365  // Sensibilità del sensore di temperatura V/℃ ((vs - v1) / (24 - 125)) --> Correggere qui eventuale errore di temperatura reale/misurata (con termometro su chip)
#define VREF           1.43     // Tensione interna di riferimento (secondo il datascheet Typ:1.43V Min:1.36V Max:1.50V)

float degC, vCC;
adc_status_t status;
adc_info_t adc_info;

const uint8_t char_number[] = { // Numeri da visualizzare sul display
        0x7d, 0x60, 0x3e, 0x7a, 0x63, 0x5b, 0x5f, 0x70, 0x7f, 0x7b /* 0-9 */
};

void setup() {
  setupLCD();
  Serial.begin(115200);
  delay(2000);
}


void loop() {
  static uint32_t last = millis();
  uint32_t now = millis();
  if(now - last >= PERIOD) {
    last = now;
    degC  = readTemp();         // Misurazione della temperatura con sensore integrato nella CPU
   lcd_display_3dig((int)degC); // Visualizza la temperatura sul display
   Serial.print("Temp:");Serial.print(degC, 2);Serial.println("°C");  // Temperatura in uscita su seriale
  }

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
  //R_IOPORT_PinCfg(NULL, BSP_IO_PORT_01_PIN_02, IOPORT_PERIPHERAL_LCDC | IOPORT_CFG_PERIPHERAL_PIN); // VL3
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

float readTemp(){
  ADC_Container adc(0, NULL);
  uint16_t adc_data = 0; // R_ADC0->ADTSDR & 0xFFFF;
  float mcu_temp_c = 0;
  int16_t v125;
  float v1,vs;
  vCC   = readVcc();
  adc.cfg.resolution        = ADC_RESOLUTION_12_BIT;
  adc.channel_cfg.scan_mask = ADC_MASK_TEMPERATURE; //ADEXICR
  analogReference(AR_INTERNAL);
  R_ADC_Open(&adc.ctrl, &adc.cfg);
  R_ADC_ScanCfg(&adc.ctrl, &adc.channel_cfg);
  R_ADC_ScanStart(&adc.ctrl); // ADCSR.ADST
  delay(1);
    R_ADC_Read (&adc.ctrl, ADC_CHANNEL_TEMPERATURE, &adc_data);
    while( ADC_STATE_SCAN_IN_PROGRESS == status.state) {
     R_ADC_StatusGet(&adc.ctrl, &status);
    }
    R_ADC_InfoGet(&adc.ctrl, &adc_info);   
    v125 = (int16_t) adc_info.calibration_data; // v125   = (R_TSN->TSCDRH << 8) + R_TSN->TSCDRL
    v1 = 3.3f * v125 / 4096; // Voltage output by the TNS at 125 degreeC
    vs = vCC * adc_data / 4096; // Voltage output by the TNS at the time of measurement of T1 (volt)
    //Serial.print("vs: ");Serial.print(vs);Serial.print(" v1: ");Serial.println(v1);
    mcu_temp_c = (vs - v1) / SLOPE + 125;
    //Serial.println(mcu_temp_c);
    R_ADC_Close(&adc.ctrl);
    return mcu_temp_c;
}

float readVcc(){
ADC_Container adc(0,NULL);
adc.cfg.resolution               = ADC_RESOLUTION_12_BIT;
adc.channel_cfg.scan_mask        = ADC_MASK_VOLT;//ADEXICR
adc.cfg_extend.adc_vref_control  = ADC_VREF_CONTROL_AVCC0_AVSS0; //ADHVREFCNT
uint16_t adc_data = 0; // R_ADC0->ADOCDR & 0xFFFF;
float vCC;
R_ADC_Open(&adc.ctrl, &adc.cfg);
R_ADC_ScanCfg(&adc.ctrl, &adc.channel_cfg);
R_ADC_ScanStart(&adc.ctrl);// ADCSR.ADST
delay(1);
R_ADC_Read (&adc.ctrl, ADC_CHANNEL_VOLT, &adc_data);
    while( ADC_STATE_SCAN_IN_PROGRESS == status.state) {
     R_ADC_StatusGet(&adc.ctrl, &status);
    }
    vCC = (VREF * 4096) / adc_data; //4096 12bit - 16383 14bit
    //Serial.print("Vcc: ");Serial.print(vCC);Serial.println("V");
    R_ADC_Close(&adc.ctrl);
    return vCC;
}
