//*************************************************************************************************

#include <stdafx.h>
#include <stdlib.h>
#include <stdio.h>
#include <io.h>
#include <mregexpr.h>
#include <windows.h>

//*************************************************************************************************
// Реализация MRegExpr.

// Определение идентификаторов элементов шаблона.
#define MREGEXPR_CHAR ((unsigned char) 0x01) // Заданный символ (может быть задан как (\xDD), где DD - шестнадцатеричный код символа)
#define MREGEXPR_CDIG ((unsigned char) 0x02) // Цифра (\d)
#define MREGEXPR_CSYM ((unsigned char) 0x03) // Алфавитный символ (\s)
#define MREGEXPR_CASC ((unsigned char) 0x04) // Цифра | Алфавитный символ | Символ подчеркивания (\w)
#define MREGEXPR_CSPC ((unsigned char) 0x05) // Пробельные символы (\t)
#define MREGEXPR_CLAT ((unsigned char) 0x06) // Латинский символ (\a)
#define MREGEXPR_CCYR ((unsigned char) 0x07) // Кириллический символ (\c)
#define MREGEXPR_CMRK ((unsigned char) 0x08) // Знак препинания (\m)
#define MREGEXPR_CHEX ((unsigned char) 0x09) // Шестнадцатеричный символ (\h)
#define MREGEXPR_CEOL ((unsigned char) 0x0A) // Символ конца строки (\$)
#define MREGEXPR_CDOT ((unsigned char) 0x10) // Любой символ ( . )
#define MREGEXPR_CCLS ((unsigned char) 0x20) // Класс символов ( [] )
#define MREGEXPR_VRNT ((unsigned char) 0x30) // Вариант выбора ( | )
#define MREGEXPR_SBRK ((unsigned char) 0x40) // Начало блока выбора ( ( )
#define MREGEXPR_EBRK ((unsigned char) 0x50) // Конец блока выбора ( ) )
#define MREGEXPR_RNGE ((unsigned char) 0x80) // Квантификатор повторений ( ? + * {} )

// Набор алфавитных символов.
unsigned char RegExprSymbols[] =
  "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
  "АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯЇЎабвгдеёжзийклмнопрстуфхцчшщъыьэюяїў";

// Набор знаков препинания.
unsigned char RegExprMarks[] =
  "!?.,;:()[]{}";

// Набор шестнадцатеричных символов.
unsigned char RegExprHexs[] =
  "0123456789ABCDEFabcdef";

//.................................................................................................
// Конструктор объекта.

__stdcall MRegExpr::MRegExpr(const char* _expression) : FTemplate(0)  
{
	if(_expression)
		Compile(_expression);
}

//.................................................................................................
// Деструктор объекта.

__stdcall MRegExpr::~MRegExpr(void)
{
	free(FTemplate);
	FTemplate = 0;
}

//.................................................................................................
// Поиск символа в буфере двоичных данных.

void* __stdcall MRegExpr::MemChr(const void* _buffer, long _size, int _value, bool _last)
{
	if (!_buffer || _size <= 0)
		return 0;
	else if (!_last)
		return (void*) memchr(_buffer, _value, _size);		  

	char* result = ((char*) _buffer) + (_size - 1);
	while (_size-- && (*((char*) result) != ((char) _value)))		 
		result--;
	return (void*) (_size >= 0 ? result : 0);
}

//..................................................................................................
// Поиск заданного символа в строке.

char* __stdcall MRegExpr::StrChr(const char* _source, int _value, bool _last)
{
	if (!_last)
		return (char*) strchr(_source, _value);
	else
	    return (char*) strrchr(_source, _value);
}

//..................................................................................................
// Выделение памяти для сохранения шаблона.

unsigned char* __stdcall MRegExpr::GetBuffer(const char* _expression)
{
	if (!_expression || !_expression[0])	  
		return 0;

	unsigned long size = 3;
	unsigned char chr;

	while ((chr = *_expression++) != 0)
	{
		if (chr == '\\')
		{
			size += 2;
			if ((chr == *_expression++) == 0)
				break;
		}
		else if (chr == '?' || chr == '+' || chr == '*')
			size += 2; 
		else if (chr == '(' || chr == ')' || chr == '|')
			size += 1; 
		else if (chr == '{')
		{
			while ((chr = *_expression++) != 0)
			{
				if (chr == '}')
					break;
				if (chr != ',' && (chr < '0' || chr > '9'))
					break;
			}

			if (chr != '}')
				return 0;
			else
				size += 2;
		}
		else if (chr == '[')
		{
			while ((chr = *_expression++) != 0)
			{
				if (chr == '\\')
				{
					if ((chr = *_expression++) == 0)   
						break;
				}
				else if (chr == ']')
					break;
			}

			if (chr != ']')
				return 0;
			else
				size += 33;		   
		}
		else
			size += 2;	 
	}

	unsigned char* buffer = (unsigned char*) malloc(size);
	if (buffer)
	{
		do
		    buffer[--size] = 0;
		while(size);
	}
	return buffer;
}

//.................................................................................................
// Поиск в шаблоне заданного элемента.

unsigned char* __stdcall MRegExpr::GetPatternGroup(unsigned char* _pattern, bool _breaked)  
{
	long brkcount = 0; 

	if (_pattern)
	{
		while (*_pattern)				
		{
			if ((*_pattern & ~MREGEXPR_RNGE) == MREGEXPR_SBRK)	   
				brkcount++;
			else if (*_pattern == MREGEXPR_EBRK && !brkcount-- && _breaked)		 
				return _pattern;
			else if (!brkcount && *_pattern == MREGEXPR_VRNT && !_breaked)		
				return _pattern;

			unsigned char currpat = *_pattern++;					

			if (currpat & MREGEXPR_RNGE)						   
				_pattern += 2;									 
			if ((currpat & ~MREGEXPR_RNGE) == MREGEXPR_CHAR)	  
				_pattern++;										  
			if ((currpat & ~MREGEXPR_RNGE) == MREGEXPR_CCLS)	  
				_pattern +=32;									  
		}
	}
	return 0;
}

//.................................................................................................
// Поиск в шаблоне заданного элемента.

unsigned char* __stdcall MRegExpr::GetPatternItem(unsigned char* _pattern, unsigned char _item, bool _last)
{
	unsigned char* lastpos = 0;	  

	if (_pattern)
	{
		while (*_pattern)
		{
			if (!_item || *_pattern == _item)		   
			{
				lastpos = _pattern;						
				if (!_last)								  
					break;
			}

			unsigned char currpat = *_pattern++;			

			if (currpat & MREGEXPR_RNGE)					
				_pattern += 2;									
			if ((currpat & ~MREGEXPR_RNGE) == MREGEXPR_CHAR)	
				_pattern++;												
			if ((currpat & ~MREGEXPR_RNGE) == MREGEXPR_CCLS)				
				_pattern += 32;
		}
	}
	return lastpos;
}

//.................................................................................................
// Рекурсивное сравнение строки с шаблоном.

const char* __stdcall MRegExpr::Advance (const char* _substr, unsigned char* _pattern)
{
    unsigned char chr;
    short rmin, rmax;
    const unsigned char* substr = (const unsigned char*) _substr;
    const unsigned char* currstr = 0;
    const unsigned char* nextstr;
    unsigned char* nextpat;

    while(*_pattern)
    {
        switch(*_pattern++)
        {
            case MREGEXPR_VRNT:

            case MREGEXPR_EBRK:
                return (const char*) substr;

            case MREGEXPR_SBRK:
                if ((nextpat = GetPatternGroup(_pattern, true)) != 0)
                {
                    nextpat++;
                    _pattern--;

                    do
                    {
                        if ((currstr = (const unsigned char*) Advance((const char*) substr, ++_pattern)) != 0 &&
                            (currstr = (const unsigned char*) Advance((const char*) currstr, nextpat)) != 0)
                            return (const char*) currstr;

                    } while((_pattern = GetPatternGroup(_pattern, false)) != 0);
				}
                break;

                case MREGEXPR_CHAR:
                    if (*substr++ == *_pattern++)
						continue;
                    break;

                case MREGEXPR_CDIG:
                    chr = *substr++;
                    if (chr >= '0' && chr <= '9')
						continue;
                    break;

                case MREGEXPR_CSYM:
                    if (StrChr((const char*) RegExprSymbols, *substr++))
						continue;
                    break;

                case MREGEXPR_CLAT:
                    if (MemChr((const char*) RegExprSymbols, 52, *substr++))
						continue;
                    break;

                case MREGEXPR_CCYR:
                    if (StrChr((const char*) RegExprSymbols + 52, *substr++))
						continue;
                    break;

                case MREGEXPR_CMRK:
                    if (StrChr((const char*) RegExprMarks, *substr++))
						continue;
                    break;

                case MREGEXPR_CHEX:
                    if (StrChr((const char*) RegExprHexs, *substr++))
						continue;
                    break;

                case MREGEXPR_CEOL:
                    if ((chr = *substr++) == '\n')
						continue;
                    break;

                case MREGEXPR_CASC:
                    if ((chr = *substr++) == '_' || (chr >= '0' && chr <= '9') ||
                        StrChr((const char*) RegExprSymbols, chr))
						continue;
                    break;

                case MREGEXPR_CSPC:
                    if ((chr = *substr++) == ' ' || chr == '\r' || chr == '\n' ||
                        chr == '\t' || chr == '\f' || chr == '\b')
						continue;
                    break;

                case MREGEXPR_CDOT:
                    if (*substr++)
						continue;
                    break;

                case MREGEXPR_CCLS:
                    if (_pattern[(*substr >> 3)] & (0x80 >> (*substr & 0x07)))
                    {
                        substr++;
                        _pattern += 32;
                        continue;
                    }
                    break;

                case (MREGEXPR_SBRK | MREGEXPR_RNGE):
                    rmin = (short) *_pattern++;
                    rmax = (short) *_pattern++;

                    if ((nextpat = GetPatternGroup(_pattern, true)) != 0)
                    {
                        nextpat++;
                        _pattern--;

                        do
                        {
                            short lrmin = rmin;
                            short lrmax = rmax;

                            nextstr = substr;
                            _pattern++;

                            while (lrmin--)
                                if ((nextstr = (const unsigned char*) Advance((const char*) nextstr, _pattern)) == 0)
                                    break;

                                if ((currstr = nextstr) == 0)
                                    continue;
                                else if (!lrmax)
                                {
                                    if ((nextstr = (const unsigned char*) Advance((const char*) currstr, nextpat)) == 0)
                                        continue;
                
                                    return (const char*) nextstr;
                                }
                                else
                                {
                                    long index = 0;
                                    const char* strlst[256];

                                    strlst[0] = 0;

                                    while (lrmax--)
                                    {
                                        if ((nextstr = (const unsigned char*) Advance((const char*) nextstr, _pattern)) == 0)
                                            break;

                                        strlst[index++] = (const char*) nextstr;
                                        strlst[index] = 0;
                                    }

                                    while (index--)
                                    {
                                        if ((nextstr = (const unsigned char*) Advance((const char*) strlst[index], nextpat)) != 0)
                                            return (const char*) nextstr;
                                    }

                                    if ((nextstr = (const unsigned char*) Advance((const char*) currstr, nextpat)) == 0)
                                        continue;

                                    return (const char*) nextstr;
								}

						} while((_pattern = GetPatternItem(_pattern, MREGEXPR_VRNT, false)) != 0);
					}
                    return 0;

                case (MREGEXPR_CHAR | MREGEXPR_RNGE):
                    rmin = (short) *_pattern++;
                    rmax = (short) *_pattern++;
                    chr = *_pattern++;

                    while (rmin--)
                        if (*substr++ != chr)
							return 0;

                    currstr = substr;

                    while (rmax--)
                        if (*substr++ != chr)
			                break;

                    break;

                case (MREGEXPR_CDIG | MREGEXPR_RNGE):
                    rmin = (short) *_pattern++;
                    rmax = (short) *_pattern++;

                    while (rmin--)
                        if ((chr = *substr++) < '0' || chr > '9')
							return 0;

                    currstr = substr;

                    while (rmax--)
                        if ((chr = *substr++) < '0' || chr > '9')
							break;

                    break;

                case (MREGEXPR_CSYM | MREGEXPR_RNGE):
                    rmin = (short) *_pattern++;
                    rmax = (short) *_pattern++;

                    while (rmin--)
                        if (!StrChr((const char*) RegExprSymbols, *substr++))
							return 0;

                    currstr = substr;

                    while (rmax--)
                        if (!StrChr((const char*) RegExprSymbols, *substr++))
						    break;

                    break;

                case (MREGEXPR_CLAT | MREGEXPR_RNGE):
                    rmin = (short) *_pattern++;
                    rmax = (short) *_pattern++;

                    while (rmin--)
                    if (!MemChr((const char*) RegExprSymbols, 52, *substr++))
						return 0;

                    currstr = substr;

                    while (rmax--)
                        if (!MemChr((const char*) RegExprSymbols, 52, *substr++))
							break;

                    break;

                case (MREGEXPR_CCYR | MREGEXPR_RNGE):
                    rmin = (short) *_pattern++;
                    rmax = (short) *_pattern++;

                    while (rmin--)
                        if (!StrChr((const char*) RegExprSymbols + 52, *substr++))
							return 0;

                    currstr = substr;

                    while (rmax--)
                    if (!StrChr((const char*) RegExprSymbols + 52, *substr++))
						break;

                    break;

                case (MREGEXPR_CMRK | MREGEXPR_RNGE):
                    rmin = (short) *_pattern++;
                    rmax = (short) *_pattern++;

                    while (rmin--)
                        if (!StrChr((const char*) RegExprMarks, *substr++))
							return 0;

                    currstr = substr;

                    while(rmax--)
                        if (!StrChr((const char*) RegExprMarks, *substr++))
						    break;

                    break;

                case (MREGEXPR_CHEX | MREGEXPR_RNGE):
                    rmin = (short) *_pattern++;
                    rmax = (short) *_pattern++;

                    while (rmin--)
                        if (!StrChr((const char*) RegExprHexs, *substr++))
							return 0;

                    currstr = substr;

                    while (rmax--)
                    if (!StrChr((const char*) RegExprHexs, *substr++))
						break;

                    break;

                case (MREGEXPR_CASC | MREGEXPR_RNGE):
                    rmin = (short) *_pattern++;
                    rmax = (short) *_pattern++;

                    while (rmin--)
                        if ((chr = *substr++) != '_' && (chr < '0' || chr > '9') &&
                            !StrChr((const char*) RegExprSymbols, chr))
							return 0;

                    currstr = substr;

                    while (rmax--)
                        if((chr = *substr++) != '_' && (chr < '0' || chr > '9') &&
                           !StrChr((const char*) RegExprSymbols, chr))
						   break;

                    break;

                case (MREGEXPR_CSPC | MREGEXPR_RNGE):
                    rmin = (short) *_pattern++;
                    rmax = (short) *_pattern++;

                    while (rmin--)
                        if ((chr = *substr++) != ' ' && chr != '\r' && chr != '\n' &&
                            chr != '\t' && chr != '\f' && chr != '\b')
							return 0;

                    currstr = substr;

                    while (rmax--)
                        if ((chr = *substr++) != ' ' && chr != '\r' && chr != '\n' &&
                            chr != '\t' && chr != '\f' && chr != '\b')
							break;

                    break;

                case (MREGEXPR_CEOL | MREGEXPR_RNGE):
                    rmin = (short) *_pattern++;
                    rmax = (short) *_pattern++;

                    while (rmin--)
                    if ((chr = *substr++) != '\n')
						return 0;

                    currstr = substr;

                    while (rmax--)
                    if ((chr = *substr++) != '\n')
						break;

                    break;

                case (MREGEXPR_CDOT | MREGEXPR_RNGE):
                    rmin = (short) *_pattern++;
                    rmax = (short) *_pattern++;

                    while (rmin--)
                    if (*substr++ == 0)
						return 0;

                    currstr = substr;

                    while (rmax--)
                        if (*substr++ == 0)
							break;

                    break;

                case (MREGEXPR_CCLS | MREGEXPR_RNGE):
                    rmin = (short) *_pattern++;
                    rmax = (short) *_pattern++;

                    while (rmin--)
                    {
                        chr = *substr++;
                        if (!(_pattern[(chr >> 3)] & (0x80 >> (chr & 0x07))))
							return 0;
					}

                    currstr = substr;

                    while (rmax--)
					{
                        chr = *substr++;
                        if (!(_pattern[(chr >> 3)] & (0x80 >> (chr & 0x07))))
							break;
					}

                    _pattern += 32;
                    break;
	    }

        if(currstr)
        {
            if (rmax < 0)
				substr++;
            do
			{
                const char* resstr = Advance((const char*) --substr, _pattern);
                if (resstr)
					return resstr;

			} while(substr > currstr);
		}
        return 0;
  }
  return (const char*) substr;
}

//.................................................................................................
// Загрузка и компиляция регулярного выражения.

bool __stdcall MRegExpr::Compile(const char* _expression)
{
	if (FTemplate)
	{
		free(FTemplate);
		FTemplate = 0;
	}

	bool result = false;
	bool brkglob = false;   
	long brkcount = 0;

	unsigned char* buffer = GetBuffer(_expression);
	unsigned char* ofspat = buffer;
	const unsigned char* curr = (const unsigned char*) _expression;

	if (buffer && curr[0] != '*' && curr[0] != '+' && curr[0] != '?' && curr[0] != '{' && curr[0] != '|' && curr[0] != '}')
	{
		unsigned char* startpat;
		unsigned char chr;

		while (curr && (chr = *curr++) != 0)
		{
			startpat = ofspat;

			switch (chr)
			{
			    case '.':
				{
				     *ofspat++ = MREGEXPR_CDOT;
				} break;

				case '|':
				{
					if ((startpat = GetPatternItem(buffer, 0, true)) == 0 || *startpat == MREGEXPR_VRNT || *startpat == MREGEXPR_SBRK)
						curr = 0;
						
                    else
                    {
                        *ofspat++ = MREGEXPR_VRNT;
                        if (!brkcount)
							brkglob = true;	
                        continue;
					}

				} break;

                case '(':
                {
                    if (!*curr)
						curr = 0;
                    else
					{
                        *ofspat++ = MREGEXPR_SBRK;
                        brkcount++;
                        continue;
					}

				} break;

                case ')':
				{
                    if (brkcount <= 0 || (startpat = GetPatternItem(buffer, 0, true)) == 0 ||
                        *startpat == MREGEXPR_VRNT || *startpat == MREGEXPR_SBRK)
						curr = 0;
                    else
					{
                        unsigned char* cpypos = buffer;

                        for (long index = 0; index < brkcount; index++)
						{
                            if ((startpat = GetPatternItem(cpypos, MREGEXPR_SBRK, false)) == 0)
								break;
                            else
								cpypos = startpat + 1;

                            if (*startpat & MREGEXPR_RNGE)
								cpypos += 2;
						}

                        if (!startpat)
							curr = 0;
                        else
						{
                            if (*curr != '?' && *curr != '+' && *curr != '*' && *curr != '{' &&
                                !GetPatternGroup(startpat + 1, false))
							{
                                cpypos = startpat;
                                while (ofspat >= cpypos)
									*cpypos = *cpypos++;
							}
                            else
								*ofspat++ = MREGEXPR_EBRK;

                            brkcount--;
                            FTemplate = 0;
						}
					}
				} break;

                case '[':
                {
                    *ofspat++ = MREGEXPR_CCLS;

                    unsigned char lastchr = 0;
                    bool range = false;
					bool negative = (*curr == '^' ? true : false);

                    if (negative)
						curr++;

                    while ((chr = *curr++) != 0)
					{
                        if (chr == '\\')
						{
                            if (*curr == ']' || *curr == '\\' || *curr == '^' || *curr == '-')
								chr = *curr++;
                            else if (*curr == 'x')
							{
                                if(*(curr + 1) == 0 || !StrChr((const char*) RegExprHexs, *(curr + 1)) ||
                                   *(curr + 2) == 0 || !StrChr((const char*) RegExprHexs, *(curr + 2)))
								{ 
									curr = 0;
								    break;
								}

                                unsigned char schr;

                                if ((schr = *(curr + 1)) >= ((unsigned char) '0') && schr <= ((unsigned char) '9'))
									chr = (unsigned char) ((schr - ((unsigned char) '0')) << 4);
                                else if (schr >= ((unsigned char) 'A') && schr <= ((unsigned char) 'F'))
									chr = (unsigned char) ((schr - ((unsigned char) 'A') + 10) << 4);
                                else
									chr = (unsigned char) ((schr - ((unsigned char) 'a') + 10) << 4);

                                if ((schr = *(curr + 2)) >= ((unsigned char) '0') && schr <= ((unsigned char) '9'))
									chr |= (unsigned char) (schr - ((unsigned char) '0'));
                                else if (schr >= ((unsigned char) 'A') && schr <= ((unsigned char) 'F'))
									chr |= (unsigned char) (schr - ((unsigned char) 'A') + 10);
                                else
									chr |= (unsigned char) (schr - ((unsigned char) 'a') + 10);

                                curr += 3;
							}
                            else if (!range && (*curr == 'd' || *curr == 's' || *curr == 'a' || *curr == 'c' || *curr == 'w' || *curr == 't' || *curr == 'm' || *curr == '$' || *curr == 'h'))
							{
                                if (*curr == 'd' || *curr == 'w')
								{
                                    for (chr = '0'; chr <= '9'; chr++)
                                        ofspat[(chr >> 3)] |= (unsigned char) (0x80 >> (chr & 0x07));
								}

                                if (*curr == 's' || *curr == 'w')
								{
                                    for (long index = 0; (chr = RegExprSymbols[index]) != 0; index++)
                                        ofspat[(chr >> 3)] |= (unsigned char) (0x80 >> (chr & 0x07));
								}
                                else if (*curr == 'a')
								{
                                    for (long index = 0; (chr = RegExprSymbols[index]) != 0 && index < 52; index++)
                                        ofspat[(chr >> 3)] |= (unsigned char) (0x80 >> (chr & 0x07));
								}
                                else if (*curr == 'c')
								{
                                    for (long index = 52; (chr = RegExprSymbols[index]) != 0; index++)
                                        ofspat[(chr >> 3)] |= (unsigned char) (0x80 >> (chr & 0x07));
								}
                                else if (*curr == 'm')
								{
                                    for (long index = 0; (chr = RegExprMarks[index]) != 0; index++)
                                        ofspat[(chr >> 3)] |= (unsigned char) (0x80 >> (chr & 0x07));
								}
                                else if (*curr == 'h')
								{
                                    for (long index = 0; (chr = RegExprHexs[index]) != 0; index++)
                                        ofspat[(chr >> 3)] |= (unsigned char) (0x80 >> (chr & 0x07));
								}

                                if (*curr == 't')
								{
                                    ofspat[(' ' >> 3)] |= (unsigned char) (0x80 >> (' ' & 0x07));
                                    ofspat[('\r' >> 3)] |= (unsigned char) (0x80 >> ('\r' & 0x07));
                                    ofspat[('\n' >> 3)] |= (unsigned char) (0x80 >> ('\n' & 0x07));
                                    ofspat[('\t' >> 3)] |= (unsigned char) (0x80 >> ('\t' & 0x07));
                                    ofspat[('\f' >> 3)] |= (unsigned char) (0x80 >> ('\f' & 0x07));
                                    ofspat[('\b' >> 3)] |= (unsigned char) (0x80 >> ('\b' & 0x07));
								}

                                if (*curr == '$')
                                    ofspat[('\n' >> 3)] |= (unsigned char) (0x80 >> ('\n' & 0x07));

                                if (*curr++ == 'w')
                                    ofspat[('_' >> 3)] |= (unsigned char) (0x80 >> ('_' & 0x07));

                                lastchr = 0;
                                continue;
							}
                            else
							{ 
								curr = 0;
								break;
							}
						}
                        else if (chr == '-')
						{
                            if (!lastchr || range)
							{
								curr = 0;
								break;
							}
                            range = true;
                            continue;
						}
                        else if (chr == ']')
						{
                            if (range)
								curr = 0;
                            break;
						}

                        ofspat[(chr >> 3)] |= (unsigned char) (0x80 >> (chr & 0x07));

                        if (range)
						{
                            chr--;
							lastchr++;

                            while (chr >= lastchr)
							{
                                ofspat[(chr >> 3)] |= (unsigned char) (0x80 >> (chr & 0x07));
                                chr--;
							}

                            lastchr = 0;
                            range = false;
						}
                        else
							lastchr = chr;
		            }
                    if (chr != ']')
			            curr = 0;
                    else
					{
                        if (negative)
						{
                            for (long index = 0; index < 32; index++)
                                ofspat[index] ^= (~0x00);
						}
                        ofspat += 32;
					}

		         } break;

                 case '\\':
			     {
                     if (*curr == '[' || *curr == '\\' || *curr == '?' || *curr == '+' ||
                         *curr == '*' || *curr == '{' || *curr == '.' || *curr == '|' ||
                         *curr == '(' || *curr == ')')
						 chr = *curr++;

                     else if (*curr == 'd' || *curr == 's' || *curr == 'a' || *curr == 'c' || *curr == 'w' || *curr == 't' || *curr == 'm' || *curr == '$' || *curr == 'h')
					 {
                         *ofspat++ = (*curr == 'd' ? MREGEXPR_CDIG : (*curr == 's' ? MREGEXPR_CSYM : (*curr == 'a' ? MREGEXPR_CLAT :
                                     (*curr == 'c' ? MREGEXPR_CCYR : (*curr == 't' ? MREGEXPR_CSPC : (*curr == 'm' ? MREGEXPR_CMRK :
                                     (*curr == '$' ? MREGEXPR_CEOL : (*curr == 'h' ? MREGEXPR_CHEX : MREGEXPR_CASC))))))));

                         curr++; break;
					 }
                     else if (*curr == 'x')
					 {
                         if (*(curr + 1) == 0 || !StrChr((const char*) RegExprHexs, *(curr + 1)) ||
                             *(curr + 2) == 0 || !StrChr((const char*) RegExprHexs, *(curr + 2))) 
						 {
							 curr = 0;
							 break;
						 }

                         unsigned char schr;

                         if ((schr = *(curr + 1)) >= ((unsigned char) '0') && schr <= ((unsigned char) '9'))
							 chr = (unsigned char) ((schr - ((unsigned char) '0')) << 4);
                         else if (schr >= ((unsigned char) 'A') && schr <= ((unsigned char) 'F'))
							 chr = (unsigned char) ((schr - ((unsigned char) 'A') + 10) << 4);
                         else
							 chr = (unsigned char) ((schr - ((unsigned char) 'a') + 10) << 4);

                         if ((schr = *(curr + 2)) >= ((unsigned char) '0') && schr <= ((unsigned char) '9'))
							 chr |= (unsigned char) (schr - ((unsigned char) '0'));
                         else if (schr >= ((unsigned char) 'A') && schr <= ((unsigned char) 'F'))
							 chr |= (unsigned char) (schr - ((unsigned char) 'A') + 10);
                         else
							 chr |= (unsigned char) (schr - ((unsigned char) 'a') + 10);

                         curr += 3;
					 }
				 }

                 default:
			     {
                     *ofspat++ = MREGEXPR_CHAR;
                     *ofspat++ = chr;
				 }
	        }

            if (!curr)
				break;
            else if (!*curr)
			{
                result = true;
                break;
			}

            if (*curr == '?' || *curr == '+' || *curr == '*' || *curr == '{')
			{
                long rmin = 0;
				long rmax = 255;

                if ((chr = *curr++) == '{')
				{
                    bool isdlm = false;
                    long* count = &rmin;
                    rmin = -1;
					rmax = -1;

                    while ((chr = *curr++) != 0)
					{
                        if (chr == '}')
						{
                            if (rmin < 0)
							{
                                if (isdlm)
									rmin = 0;
                                else
									curr = 0;
							}

                            if (rmax < 0)
								rmax = (isdlm ? 255 : rmin);

                            break;
						}
                        else if (chr >= '0' && chr <= '9')
						{
                            if (*count == -1)
								*count = (chr - '0');
                            else 
								*count = (*count * 10) + (chr - '0');

                            if (*count > 255)
							{ 
								curr = 0;
								break;
							}
						}
                        else if (chr == ',' && count == &rmin)
						{
                            isdlm = true;
                            count = &rmax;
						}
                        else
						{ 
							curr = 0;
							break;
						}
					}
				}
                else if (chr == '+')
					rmin = 1;
                else if (chr == '?')
					rmax = 1;

                if (rmin > rmax)
					curr = 0;
                if (!curr)
					break;

                unsigned char* cpypos = ofspat;
                while (startpat < cpypos--)
					cpypos[2] = cpypos[0];

                startpat[0] |= MREGEXPR_RNGE;
                startpat[1] = (unsigned char) rmin;
                startpat[2] = (unsigned char) (rmax - rmin);
                ofspat += 2;
			}
	     }

         if (curr && !chr)
			 result = true;
     }

     if (result)
	 {
         if (brkglob)
		 {
             *ofspat++ = MREGEXPR_EBRK;
             while (buffer < ofspat--)
				 ofspat[1] = ofspat[0];
             *buffer = MREGEXPR_SBRK;
		 }
         FTemplate = buffer;
	 }
     else if (buffer)
		 free(buffer);

     return result;
}

//.................................................................................................
// Получение признака загрузки регулярного выражения.

bool __stdcall MRegExpr::IsLoaded(void) const
{
	return (!FTemplate ? false : true);
}

//.................................................................................................
// Проверка строки на соответствие регулярному выражению.

bool __stdcall MRegExpr::IsEqual(const char* _string)
{
	if (!_string)
		return false;
	const char* endpos = Advance(_string, FTemplate);

	if (!endpos)
		return false;
	return ((endpos - _string) == strlen(_string) ? true : false);
}

//.................................................................................................
// Поиск подстроки, соответствующей регулярному выражению.

const char* __stdcall MRegExpr::Find(const char* _string, long& _length)
{
	_length = 0;

	if (!FTemplate || !_string || !_string[0])
		return 0;

	unsigned char* itempat = FTemplate;
	const char* endpos;

	if (itempat[0] == MREGEXPR_CHAR)
	{
		do
		{
			if (*((const unsigned char*) _string) != itempat[1])
				continue;

			if ((endpos = Advance(_string, itempat)) != 0)
			{
				_length = endpos - _string;

				if (_length > 0)
					return _string;
			}
		} while (*_string++);

		return 0;
	}

	do
	{
		if ((endpos = Advance(_string, itempat)) != 0)
		{
			_length = endpos - _string;

			if (_length > 0)
				return _string;
		}
	} while (*_string++);

	return 0;	
}

//*************************************************************************************************

int main (int argc, char* argv[])
{	 
	if(argc == 1)
	{
	    puts("Usage: mregexpr <regular expression> <path file name>");
	    return 0;
	}

	if (argc != 3)
    {
        puts("Command line error.");
	    puts("Usage: mregexpr <regular expression> <path file name>");
	    return -1;
	}  

    MRegExpr regexpr;

    if (!regexpr.Compile(argv[1]))
    {
        printf("Incorrect regular expression: %s\n", argv[1]);
	    return -1;
	}
  
    WIN32_FIND_DATAA fdi;   
    HANDLE hsearch = ::FindFirstFile(argv[2], &fdi); 

    if (hsearch != INVALID_HANDLE_VALUE)  
	{															  
		char path[MAX_PATH + 1] = "";	              
        char* strptrl = strrchr(argv[2], '\\');
        char* strptrr = strrchr(argv[2], '/');
        char* strptrd = strrchr(argv[2], ':');
    
        if (strptrr > strptrl)
			strptrl = strptrr;
        if (strptrd > strptrl)
			strptrl = strptrd;
        if (strptrr > 0)
			strptrl = strptrr;

        if (strptrl >= argv[2])
            strncat(path, argv[2], strptrl - argv[2] + 1);

        printf("\nRegular expression : %s\n", argv[1]);
        printf("Path file name     : %s\n", argv[2]);
        printf("\nFound data in files:\n");

        do
        {
            if ((fdi.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0 && fdi.nFileSizeLow > 0)
			{
                char filepath[MAX_PATH + 1];
                strcpy(filepath, path);
                strcat(filepath, fdi.cFileName);

                HANDLE hfile = ::CreateFile(filepath, GENERIC_READ, (FILE_SHARE_READ | FILE_SHARE_DELETE), 0, OPEN_EXISTING, 0, 0);

                if (hfile	 != INVALID_HANDLE_VALUE)
				{
                    char* buffer = (char*) malloc(fdi.nFileSizeLow + 1);
          
                    if (buffer)
					{
                        memset(buffer, 0, fdi.nFileSizeLow + 1);

                        if (::ReadFile(hfile, buffer, fdi.nFileSizeLow, &fdi.nFileSizeLow, 0))	 
						{               
                            long length = 0;
                            char* findptr = (char*) regexpr.Find(buffer, length);	  
              
                            if (findptr)
                            {
                                printf("\n%s\n", filepath);

                                do
								{
                                    char savech = findptr[length];
                                    findptr[length] = 0;
                                    printf("Position[%d], Length[%d], Text[%s]\n", (findptr - buffer), length, findptr);
                                    findptr[length] = savech;
                                    findptr += length;                  

								} while((findptr = (char*) regexpr.Find(findptr, length)) != 0);
							}
						}

                        free(buffer);
					}
                    else
					{
                        printf("Not memory for load file: %s\n", filepath);
					}

                    ::CloseHandle(hfile);
				}        
			}

		} while(::FindNextFile(hsearch, &fdi));

        ::FindClose(hsearch);
	}
    else
	{
        printf("Required files not found for path: %s\n", argv[1]);
	    return -1;
	}

    return 0;
}