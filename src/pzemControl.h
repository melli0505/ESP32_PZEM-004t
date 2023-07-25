#include <PZEM004Tv30.h>

void setup_pzem(PZEM004Tv30* pzem_instance);

float get_voltage(void);
float get_current(void);
float get_power(void);
float get_energy(void);
float get_frequency(void);
float get_pf(void);

bool reset_energy_counter(void);