#include "Arduino.h"

void jumpToApplicationAt0x38080() {
  /* Load stack pointer and program counter from start of new program */
  asm("movw r0, #0x8080");
  asm("movt r0, #0x0003");
  asm("ldr sp, [r0]");
  asm("ldr pc, [r0, #4]");
}

void jumpToApplicationAt0x8080() {
  /* Load stack pointer and program counter from start of new program */
  asm("movw r0, #0x8080");
  asm("ldr sp, [r0]");
  asm("ldr pc, [r0, #4]");
}

/*
 * These are the minimum peripherals that needed to be disabled to allow the
 * uTasker USB-MSD application to work.  You may need to reset more peripherals
 * depending on the application you are running, and what other peripherals
 * your sketch uses if you add more to this example than just blinking an LED
 */
void resetPeripherals() {
  /* set (some of) USB back to normal */
  NVIC_DISABLE_IRQ(IRQ_USBOTG);
  NVIC_CLEAR_PENDING(IRQ_USBOTG);
  SIM_SCGC4 &= ~(SIM_SCGC4_USBOTG);

  /* disable all GPIO interrupts */
  NVIC_DISABLE_IRQ(IRQ_PORTA);
  NVIC_DISABLE_IRQ(IRQ_PORTB);
  NVIC_DISABLE_IRQ(IRQ_PORTC);
  NVIC_DISABLE_IRQ(IRQ_PORTD);
  NVIC_DISABLE_IRQ(IRQ_PORTE);

  /* set (some of) ADC1 back to normal */
  // wait until calibration is complete
  while (ADC1_SC3 & ADC_SC3_CAL);

  // clear flag if calibration failed
  if (ADC1_SC3 & 1 << 6)
    ADC1_SC3 |= 1 << 6;

  // clear conversion complete flag (which could trigger ISR otherwise)
  if (ADC1_SC1A & 1 << 7)
    ADC1_SC1A |= 1 << 7;

  /* set some clocks back to default/reset settings */
  MCG_C1 = MCG_C1_CLKS(2) | MCG_C1_FRDIV(4);
  SIM_CLKDIV1 = 0;
  SIM_CLKDIV2 = 0;
}

void startup_late_hook(void) {
  // look for the condition that indicates we want to jump to the application with offset
  if (eeprom_read_byte(0) == 0xAE) {

    // clear the condition
    eeprom_write_byte(0, 0);

    // set peripherals (mostly) back to normal then jump
    __disable_irq();
    resetPeripherals();
    jumpToApplicationAt0x38080();
  }
}
