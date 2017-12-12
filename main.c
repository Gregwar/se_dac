#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>

typedef enum {
    CHANNEL_A=0,
    CHANNEL_B=1
} DAC_CHANNEL;

typedef enum {
    GAIN_1=0,
    GAIN_2=1
} DAC_GAIN;

typedef enum {
    SHUTDOWN_YES=0,
    SHUTDOWN_NO=1
} DAC_SHUTDOWN;

#define SET_PIN(PORT, PIN, ON) \
    if (ON) PORT |= _BV(PIN); \
    else PORT &= ~_BV(PIN);

void dac_send_bit(uint8_t bit)
{
    SET_PIN(PORTB, PB3, bit);
    SET_PIN(PORTB, PB5, 1);
    SET_PIN(PORTB, PB5, 0);
}

void dac_sample(DAC_CHANNEL channel, DAC_GAIN gain,
    DAC_SHUTDOWN shutdown, uint32_t sample)
{
    int k;
    SET_PIN(PORTB, PB2, 0);

    // Channel
    dac_send_bit(channel);
    dac_send_bit(0);
    dac_send_bit(gain);
    dac_send_bit(shutdown);

    for (k=11; k>=0; k--) {
        dac_send_bit((sample>>k)&1);
    }

    SET_PIN(PORTB, PB2, 1);
}

int main()
{
    DDRB |= _BV(PB5); // SCK
    DDRB |= _BV(PB3); // SDI
    DDRB |= _BV(PB2); // SS

    SET_PIN(PORTB, PB5, 0);
    SET_PIN(PORTB, PB2, 1);

    uint32_t v = 0;
    while (1) {
        dac_sample(CHANNEL_A, GAIN_1, SHUTDOWN_NO, v);
        _delay_us(10);
        v += 1;
        v = v & 0b111111111111;
    }
}
