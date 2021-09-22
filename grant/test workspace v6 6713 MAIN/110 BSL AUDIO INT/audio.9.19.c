//-------------------------------------------------------------------
#include "dsk6713.h"
#include "dsk6713_aic23.h"  	  						//codec-dsk support file

//-------------------------------------------------------------------
//-------------------------------------------------------------------

//-------------------------------------------------------------------
DSK6713_AIC23_Config config1 = { \
    0x0017, /* Set-Up Reg 0       Left line input channel volume control */  \
            /* LRS     0          simultaneous left/right volume: disabled */\
            /* LIM     0          left line input mute: disabled */          \
            /* XX      00         reserved */                                \
            /* LIV     10111      left line input volume: 0 dB */            \
                                                                             \
    0x0017, /* Set-Up Reg 1       Right line input channel volume control */ \
            /* RLS     0          simultaneous right/left volume: disabled */\
            /* RIM     0          right line input mute: disabled */         \
            /* XX      00         reserved */                                \
            /* RIV     10111      right line input volume: 0 dB */           \
                                                                             \
    0x01f9, /* Set-Up Reg 2       Left channel headphone volume control */   \
            /* LRS     1          simultaneous left/right volume: enabled */ \
            /* LZC     1          left channel zero-cross detect: enabled */ \
            /* LHV     1111001    left headphone volume: 0 dB */             \
                                                                             \
    0x01f9, /* Set-Up Reg 3       Right channel headphone volume control */  \
            /* RLS     1          simultaneous right/left volume: enabled */ \
            /* RZC     1          right channel zero-cross detect: enabled */\
            /* RHV     1111001    right headphone volume: 0 dB */            \
                                                                             \
    0x0011, /* Set-Up Reg 4       Analog audio path control */               \
            /* X       0          reserved */                                \
            /* STA     00         sidetone attenuation: -6 dB */             \
            /* STE     0          sidetone: disabled */                      \
            /* DAC     1          DAC: selected */                           \
            /* BYP     0          bypass: off */                             \
            /* INSEL   0          input select for ADC: line */              \
            /* MICM    0          microphone mute: disabled */               \
            /* MICB    1          microphone boost: enabled */               \
                                                                             \
    0x0000, /* Set-Up Reg 5       Digital audio path control */              \
            /* XXXXX   00000      reserved */                                \
            /* DACM    0          DAC soft mute: disabled */                 \
            /* DEEMP   00         deemphasis control: disabled */            \
            /* ADCHP   0          ADC high-pass filter: disabled */          \
                                                                             \
    0x0000, /* Set-Up Reg 6       Power down control */                      \
            /* X       0          reserved */                                \
            /* OFF     0          device power: on (i.e. not off) */         \
            /* CLK     0          clock: on */                               \
            /* OSC     0          oscillator: on */                          \
            /* OUT     0          outputs: on */                             \
            /* DAC     0          DAC: on */                                 \
            /* ADC     0          ADC: on */                                 \
            /* MIC     0          microphone: on */                          \
            /* LINE    0          line input: on */                          \
                                                                             \
    0x0043, /* Set-Up Reg 7       Digital audio interface format */          \
            /* XX      00         reserved */                                \
            /* MS      1          master/slave mode: master */               \
            /* LRSWAP  0          DAC left/right swap: disabled */           \
            /* LRP     0          DAC lrp: MSB on 1st BCLK */                \
            /* IWL     00         input bit length: 16 bit */                \
            /* FOR     11         data format: DSP format */                 \
                                                                             \
    0x0081, /* Set-Up Reg 8       Sample rate control */                     \
            /* X       0          reserved */                                \
            /* CLKOUT  1          clock output divider: 2 (MCLK/2) */        \
            /* CLKIN   0          clock input divider: 2 (MCLK/2) */         \
            /* SR,BOSR 00000      sampling rate: ADC  48 kHz DAC  48 kHz */  \
            /* USB/N   1          clock mode select (USB/normal): USB */     \
                                                                             \
    0x0001  /* Set-Up Reg 9       Digital interface activation */            \
            /* XX..X   00000000   reserved */                                \
            /* ACT     1          active */                                  \
};


    
    
DSK6713_AIC23_CodecHandle hCodec;
Uint32 CODECEventId;

union {
	Uint32 uint;
	short channel[2];
	} AIC_data; 


//-------------------------------------------------------------------


//-------------------------------------------------------------------
void main(void);
void output_sample(int out_data);
Uint32 input_sample(void);
void init(void);
//-------------------------------------------------------------------


//-------------------------------------------------------------------
extern far void vectors();         //external function
//-------------------------------------------------------------------


//-------------------------------------------------------------------

//-------------------------------------------------------------------
void main(void)
{

	init();
	for(;;) // ever
	{

	} // ever
		 
} // void main(void)
//-------------------------------------------------------------------

//-------------------------------------------------------------------
void output_sample(int out_data)    //for out to Left and Right channels
{

	AIC_data.uint=out_data;          //32-bit data -->data structure

    MCBSP_write(DSK6713_AIC23_DATAHANDLE, AIC_data.uint);//write/output data
		
} // void output_sample(int out_data)
//-------------------------------------------------------------------


//-------------------------------------------------------------------
Uint32 input_sample(void)                      	  	//for 32-bit input
{

	AIC_data.uint=MCBSP_read(DSK6713_AIC23_DATAHANDLE);//read data

	return(AIC_data.uint);
} // Uint32 input_sample(void)
//-------------------------------------------------------------------



//-------------------------------------------------------------------
interrupt void c_int11(void)            //interrupt service routine
{ 
  output_sample(input_sample());    //output value
  return;
} // c_int11
//-------------------------------------------------------------------


//-------------------------------------------------------------------
void init(void)	     	 	//for communication/init using interrupt
{
	DSK6713_init();                   	//call BSL to init DSK-EMIF,PLL)
	CSL_init();
    IRQ_globalDisable();           	    //disable interrupts
   	hCodec = DSK6713_AIC23_openCodec(0, &config1);
                                        // codec has to be open before handle is referenced
//	* (volatile int *) 0x0190000C = 0x000000A0;						// prefer a double 16-bit frame
//	* (volatile int *) 0x01900010 = 0x000000A0;
	CODECEventId=MCBSP_getXmtEventId(DSK6713_AIC23_codecdatahandle);//McBSP1 Xmit

  	IRQ_setVecs(vectors);     			//point to the IRQ vector table

  	IRQ_map(CODECEventId, 11);			//map McBSP1 Xmit to INT11
  	IRQ_reset(CODECEventId);    		//reset codec INT 11
    IRQ_globalEnable();       			//globally enable interrupts
  	IRQ_nmiEnable();          			//enable NMI interrupt
    IRQ_enable(CODECEventId);			//enable CODEC eventXmit INT11

	output_sample(0);        			//start McBSP interrupt outputting a sample
} // init
//-------------------------------------------------------------------


 
 
