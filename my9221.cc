// This is just cut and paste from my test code; I'll build out something a little better soon.

// This is using the Arduino libraries, but you get the idea.

class MY9221 {
private:
    uint8_t clock_pin;
    uint8_t data_pin;
public:
    MY9221(uint8_t clock, uint8_t data) : clock_pin(clock), data_pin(data) {}
    void init(uint8_t chain_len = 1) {
        pinMode(clock_pin, OUTPUT);
        digitalWrite(clock_pin, LOW);
        pinMode(data_pin, OUTPUT);
        digitalWrite(data_pin, LOW);
        for (;chain_len > 0;chain_len--) {
            // clock in zeros for every chip in the chain
            for (int i = 0; i < 13; i++) shift_16(0);
        }
        latch();
    }
    

    void shift_16(uint16_t data) {
        digitalWrite(clock_pin, LOW);  
        // 16 bits, both clock edges, LSB first
        for (int i = 0; i < 8; i++) {
            digitalWrite(data_pin, ((data & 0x8000) == 0) ? LOW:HIGH);
            digitalWrite(clock_pin, HIGH);
            digitalWrite(data_pin, ((data & 0x4000) == 0) ? LOW:HIGH);
            digitalWrite(clock_pin, LOW);
            data <<= 2;
        } 
    }
    
    void latch() {
        digitalWrite(data_pin, LOW);
        delayMicroseconds(240); 
        for (int i = 0; i < 4; i++) {
            digitalWrite(data_pin, HIGH);
            digitalWrite(data_pin, LOW);
        }
        delayMicroseconds(1); 
    }
};


