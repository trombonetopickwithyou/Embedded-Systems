
//-------------------------------------------------------------------
typedef short shortDP4;
typedef int intDP8;
//-------------------------------------------------------------------

//-------------------------------------------------------------------
#define BUFFLEN 64
//-------------------------------------------------------------------

//-------------------------------------------------------------------
shortDP4 DP4plusDP4(shortDP4 x, shortDP4 y);
intDP8 DP4timesDP4(shortDP4 x, shortDP4 y);
shortDP4 DP8rescaleDP4(intDP8 x);
short DP4rescaleshort(shortDP4 x);

//-------------------------------------------------------------------


//-------------------------------------------------------------------
int main(void)
{
	shortDP4 data[BUFFLEN];
	intDP8 scaledata[BUFFLEN];
	int unscaledata[BUFFLEN];

	shortDP4 scalefactor = (5);      
	                                 // one half is 1000 = "8"
	                                 // one third is 0101 = "5"
	                                 // two third is 1010+
	                                 //              1011 = "11"
	
	// .5 .25 .125 0625 .03125
    // 1   0   1    1               = .6875 = "11"
    // 1   0   1    0               = .625 = "10"
	// 1   0   0    0               = .5 = '8'
	// 0   1   1    0               = .375  = '6'
	// 0   1   0    1               = .3125 = '5'
	
	int i;
	
	for(i=0;i<BUFFLEN; i++)
	  {
	  	data[i] = i;          
	  	                      // initialize some data - slow ramp
	  	                      // numbers in DP4 format already, so LSB -> 1/16
	  	                      // this loop increments by 1/16 up to 63/16
	  } // for(i)
	  
	for(i=0;i<BUFFLEN; i++)
	  {
	  	unscaledata[i] = DP4rescaleshort(data[i]);
  
	  	                     // first rescale is 0/16 
	  	                     // second rescale is 1/16
	  	                     // moves back to int format
	  } // for(i)

	for(i=0;i<BUFFLEN-1; i++)
	  {
	  	data[i] = DP4plusDP4(data[i], data[i+1]); 
	  	                      // first add is 0 + 1/16
	  	                      // second add is 1/16 + 2/16
	  	                      // stays in DP4 format
	  } // for(i)
	  
	for(i=0;i<BUFFLEN; i++)
	  {
	  	scaledata[i] = DP4timesDP4(data[i],scalefactor);
                              // first multiply is 1/16 * 1/2
                              // second multiply is 3/16 * 1/2
                              // moves to DP8 format
	  } // for(i)

	for(i=0;i<BUFFLEN; i++)
	  {
	  	data[i] = DP8rescaleDP4(scaledata[i]);
  
	  	                     // first rescale is 1/32 
	  	                     // second rescale is 3/32
	  	                     // moves back to DP4 format
	  } // for(i)

	for(i=0;i<BUFFLEN; i++)
	  {
	  	unscaledata[i] = DP4rescaleshort(data[i]);
  
	  	                     // first rescale is 1/16 
	  	                     // second rescale is 3/16
	  	                     // moves back to int format
	  } // for(i)

	return unscaledata[0];
} // main
//-------------------------------------------------------------------

//-------------------------------------------------------------------
shortDP4 DP4plusDP4(shortDP4 x, shortDP4 y)
{
	return x+y;
} // DP4plusDP4
//-------------------------------------------------------------------

//-------------------------------------------------------------------
intDP8 DP4timesDP4(shortDP4 x, shortDP4 y)
{
	return x*y;
} // DP4timesDP4
//-------------------------------------------------------------------

//-------------------------------------------------------------------
shortDP4 DP8rescaleDP4(intDP8 x)
{
	int roundup;
	
	  	if(x & 0x8)
	  	  roundup = 1;
	  	else
	  	  roundup = 0;
	  	return (x >> 4) + roundup;

} // DP8rescaleDP4
//-------------------------------------------------------------------

//-------------------------------------------------------------------
short DP4rescaleshort(shortDP4 x)
{
	int roundup;
	
	  	if(x & 0x8)
	  	  roundup = 1;
	  	else
	  	  roundup = 0;
	  	return (x >> 4) + roundup;

} // DP4rescaleint
//-------------------------------------------------------------------
