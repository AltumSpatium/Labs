//**************************************************************************************************

#ifndef mregexpr_header
#define mregexpr_header

//**************************************************************************************************

class MRegExpr
{
    private:

		// Указатель на шаблон регулярного выражения.
		unsigned char* FTemplate;

    protected:				 //!!

		// Поиск символа в буфере двоичных данных.
		void* __stdcall MemChr(const void* _buffer, long _size, int _value, bool _last = false);

		// Поиск заданного символа в строке.
		char* __stdcall StrChr(const char* _source, int _value, bool _last = false);

		// Выделение памяти для сохранения шаблона.
		unsigned char* __stdcall GetBuffer(const char* _expression);

		// Поиск в шаблоне группирующего элемента.
		unsigned char* __stdcall GetPatternGroup(unsigned char* _pattern, bool _breaked);

		// Поиск в шаблоне заданного элемента.
		unsigned char* __stdcall GetPatternItem(unsigned char* _pattern, unsigned char _item, bool _last);

		// Рекурсивное сравнение строки с шаблоном.
		const char* __stdcall Advance(const char* _substr, unsigned char* _pattern);

    public:

		// Конструктор объекта.
		__stdcall MRegExpr(const char* _expression = 0);

		// Деструктор объекта.
		__stdcall ~MRegExpr(void);

		// Загрузка и компиляция регулярного выражения.
		bool __stdcall Compile(const char* _expression);

		// Получение признака загрузки регулярного выражения.
		bool __stdcall IsLoaded(void) const;

		// Проверка строки на соответствие регулярному выражению.
		bool __stdcall IsEqual(const char* _string);

		// Поиск подстроки, соответствующей регулярному выражению.
		const char* __stdcall Find(const char* _string, long& _length);
};

//**************************************************************************************************

#endif

//**************************************************************************************************
