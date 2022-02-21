//#define DEBUG // à commenter pour enlever le debug

#ifdef DEBUG
#define DPRINTF(s)   {Serial.print(F(s));}
#define DPRINT(x)  {Serial.print(x);}
#define DPRINTLN(x)  {Serial.println(x);}
#define DDELAY(x)
//#define TestTempBac
//#define TestTempRampe
//#define TestBatterie
//#define TestPH
//#define TestORP
//#define TestSalinite
//#define TestSpeed

#else
#define DPRINTF(s)
#define DPRINT(x)
#define DPRINTLN(x)
#define DDELAY(x)  {delay(x);}

#endif
