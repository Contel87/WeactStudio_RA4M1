/* Esempio di programmazione ad oggetti ... Blink del led sulla WeAct Studio RA4M1 con il bootloader della Minima R4 */

bsp_io_port_pin_t io_pin = g_pin_cfg[21].pin; // Sulla Minima R4 il led è sul pin 21 (P012)
#define PIN_IO_PORT_ADDR(pn)      (R_PORT0 + ((uint32_t) (R_PORT1 - R_PORT0) * ((pn) >> 8u)))

  volatile uint16_t *set = &(PIN_IO_PORT_ADDR(io_pin)->POSR);
  volatile uint16_t *clr = &(PIN_IO_PORT_ADDR(io_pin)->PORR);
  uint16_t msk = (1U << (io_pin & 0xFF));

/* L'oggetto "led" e' una struttura contenente 3 puntatori a funzioni */
struct led{
    void (*on)(struct led *p);
    void (*off)(struct led *p);
    void (*run)(void);
};

/* Led acceso */
static void ra_led_on(struct led *p){
    *set = msk;
}

/* Led spento */
static void ra_led_off(struct led *p){
    *clr = msk;
}

/* La variabile "g_led" contenente i 3 puntatori a funzione, permette a questi ultimi di puntare a specifiche funzioni*/
static struct led g_led = {
    .on = ra_led_on,
    .off = ra_led_off,
    .run = led_blink,
};

/* Led lampeggiante */
void led_blink(){    
        g_led.on(&g_led);
        delay(500);
        g_led.off(&g_led);
        delay(500);
}

void setup() {
  //pinMode(21, OUTPUT); // Imposta P012 OUTPUT - Variante 1
  //R_IOPORT_PinCfg(NULL, BSP_IO_PORT_00_PIN_12, IOPORT_CFG_PORT_DIRECTION_OUTPUT); // Imposta P012 OUTPUT - Variante 2
  R_PFS->PORT[0].PIN[12].PmnPFS_b.PDR = 1; // Imposta P012 OUTPUT - Variante 3
  
  /*
  // enable writing to the pin registers
    R_PMISC->PWPR = 0;          // Clear BOWI bit - writing to PFSWE bit enabled
    R_PMISC->PWPR = 1U << 6;    // Set PFSWE bit - writing to PFS register enabled

    // Set GPIO output mode
    R_PFS->PORT[0].PIN[12].PmnPFS_b.PDR = 1;  // "Output"
    R_PFS->PORT[0].PIN[12].PmnPFS_b.DSCR = 1; // "Middle Drive capability"
    R_PFS->PORT[0].PIN[12].PmnPFS_b.PMR = 0;  // "Used as a general I/O pin"
    */
}

void loop() {
  g_led.run(); 
}

