/*
Original Project:
// UNO R4 で温度計を作る 20240424_UnoR4VccTempMeterV100.ino
// CPU温度と5V,3.3Vの電源電圧を測ってOLEDに表示
// 2024/4/26 ラジオペンチ http://radiopench.blog96.fc2.com/
Questo programma abilita la tensione di riferimento AREF di 1.45V e usa quest'ultima per misurare la tensione di alimentazione. Viene impostato l'ACD a 14 bit.
la temperatura e la tensione vengono misurate tramite le funzioni readTemp, readAref e readVcc.I risultati delle misurazioni vengono visualizzati in formato CSV sulla porta seriale.
P.S. Questo articolo è stato scritto partendo dal presupposto che la tensione di riferimento interna di 1,45 V abbia un piccolo margine di errore.
Tuttavia, la scheda tecnica del chip (Tabella 48.50 Caratteristiche della tensione di riferimento interna A/D) fornisce un margine di errore 
piuttosto ampio: min 1,36 V, tip 1,43 V, max 1,5 V.
Per ottenere il valore rele della temperatura correggere la variabile B0 fino ad avere sul monitor seriale la stessa temperatura del chip (Io ho usato un termometro ad infrarossi)
*/

#define A0             -0.00365  // Sensibilità del sensore di temperatura V/℃ (valore elencato nella scheda tecnica)
#define B0             3         // Correggere qui eventuale differenza di temperatura reale/misurata sul RA4M1
#define ADC_STD_V      1.45      // Tensione di riferimento interna del convertitore AD
 
int   dd;
char  cBuff[20];       // buffer di manipolazione delle stringhe
float v125;            // Voltaggio di calibrazione 125℃ (valore calcolato dalle costanti del chip)
int   nnn;             // Numero di ripetizioni della misurazione
long  sumD;            // Valore cumulativo dell'ADC
float aveD, vvv, ttt;  // Conversione dei risultati della misurazione
 
 
void setup() {
  pinMode(21, OUTPUT);
  set48MhzClock();
  rtcSOSC();
  Serial.begin(115200);
  delay(2000);
 
  Serial.println("20240426_UnoR4VccTempMeterV100 start!");
  dd   = ((R_TSN->TSCDRH << 8) + R_TSN->TSCDRL);  // Acquisizione del valore ADC del valore di calibrazione della temperatura (125 ℃/3,3 V/12 bit)
  v125 = 3.3 * dd / 4096.0;                       // Convertito in valore di calibrazione della tensione di 125 ℃
  setupADC14();                                   // Impostazioni iniziali dell'ADC a 14 bit
  dumpTSNregs();                                  // Scarica il contenuto dei registri correlati
}
 
void loop() {
  float degC, vAref, vCC;
 
  digitalWrite(21, HIGH);  // LED indicatore di funzionamento acceso
  degC  = readTemp();      // Misurazione della temperatura con sensore integrato nella CPU
  vAref = readAref();      // Misurazione della tensione del pin Aref (3,3 V)
  vCC   = readVcc();       // Misura della tensione Vcc (5V).
  digitalWrite(21, LOW);   // LED off
 
  Serial.print("Temp:");Serial.print(degC, 2);Serial.print("°C");  // Temperatura di uscita in serie
  Serial.print(", ");
  Serial.print("vAref:");Serial.print(vAref, 3);Serial.print("V");  // Uscita in tensione Aref in serie
  Serial.print(", ");
  Serial.print("vCC:");Serial.print(vCC, 3);Serial.println("V");  // Uscita tensione Vcc in serie
  delay(270);      // Regola il periodo del loop = 300 ms
}
 
float readTemp() {  // Misurazione della temperatura del chip con sensore TSN
  nnn  = 100;       // Numero di ripetizioni della misurazione
  sumD = 0;
 
  R_ADC0->ADEXICR_b.OCSA     = 0;     // Non misurare la tensione di riferimento interna
  R_ADC0->ADEXICR_b.TSSA     = 1;     // Sensore di temperatura di misurazione
  R_ADC0->ADHVREFCNT_b.HVSEL = 0b01;  // Utilizzare VREFH0 come tensione di riferimento (pin AREF: 3,3 V)
  for (int n = 0; n < nnn; n++) {     // Misurazioni ripetute per la media
    runAdc();                         // Esecuzione dell'ADC
    sumD += R_ADC0->ADTSDR & 0xFFFF;  // Leggere e accumulare i valori del sensore di temperatura A/D
  }
  aveD = (float)sumD / nnn;             // calcolare il valore medio
  vvv  = 3.3 * aveD / 4096.0;           // Convertire in tensione
  ttt  = (vvv - v125) / A0 + 125 + B0;  // calcolare la temperatura
  return ttt;
}
 
float readAref() {  // Misurare la tensione AREF
  nnn  = 100;       // Numero di ripetizioni
  sumD = 0;
 
  R_ADC0->ADEXICR_b.OCSA     = 1;     // Misurare la tensione di riferimento interna
  R_ADC0->ADEXICR_b.TSSA     = 0;     // Il sensore di temperatura non misura
  R_ADC0->ADHVREFCNT_b.HVSEL = 0b01;  // Utilizzare VREFH0 come tensione di riferimento (pin AREF: 3,3 V)
  for (int n = 0; n < nnn; n++) {     // Misurazioni ripetute per la media
    runAdc();                         // Esecuzione dell'ADC
    sumD += R_ADC0->ADOCDR & 0xFFFF;  // Leggere e accumulare il valore della tensione di riferimento interno (1,45 V)
  }
  aveD = (float)sumD / nnn;          // calcolare il valore medio
  vvv  = ADC_STD_V * 4096.0 / aveD;  // Trova la tensione di Aref facendo il calcolo all'indietro
  return vvv;
}
 
float readVcc() {  // Misurare la tensione di Vcc
  nnn  = 100;      // Numero di ripetizioni
  sumD = 0;
 
  R_ADC0->ADEXICR_b.OCSA     = 1;     // Misurare la tensione di riferimento interna
  R_ADC0->ADEXICR_b.TSSA     = 0;     // Il sensore di temperatura non misura
  R_ADC0->ADHVREFCNT_b.HVSEL = 0b00;  // Utilizzare AVCC (5 V) come tensione di riferimento
  for (int n = 0; n < nnn; n++) {     // Misurazioni ripetute per la media
    runAdc();                         // Esecuzione dell'ADC
    sumD += R_ADC0->ADOCDR & 0xFFFF;  // Leggere e accumulare il valore della tensione di riferimento interno (1,45 V)
  }
  aveD = (float)sumD / nnn;          // calcolare il valore medio
  vvv  = ADC_STD_V * 4096.0 / aveD;  // Trova la tensione di Vcc effettuando il calcolo all'indietro
  return vvv;
}
 
void runAdc() {                        // Esegui l'ADC
  R_ADC0->ADCSR_b.ADST = 1;            // Avvia la conversione AD
  while (R_ADC0->ADCSR_b.ADST == 1) {  // Attendi il completamento della conversione
  }                                    //
}
 
void setupADC14() {                    // Impostazioni di base per ADC a 14 bit
  R_SYSTEM->PRCR            = 0xA501;  // Sgancio protezione registro orologio
  R_SYSTEM->SCKDIVCR_b.PCKC = 0b010;   // Imposta l'orologio del modulo periferico in modo che divida per 4 (12 MHz) (se impostato su 010)
  R_SYSTEM->PRCR            = 0xA500;  // Riproteggere il registro dell'orologio
 
  R_ADC0->ADCSR_b.ADST = 0;            // Interrompere l'ADC per la configurazione
  while (R_ADC0->ADCSR_b.ADST == 1) {  // Attendere fino all'arresto dell'ADC
  }                                    //
  R_ADC0->ADCER_b.ADPRC      = 0b00;   // Specificare la precisione della conversione ADC su 12 bit
  R_ADC0->ADCER_b.ADRFMT     = 0;      // I risultati dell'ADC sono giustamente giustificati
  R_ADC0->ADANSA[0]          = 0;      // Cancella il registro di selezione del canale AD A0
  R_ADC0->ADANSA[1]          = 0;      // Cancella il registro di selezione del canale AD A1
  R_ADC0->ADANSB[0]          = 0;      // Cancella il registro di selezione del canale AD B0
  R_ADC0->ADANSB[1]          = 0;      // Cancella il registro di selezione del canale AD B1
  R_ADC0->ADCSR_b.GBADIE     = 0;      // Nessuna interruzione GBADI
  R_ADC0->ADCSR_b.DBLE       = 0;      // Nessun doppio trigger
  R_ADC0->ADEXICR_b.OCSA     = 1;      // Selettore di tensione di riferimento interno (provvisorio)
  R_ADC0->ADEXICR_b.TSSA     = 0;      // Selettore del sensore di temperatura (provvisorio)
  R_ADC0->ADSSTRT            = 200;    // Imposta il tempo di campionamento (200*1/12Mhz=16,7us)
  R_ADC0->ADHVREFCNT_b.HVSEL = 0b01;   // Selezione della tensione a fondo scala (provvisoria)
}
 
void dumpTSNregs() {
  sprintf(cBuff, "TSCDRH = 0x%02x, ", R_TSN->TSCDRH);           // Registro di calibrazione del sensore di temperatura H
  Serial.print(cBuff);                                          // uscita in seriale
  sprintf(cBuff, "TSCDRL = 0x%02x, ", R_TSN->TSCDRL);           // Registro di calibrazione del sensore di temperatura L
  Serial.print(cBuff);                                          // uscita in seriale
  sprintf(cBuff, "ADCSR = 0x%04x, ", R_ADC0->ADCSR);            //
  Serial.print(cBuff);                                          // uscita in seriale
  sprintf(cBuff, "ADCER = 0x%04x, ", R_ADC0->ADCER);            //
  Serial.print(cBuff);                                          // uscita in seriale
  sprintf(cBuff, "ADEXICR = 0x%04x, ", R_ADC0->ADEXICR);        //
  Serial.print(cBuff);                                          // uscita in seriale
  sprintf(cBuff, "ADSSTRT = 0x%02x, ", R_ADC0->ADSSTRT);        //
  Serial.print(cBuff);                                          // uscita in seriale
  sprintf(cBuff, "ADHVREFCNT = 0x%02x, ", R_ADC0->ADHVREFCNT);  //
  Serial.print(cBuff);                                          // uscita in seriale
  Serial.print("v125 = ");                                      //
  Serial.print(v125, 5);                                        // Valore di calibrazione 125 ℃
  Serial.println();
}

//-------------This function to set 48Mhz with external quartz of RA4M1 (only if your board have external 16Mhz OSC, in this case I use Weact Studio RA4M1 board)
void set48MhzClock(){ // WITH Externel Quartz of 16Mhz AND PLL
  R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_CGC); // Disable register protection
  R_SYSTEM->MOSCCR_b.MOSTP = 1;   // SET MOSTP bit to 1 to stop Main Clock Oscillator
  asm volatile ( "dsb" );         // Data bus Synchronization instruction (?)
  R_SYSTEM->MOMCR_b.MODRV1 = 0;   // MODRV1 = 0 (10 MHz to 20 MHz) becouse external quartz is 16Mhz
  R_SYSTEM->MOSCWTCR       = 0x07;// Set stability timeout period (?)
  R_SYSTEM->MOSCCR_b.MOSTP = 0;   // Enable XTAL
  R_BSP_SoftwareDelay(100, BSP_DELAY_UNITS_MILLISECONDS); // wait for XTAL to stabilise
// ----------------------------- and now ... setup PLL Frequency Multiplication to 12x and PLL Frequency Division to /4 (16*12/4 = 48Mhz)
R_SYSTEM->PLLCR_b.PLLSTP = 1;   // Disable PLL
R_SYSTEM->PLLCCR2        = 0x0B; // PLL Frequency Multiplication to 12x
R_SYSTEM->PLLCCR2       |= 0x80; // Output Frequency Division to /4
R_SYSTEM->PLLCR_b.PLLSTP = 0;   // Enable PLL
R_BSP_SoftwareDelay(1000, BSP_DELAY_UNITS_MICROSECONDS);// wait for PLL to stabilise
if (R_SYSTEM->OSCSF == 0b0101001){ // Check for stabile XTAL, before allowing switch
  R_SYSTEM->SCKSCR  = 0x05;          // Select PLL as the system clock
  Serial.println("System clock changed to XTAL with PLL"); 
} else {
  Serial.println("XTAL or PLL not stable, remain on HOCO clock");
}
R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_CGC);// Enable register protection
}

void rtcSOSC() { // Swith to external 32.768Khz Sub-OScillator (24.3.3 User Manual)
  R_RTC->RCR4_b.RCKSEL = 0; //Select SOSC
  delay(10);
  R_RTC->RCR2_b.START = 0;            // Temporarily stop RTC
  while (R_RTC->RCR2_b.START == 1) {}  // Wait until RTC stops
  R_RTC->RCR2_b.RESET = 1;            // Restart RTC
  while (R_RTC->RCR2_b.RESET == 1) {  // Wait until RTC restarts
  //adj60SEC();
  }
}
