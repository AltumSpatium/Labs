//**************************************************************************************************

#ifndef mregexpr_header
#define mregexpr_header

//**************************************************************************************************

class MRegExpr
{
    private:

		// ��������� �� ������ ����������� ���������.
		unsigned char* FTemplate;

    protected:				 //!!

		// ����� ������� � ������ �������� ������.
		void* __stdcall MemChr(const void* _buffer, long _size, int _value, bool _last = false);

		// ����� ��������� ������� � ������.
		char* __stdcall StrChr(const char* _source, int _value, bool _last = false);

		// ��������� ������ ��� ���������� �������.
		unsigned char* __stdcall GetBuffer(const char* _expression);

		// ����� � ������� ������������� ��������.
		unsigned char* __stdcall GetPatternGroup(unsigned char* _pattern, bool _breaked);

		// ����� � ������� ��������� ��������.
		unsigned char* __stdcall GetPatternItem(unsigned char* _pattern, unsigned char _item, bool _last);

		// ����������� ��������� ������ � ��������.
		const char* __stdcall Advance(const char* _substr, unsigned char* _pattern);

    public:

		// ����������� �������.
		__stdcall MRegExpr(const char* _expression = 0);

		// ���������� �������.
		__stdcall ~MRegExpr(void);

		// �������� � ���������� ����������� ���������.
		bool __stdcall Compile(const char* _expression);

		// ��������� �������� �������� ����������� ���������.
		bool __stdcall IsLoaded(void) const;

		// �������� ������ �� ������������ ����������� ���������.
		bool __stdcall IsEqual(const char* _string);

		// ����� ���������, ��������������� ����������� ���������.
		const char* __stdcall Find(const char* _string, long& _length);
};

//**************************************************************************************************

#endif

//**************************************************************************************************
