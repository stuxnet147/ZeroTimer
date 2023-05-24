typedef struct _KSYSTEM_TIME2
{
	unsigned long LowPart;
	long High1Time;
	long High2Time;
} KSYSTEM_TIME2, * PKSYSTEM_TIME2;

union TICK_COUNT_UNION
{
	KSYSTEM_TIME2 TickCount;
	unsigned long long TickCountQuad;
};

volatile TICK_COUNT_UNION* TickCountPtr = reinterpret_cast<TICK_COUNT_UNION* volatile>(0x7FFE0320);
volatile unsigned int* volatile TickCountMultiplierPtr = reinterpret_cast<unsigned int* volatile>(0x7FFE0004);

unsigned int GetTickCount_()
{
	return TickCountPtr->TickCountQuad * static_cast<unsigned long long>(*TickCountMultiplierPtr) >> 24;
}

#define concat_2(a1, a2) a1##a2
#define concat_1(a1, a2) concat_2(a1, a2)
#define var(prefix) concat_1(prefix, __LINE__)
#define TIMER(max_millisecond) \
	[]{\
		static unsigned int var(TIME_VAR_) = 0; \
		if (var(TIME_VAR_) == 0) \
		{ \
			var(TIME_VAR_) = GetTickCount_(); \
		} \
		else if (GetTickCount_() - var(TIME_VAR_) >= max_millisecond) \
		{ \
			var(TIME_VAR_) = 0; \
			return true; \
		} \
		return false;\
	}()

int main()
{
	while(1)
	{
		if (TIMER(1000))
		{
			// Timer expired
		}
		else
		{
			// Timer running
		}
	}
	return 0;
}
