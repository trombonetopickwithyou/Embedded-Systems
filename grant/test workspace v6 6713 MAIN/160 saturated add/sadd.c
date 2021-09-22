//---------------------------------------------------------------------------------------------
int testnum(int x, int y);
//---------------------------------------------------------------------------------------------



//---------------------------------------------------------------------------------------------
void main(void)
{
	long a;
	long b = 0x1234567890ab;	// long is only 40 bits, so we throw away MSB. Compiler warning
	long c = 0x234567890abc;	// Another warning for the same reason
	a = b + c;					// long + long has no saturation protection

	a += a;						// goes negative
	a += a;
	a += a;
	a += a;
	a += a;
	
	testnum(0x7FFFFFFF, 0x7FFFFFFF);
	testnum(0x80000000, 0x80000000);
	testnum(0x7FFFFFFF, 0x80000000);

} // void main(void)
//---------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------
int testnum(int x, int y)
{
	int zint;
	long zlong;
	
	zint = x + y;
	zlong = x+y;
	zint = _sat(zlong);
	zint = _sadd(x,y);
	zint = _add2(x,y);
    zlong = _lsadd(x,y);
	zint = _sat(zlong);
    zint = x*y;
	zint = _smpy(x,y);			// treat x and y as 16-bit numbers
	y = abs(x);
	y = _abs(x);
	zlong = abs(x);
	zlong = _abs(x);
	zlong = _labs(x);
	
	return zint;				// getting the compiler not to complain about unused variable

} // void testnum(int x, int y)
//---------------------------------------------------------------------------------------------


