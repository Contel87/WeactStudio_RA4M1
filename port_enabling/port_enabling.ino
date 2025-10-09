/*
 * WeActStudio RA4M1 caricata con il bootloader della "nano R4"
 * Il led sulla GPIO P012 deve essere abilitato con la funzione pinPeripheral()
 * che fa riferimento alla funzione R_IOPORT_PinCfg() perchè sulla "nano R4" la 
 * porta P012 non esiste ...
 */



void setup() {
  pinPeripheral(BSP_IO_PORT_00_PIN_12, IOPORT_CFG_PORT_DIRECTION_OUTPUT);
   R_IOPORT_PinWrite(nullptr, BSP_IO_PORT_00_PIN_12, BSP_IO_LEVEL_HIGH);
   delay(1000);
   R_IOPORT_PinWrite(nullptr, BSP_IO_PORT_00_PIN_12, BSP_IO_LEVEL_LOW);
}

void loop() {
  // put your main code here, to run repeatedly:

}
