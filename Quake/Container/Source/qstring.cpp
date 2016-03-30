#include "..\Include\qstring.h"

QBool isSensitive(Sensitivity sensitivity)
{
	return sensitivity == STR_SENSITIVE;
}

QBool isInsensitive(Sensitivity sensitivity)
{
	return sensitivity == STR_INSENSITIVE;
}

QString::Comparison::Comparison(size_t trav, QBool equal)
	: traversed(trav), isEqual(equal)
{
}

QString::QString(size_t length)
	:  _string(nullptr), _maxLength(), _length()
{
	//_string = _allocator.allocate(sizeof(Character) * length);
	if (length != 0)
	{
		size_t zeroLen = length + 1;
		_maxLength = zeroLen;
		_length = 1;
		_string = new Character[zeroLen];
		_string[0] = '\0';
	}
}

QString::QString(const Character* string)
	:  _string(nullptr), _maxLength(), _length()
{
	if (string)
	{
		size_t otherLen = strlen(string);
		QBool hasTZero = *(string + otherLen - 1) == '\0';
		size_t len = hasTZero ? otherLen : otherLen + 1;
		_maxLength = len;
		_length = len;
		_string = new Character[len];
		strncpy_s(_string, len, string, otherLen);
		if (!hasTZero)
		{
			_string[len - 1] = '\0';
		}
	}
}

QString::QString(const Character* begin, const Character* end)
	: _string(nullptr), _maxLength(), _length()
{
	if (_comesBefore(begin, end))
	{
		QBool hasTZero = *end == '\0';
		size_t len = hasTZero ? end - begin : end - begin + 1;
		_string = new Character[len];
		_length = len;
		_maxLength = len;
		strncpy_s(_string, len, begin, len - 1);
		if (!hasTZero)
		{
			_string[len - 1] = '\0';
		}
	}
}

QString::QString(const Iterator begin, const Iterator end)
	: _string(nullptr), _maxLength(), _length()
{
	if (_comesBefore(begin, end))
	{
		const Character* string = begin.ptr();
		size_t otherLen = strlen(string);
		QBool hasTZero = *end == '\0';
		size_t len = hasTZero ? otherLen : otherLen + 1;
		_maxLength = len;
		_length = len;
		_string = new Character[len];
		strncpy_s(_string, len, string, otherLen);
		if (!hasTZero)
		{
			_string[len - 1] = '\0';
		}
	}
}

QString::QString(const QString& string)
	: _string(nullptr), _maxLength(string._maxLength), _length(string._length)
{
	if (string)
	{
		_string = new char[_length];
		size_t len = string._length;
		const Character* str = string._string;
		strncpy_s(_string, _length, str, len);
	}
}

QString::~QString()
{
	if (_string)
	{
		delete[] _string;
	}
}

void QString::pushFront(Character ch)
{
	if (!_hasSpaceFor(1))
	{
		reserve(_maxLength + 3);
	}
	Character last = _string[0];
	_string[0] = ch;
	size_t end = _length + 1;
	for (size_t i = 1; i < end; ++i)
	{
		Character hold = _string[i];
		_string[i] = last;
		last = hold;
	}
	_incrementLength();
}

void QString::pushFront(const Character* string)
{
	size_t len = strlen(string);
	if (!_hasSpaceFor(len))
	{
		reserve(_maxLength + len);
	}
	Character last = _string[0];
	_string[0] = string[0];
	size_t end = _length + len;
	for (size_t i = 1; i < end; ++i)
	{
		//Character hold = _string[i];
		//Character hold2 = _string[i + 1];
		//_string[i] = last;
		//last = hold;
		///*if (i < len)
		//{
		//	_string[i] = string[i];
		//}*/
	}
}

void QString::pushFront(const QString& string)
{
}

void QString::setFront(Character ch)
{
	if (_length > 1)
	{
		_string[0] = ch;
	}
}

QString::Reference QString::getFront() const
{
	return _string[0];
}

void QString::pushBack(Character ch)
{
	if (!_hasSpaceFor(1))
	{
		reserve(_maxLength + 2);
	}
	_string[_length - 1] = ch;
	_string[_length] = '\0';
	_incrementLength();
}

void QString::pushBack(const Character* string)
{
	size_t len = strlen(string);
	if (!_hasSpaceFor(len))
	{
		reserve(_maxLength + len);
	}
	for (size_t i = _length - 1, j = 0; i < _maxLength && j < len; ++i, ++j)
	{
		Character ch = string[j];
		_string[i] = ch;
	}
	_addLength(len);
	if (string[len] == '\0')
	{
		_setTerminatingZero(_length - 1);
	}
}

void QString::pushBack(const QString& string)
{
	size_t len = string._length - 1;
	if (!_hasSpaceFor(len))
	{
		reserve(_maxLength + len);
	}
	for (size_t i = _length - 1, j = 0; i < _maxLength && j < len; ++i, ++j)
	{
		Character ch = string[j];
		_string[i] = ch;
	}
	_addLength(len);
	if (string[len] == '\0')
	{
		_setTerminatingZero(_length - 1);
	}
}

void QString::setBack(Character ch)
{
	if (_length - 2 >= 0)
	{
		_string[_length - 2] = ch;
	}
}

QString::Reference QString::getBack() const
{
	return _string[_length - 2];
}

void QString::concat(const Character* string)
{
	size_t len = strlen(string);
	if (!_hasSpaceFor(len))
	{
		reserve(_maxLength + len);
	}
	strncat_s(_string, _maxLength, string, len);
	_addLength(len);
}

void QString::concat(const QString& string)
{
	size_t len = string._length - 1;
	if (!_hasSpaceFor(len))
	{
		reserve(_maxLength + len);
	}
	strncat_s(_string, _maxLength, string._string, len);
	_addLength(len);
}

//void QString::insert(size_t index, Character ch)
//{
//	if (_checkIndex(index))
//	{
//		if (!_hasSpaceFor(1))
//		{
//			reserve(_maxLength + 5);
//		}
//		for (size_t i = index; i < _length; ++i)
//		{
//			Character hold = _string[i];
//			Character next = _string[i + 1];
//			_string[i + 1] = hold;
//		}
//		_string[index] = ch;
//		_incrementLength();
//		_adjustEnd(1);
//	}
//}
//
//void QString::insert(size_t begin, size_t end, const Character* string)
//{
//	if (_checkIndicies(begin, end) || _checkIndex(begin) && _comesBefore(begin, end))
//	{
//		size_t spaceNeeded = end - begin;
//		if (!_hasSpaceFor(spaceNeeded))
//		{
//			reserve(spaceNeeded);
//		}
//		size_t trueEnd = end + 1;
//		Character hold = _string[0];
//		for (size_t i = begin, j = 0; i < trueEnd; ++i, ++j)
//		{
//			Character hold = _string[i];
//			Character next = _string[i + 1];
//			_string[i + 1] = hold;
//			hold = next;
//
//			Character ch = string[j];
//			_string[i] = ch;
//		}
//		size_t newLen = _length + spaceNeeded;
//		_setLength(newLen);
//		_adjustEnd(newLen);
//	}
//}
//
//void QString::insert(size_t begin, size_t end, const QString& string)
//{
//	//if (_checkNegativeIndicies(begin, end) || _checkIndex(begin) && _comesBefore(begin, end))
//	//{
//	//	size_t spaceNeeded = end - begin;
//	//	if (!_hasSpaceFor(spaceNeeded))
//	//	{
//	//		reserve(_maxLength + spaceNeeded);
//	//	}
//	//	size_t trueEnd = end + 1;
//	//	for (size_t i = begin, j = 0; i < trueEnd; ++i, ++j)
//	//	{
//	//		Character hold = _string[i];
//	//		Character next = _string[i + 1];
//	//		_string[i + 1] = hold;
//
//	//		Character ch = string[j];
//	//		_string[i] = ch;
//	//	}
//	//	_setLength(_length + spaceNeeded);
//	//	//_adjustEnd(spaceNeeded);
//	//}
//}
//
//void QString::insert(Iterator iterator, Character ch)
//{
//	/*if (_checkIterator(iterator))
//	{
//		if (!_hasSpaceFor(1))
//		{
//			reserve(_maxLength + 5);
//		}
//		for (Iterator i = _begin; i < _end; ++i)
//		{
//			Iterator hold = i;
//			Iterator next = i.next();
//			i.next() = hold;
//		}
//		iterator.set(ch);
//		_incrementLength();
//		_adjustEnd(1);
//	}*/
//}

void QString::set(size_t index, Character ch)
{
	if (_checkIndex(index))
	{
		_string[index] = ch;
	}
}

void QString::set(size_t begin, size_t end, const Character* string)
{
	if (_checkIndicies(begin, end))
	{
		size_t distance = end - begin;
		size_t len = strlen(string);
		size_t buffer = distance > len ? len : distance;
		for (size_t i = begin, j = 0; i < end && j < buffer; ++i, ++j)
		{
			Character ch = string[j];
			_string[i] = ch;
		}
	}
}

void QString::set(size_t begin, size_t end, const QString& string)
{
	if (_checkIndicies(begin, end))
	{
		size_t distance = end - begin;
		size_t len = string._length - 1;
		size_t buffer = distance > len ? len : distance;
		for (size_t i = begin, j = 0; i < end && j < buffer; ++i, ++j)
		{
			Character ch = string[j];
			_string[i] = ch;
		}
	}
}

void QString::set(Iterator iterator, Character ch)
{
	if (_checkIterator(iterator))
	{
		iterator.set(ch);
	}
}

void QString::set(Iterator begin, Iterator end, const Character* string)
{
	if (_checkIterators(begin, end))
	{
		size_t distance = iterator_distance(end, begin);
		size_t len = strlen(string);
		size_t buffer = distance > len ? len : distance;
		size_t j = 0;
		for (Iterator i = begin; i < end; ++i, ++j)
		{
			Character ch = string[j];
			i.set(ch);
		}
	}
}

void QString::set(Iterator begin, Iterator end, const QString& string)
{
	if (_checkIterators(begin, end))
	{
		size_t distance = iterator_distance(end, begin);
		size_t len = string._length - 1;
		size_t buffer = distance > len ? len : distance;
		size_t j = 0;
		for (Iterator i = begin; i < end; ++i, ++j)
		{
			Character ch = string[j];
			i.set(ch);
		}
	}
}

QString QString::substring(size_t index) const
{
	if (_checkIndex(index))
	{
		size_t len = _length - index - 1;
		Character* start = _string + index;
		return QString(start);
	}
	return EMPTY_STRING;
}

QString QString::substring(size_t begin, size_t end) const
{
	if (_checkIndicies(begin, end))
	{
		size_t distance = end - begin;
		Character* start = _string + begin;
		Character* stop = _string + begin + distance - 1;
		return QString(start, stop);
	}
	return EMPTY_STRING;
}

QString QString::substring(const Iterator iterator) const
{
	if (_checkIterator(iterator))
	{
		return QString(iterator.ptr());
	}
	return EMPTY_STRING;
}

QString QString::substring(const Iterator begin, const Iterator end) const
{
	if (_checkIterators(begin, end))
	{
		return QString(begin, end);
	}
}

QString::Iterator QString::find(Character ch, size_t buffer, Sensitivity sensitivity) const
{
	CharChecker charCheck = _getCharChecker(sensitivity);
	Iterator begin = getBegin();
	Iterator end = getEnd();
	for (Iterator i = begin; i < end; ++i)
	{
		if (charCheck(ch, i.get()) && !--buffer)
		{
			return i;
		}
	}
	return end;
}

QString::Iterator QString::find(const Character* string, size_t buffer, Sensitivity sensitivity) const
{
	size_t len = strlen(string);
	CharChecker charCheck = _getCharChecker(sensitivity);
	StrChecker strCheck = _getStrChecker(sensitivity);
	Iterator begin = getBegin();
	Iterator end = getEnd();
	for (Iterator i = begin; i < end; ++i)
	{
		if (charCheck(string[0], i.get()))
		{
			size_t distance = iterator_distance(i, begin);
			Character* start = _string + distance;
			if (!strCheck(start, string, len) && !--buffer)
			{
				return i;
			}
			else
			{
				i += len;
			}
		}
	}
	return end;
}

QString::Iterator QString::find(const QString& string, size_t buffer, Sensitivity sensitivity) const
{
	size_t len = string._length - 1;
	CharChecker charCheck = _getCharChecker(sensitivity);
	StrChecker strCheck = _getStrChecker(sensitivity);
	Iterator begin = getBegin();
	Iterator end = getEnd();
	for (Iterator i = begin; i < end; ++i)
	{
		if (charCheck(string[0], i.get()))
		{
			size_t distance = iterator_distance(i, begin);
			Character* start = _string + distance;
			if (!strCheck(start, string._string, len) && !--buffer)
			{
				return i;
			}
			else
			{
				i += len;
			}
		}
	}
	return end;
}

QString::ReverseIterator QString::rfind(Character ch, Sensitivity sensitivity) const
{
	CharChecker charCheck = _getCharChecker(sensitivity);
	ReverseIterator begin = getRBegin();
	ReverseIterator end = getREnd();
	for (ReverseIterator i = begin; i < end; ++i)
	{
		if (charCheck(ch, i.get()))
		{
			return i;
		}
	}
	return end;
}

QString::ReverseIterator QString::rfind(const Character* string, Sensitivity sensitivity) const
{
	size_t len = strlen(string);
	CharChecker charCheck = _getCharChecker(sensitivity);
	StrChecker strCheck = _getStrChecker(sensitivity);
	ReverseIterator begin = getRBegin();
	ReverseIterator end = getREnd();
	for (ReverseIterator i = begin; i < end; ++i)
	{
		size_t distance = iterator_distance(i, begin);
		String start = _string + (_length - distance - 1);

		/*if (!strCheck(start, string, len))
		{
			return i;
		}
		else
		{
			i += len;
		}*/
	}
	return end;
}

QString::ReverseIterator QString::rfind(const QString& string, Sensitivity sensitivity) const
{
	/*size_t len = string.getLength();
	CharChecker charCheck = _getCharChecker(sensitivity);
	ReverseIterator begin = _rbegin;
	ReverseIterator end = _rend;
	ReverseIterator found = end;
	for (ReverseIterator i = begin; i < end; ++i)
	{
		if (charCheck(string[len - 1], i.get()))
		{
			found = i;
			++i;
			for (size_t j = len - 2; charCheck(string[j], i.get()); --j, ++i)
			{
				if (j == -1)
				{
					return found;
				}
			}
		}
	}
	return end;*/
	return ReverseIterator();
}

QString::Iterator QString::findLast(Character ch, Sensitivity sensitivity) const
{
	CharChecker charCheck = _getCharChecker(sensitivity);
	Iterator begin = getBegin();
	Iterator end = getEnd();
	Iterator found = end;
	for (Iterator i = begin; i < end; ++i)
	{
		if (charCheck(ch, i.get()))
		{
			found = i;
		}
	}
	return found;
}

QString::Iterator QString::findLast(const Character* string, Sensitivity sensitivity) const
{
	size_t len = strlen(string);
	CharChecker charCheck = _getCharChecker(sensitivity);
	Iterator begin = getBegin();
	Iterator end = getEnd();
	Iterator found = end;
	Iterator i = begin;
	size_t count = 0;
	while (i < end)
	{
		if (charCheck(string[0], i.get()))
		{
			size_t distance = iterator_distance(i, begin);
			Character* start = _string + distance;
			Iterator possible = i;
			Comparison comparison = _compare(start, string, len, charCheck);
			size_t traversed = comparison.traversed;
			i = iterator_traverse(i, traversed);
			count += traversed;
			if (comparison.isEqual)
			{
				found = possible;
				int32 indicesLeft = _length - count - 1;
				if (indicesLeft < len)
				{
					return found;
				}
			}
		}
		else
		{
			++i;
			++count;
		}
	}
	return found;
}

QString::Iterator QString::findLast(const QString& string, Sensitivity sensitivity) const
{
	size_t len = string._length - 1;
	CharChecker charCheck = _getCharChecker(sensitivity);
	Iterator begin = getBegin();
	Iterator end = getEnd();
	Iterator found = end;
	Iterator i = begin;
	size_t count = 0;
	while (i < end)
	{
		if (charCheck(string[0], i.get()))
		{
			size_t distance = iterator_distance(i, begin);
			Character* start = _string + distance;
			Character* other = string._string;
			Iterator possible = i;
			Comparison comparison = _compare(start, other, len, charCheck);
			size_t traversed = comparison.traversed;
			i = iterator_traverse(i, traversed);
			count += traversed;
			if (comparison.isEqual)
			{
				found = possible;
				int32 indicesLeft = _length - count - 1;
				if (indicesLeft < len)
				{
					return found;
				}
			}
		}
		else
		{
			++i;
			++count;
		}
	}
	return found;
}

QBool QString::has(Character ch, Sensitivity sensitivity) const
{
	CharChecker charCheck = _getCharChecker(sensitivity);
	Iterator begin = getBegin();
	Iterator end = getEnd();
	for (Iterator i = begin; i < end; ++i)
	{
		if (charCheck(ch, i.get()))
		{
			return true;
		}
	}
	return false;
}

QBool QString::has(const Character* string, Sensitivity sensitivity) const
{
	size_t len = strlen(string);
	CharChecker charCheck = _getCharChecker(sensitivity);
	Iterator begin = getBegin();
	Iterator end = getEnd();
	Iterator i = begin;
	while (i < end)
	{
		if (charCheck(string[0], i.get()))
		{
			size_t distance = iterator_distance(i, begin);
			Character* start = _string + distance;
			Comparison comparison = _compare(start, string, len, charCheck);
			if (comparison.isEqual)
			{
				return true;
			}
			else
			{
				i = iterator_traverse(i, comparison.traversed);
			}
		}
		else
		{
			++i;
		}
	}
	return false;
}

QBool QString::has(const QString& string, Sensitivity sensitivity) const
{
	size_t len = string._length - 1;
	CharChecker charCheck = _getCharChecker(sensitivity);
	Iterator begin = getBegin();
	Iterator end = getEnd();
	Iterator i = begin;
	while (i < end)
	{
		if (charCheck(string[0], i.get()))
		{
			size_t distance = iterator_distance(i, begin);
			Character* start = _string + distance;
			Character* other = string._string;
			Comparison comparison = _compare(start, other, len, charCheck);
			if (comparison.isEqual)
			{
				return true;
			}
			else
			{
				i = iterator_traverse(i, comparison.traversed);
			}
		}
		else
		{
			++i;
		}
	}
	return false;
}

////QBool QString::rhas(Character ch, Sensitivity sensitivity) const
////{
////	CharChecker charCheck = _getCharChecker(sensitivity);
////	ReverseIterator begin = _rbegin;
////	ReverseIterator end = _rend;
////	for (ReverseIterator i = begin; i < end; ++i)
////	{
////		if (charCheck(ch, i.get()))
////		{
////			return true;
////		}
////	}
////	return false;
////}
////
////QBool QString::rhas(const Character* string, Sensitivity sensitivity) const
////{
////	size_t len = Q_strLen(string);
////	CharChecker charCheck = _getCharChecker(sensitivity);
////	ReverseIterator begin = _rbegin;
////	ReverseIterator end = _rend;
////	for (ReverseIterator i = begin; i < end; ++i)
////	{
////		if (charCheck(string[len - 1], i.get()))
////		{
////			++i;
////			for (size_t j = len - 2; charCheck(string[j], i.get()); --j, ++i)
////			{
////				if (charCheck(string[j], i.get()) && j == 0)
////				{
////					return true;
////				}
////			}
////		}
////	}
////
////	return false;
////}
////
////QBool QString::rhas(const QString& string, Sensitivity sensitivity) const
////{
////	size_t len = string.getLength();
////	CharChecker charCheck = _getCharChecker(sensitivity);
////	ReverseIterator begin = _rbegin;
////	ReverseIterator end = _rend;
////	for (ReverseIterator i = begin; i < end; ++i)
////	{
////		if (charCheck(string[len - 1], i.get()))
////		{
////			++i;
////			for (size_t j = len - 2; charCheck(string[j], i.get()); --j, ++i)
////			{
////				if (charCheck(string[j], i.get()) && j == 0)
////				{
////					return true;
////				}
////			}
////		}
////	}
////	return false;
////}

size_t QString::occurances(Character ch, Sensitivity sensitivity) const
{
	CharChecker charCheck = _getCharChecker(sensitivity);
	Iterator begin = getBegin();
	Iterator end = getEnd();
	size_t occurances = 0;
	for (Iterator i = begin; i < end; ++i)
	{
		if (charCheck(ch, i.get()))
		{
			++occurances;
		}
	}
	return occurances;
}

size_t QString::occurances(const Character* string, Sensitivity sensitivity) const
{
	size_t len = strlen(string);
	CharChecker charCheck = _getCharChecker(sensitivity);
	Iterator begin = getBegin();
	Iterator end = getEnd();
	Iterator i = begin;
	size_t occurances = 0;
	while (i < end)
	{
		if(charCheck(string[0], i.get()))
		{
			size_t distance = iterator_distance(i, begin);
			Character* start = _string + distance;
			Comparison comparison = _compare(start, string, len, charCheck);
			i = iterator_traverse(i, comparison.traversed);
			if (comparison.isEqual)
			{
				occurances++;
			}
		}
		else
		{
			++i;
		}
	}
	return occurances;
}

size_t QString::occurances(const QString& string, Sensitivity sensitivity) const
{
	size_t len = string.getLength();
	CharChecker charCheck = _getCharChecker(sensitivity);
	Iterator begin = getBegin();
	Iterator end = getEnd();
	Iterator i = begin;
	size_t occurances = 0;
	while (i < end)
	{
		if (charCheck(string[0], i.get()))
		{
			size_t distance = iterator_distance(i, begin);
			Character* start = _string + distance;
			Character* other = string._string;
			Comparison comparison = _compare(start, other, len, charCheck);
			i = iterator_traverse(i, comparison.traversed);
			if (comparison.isEqual)
			{
				occurances++;
			}
		}
		else
		{
			++i;
		}
	}
	return occurances;
}

QBool QString::is(CharacterFilter filter, CharacterFilter exception) const
{
	Iterator begin = getBegin();
	Iterator end = getEnd();
	for (Iterator i = begin; i < end; ++i)
	{
		Character ch = i.get();
		QBool match = filter(ch);
		QBool except = exception ? exception(ch) : false;
		if (filterNotMatch(match) && exceptionNotApply(except))
		{
			return false;
		}
	}
	return true;
}

QBool QString::is(size_t index, CharacterFilter filter, CharacterFilter exception) const
{
	return _checkIndex(index) ? filter(_string[index]) || (exception && exception(_string[index])) : false;
}

QBool QString::is(size_t begin, size_t end, CharacterFilter filter, CharacterFilter exception) const
{
	if (_checkIndicies(begin, end))
	{
		size_t trueEnd = end + 1;
		for (size_t i = begin; i < trueEnd; ++i)
		{
			Character ch = _string[i];
			QBool match = filter(ch);
			QBool except = exception ? exception(ch) : false;
			if (filterNotMatch(match) && exceptionNotApply(except))
			{
				return false;
			}
		}
		return true;
	}
	return false;
}

QBool QString::is(const Iterator iterator, CharacterFilter filter, CharacterFilter exception) const
{
	return _checkIterator(iterator) ? filter(iterator.get()) || (exception && exception(iterator.get())) : false;
}

QBool QString::is(Iterator begin, Iterator end, CharacterFilter filter, CharacterFilter exception) const
{
	if (_checkIterators(begin, end))
	{
		Iterator trueEnd = end + 1;
		for (Iterator i = begin; i < trueEnd; ++i)
		{
			Character ch = i.get();
			QBool match = filter(ch);
			QBool except = exception ? exception(ch) : false;
			if (filterNotMatch(match) && exceptionNotApply(except))
			{
				return false;
			}
		}
		return true;
	}
	return false;
}

////QBool QString::ris(size_t index, CharacterFilter filter, CharacterFilter exception) const
////{
////	if (_checkNegativeIndex(index))
////	{
////		size_t pos = _negToPos(index);
////		return filter(_string[pos]) || exception(_string[pos]);
////	}
////	return false;
////}
////
////QBool QString::ris(size_t begin, size_t end, CharacterFilter filter, CharacterFilter exception) const
////{
////	if (_checkNegativeIndicies(begin, end))
////	{
////		size_t posBeg = _negToPos(begin);
////		size_t posEnd = _negToPos(end);
////		for (size_t i = posBeg; i < posEnd; ++i)
////		{
////			if (!filter(_string[i]) && !exception(_string[i]))
////			{
////				return false;
////			}
////		}
////		return true;
////	}
////	return false;
////}
//
//QBool QString::ris(ReverseIterator iterator, CharacterFilter filter, CharacterFilter exception) const
//{
//	return _checkReverseIterator(iterator) ? filter(iterator.get()) || exception(iterator.get()) : false;
//}
//
//QBool QString::ris(ReverseIterator begin, ReverseIterator end, CharacterFilter filter, CharacterFilter exception) const
//{
//	if (_checkReverseIterators(begin, end))
//	{
//		ReverseIterator trueEnd = end + 1;
//		for (ReverseIterator i = begin; i < trueEnd; ++i)
//		{
//			if (!filter(i.get()) || !exception(i.get()))
//			{
//				return false;
//			}
//		}
//		return true;
//	}
//	return false;
//}

void QString::to(CharacterConverter converter)
{
	Iterator begin = getBegin();
	Iterator end = getEnd();
	for (Iterator i = begin; i < end; ++i)
	{
		Character ch = i.get();
		ch = converter(ch);
		i.set(ch);
	}
}

void QString::to(size_t index, CharacterConverter converter)
{
	if (_checkIndex(index))
	{
		Character ch = _string[index];
		ch = converter(ch);
		_string[index] = ch;
	}
}

void QString::to(size_t begin, size_t end, CharacterConverter converter)
{
	if (_checkIndicies(begin, end))
	{
		size_t trueEnd = end + 1;
		for (size_t i = begin; i < trueEnd; ++i)
		{
			Character ch = _string[i];
			ch = converter(ch);
			_string[i] = ch;
		}
	}
}

//void QString::to(Iterator iterator, CharacterConverter converter, CharacterFilter filter)
//{
//	if (_checkIterator(iterator))
//	{
//		Character ch = iterator.get();
//		if (!filter(ch))
//		{
//			ch = converter(ch);
//			iterator.set(ch);
//		}
//	}
//}
//
//void QString::to(Iterator begin, Iterator end, CharacterConverter converter, CharacterFilter filter)
//{
//	if (_checkIterators(begin, end))
//	{
//		Iterator trueEnd = end + 1;
//		for (Iterator i = begin; i < trueEnd; ++i)
//		{
//			Character ch = i.get();
//			if (!filter(ch))
//			{
//				ch = converter(ch);
//				i.set(ch);
//			}
//		}
//	}
//}

void QString::rto(size_t index, CharacterConverter converter, CharacterFilter filter)
{
	/*if (_checkNegativeIndex(index))
	{
		size_t i = _negToPos(index);
		Character ch = _string[i];
		if (!filter(ch))
		{
			_string[i] = converter(ch);
		}
	}*/
}

////void QString::rto(size_t begin, size_t end, CharacterConverter converter, CharacterFilter filter)
////{
////	if (_checkNegativeIndicies(begin, end))
////	{
////		size_t posBeg = _negToPos(begin);
////		size_t trueEnd = _negToPos(end) + 1;
////		for (size_t i = posBeg; i < trueEnd; ++i)
////		{
////			Character ch = _string[i];
////			if (!filter(ch))
////			{
////				_string[i] = converter(ch);
////			}
////		}
////	}
////}
//
//void QString::rto(ReverseIterator iterator, CharacterConverter converter, CharacterFilter filter)
//{
//	if (_checkReverseIterator(iterator))
//	{
//		Character ch = iterator.get();
//		if (!filter(ch))
//		{
//			ch = converter(ch);
//			iterator.set(ch);
//		}
//	}
//}
//
//void QString::rto(ReverseIterator begin, ReverseIterator end, CharacterConverter converter, CharacterFilter filter)
//{
//	if (_checkReverseIterators(begin, end))
//	{
//		ReverseIterator trueEnd = end + 1;
//		for (ReverseIterator i = begin; i < trueEnd; ++i)
//		{
//			Character ch = i.get();
//			if (!filter(ch))
//			{
//				ch = converter(ch);
//				i.set(ch);
//			}
//		}
//	}
//}

void QString::copy(const Character* string)
{
	size_t max = strlen(string) + 1;
	if (_maxLength < max)
	{
		reserve(max);
	}
	strncpy_s(_string, max, string, max);
	_setLength(max);
}

void QString::copy(const QString& string)
{
	size_t max = string._maxLength;
	if (_maxLength < max)
	{
		reserve(max);
	}
	strncpy_s(_string, max, string._string, max);
	_setLength(string._length);
}

void QString::reserve(size_t size)
{
	if (size > _maxLength)
	{
		Character* string = new Character[size];
		strncpy_s(string, size, _string, _maxLength);
		delete[] _string;
		_string = string;
		_setMaxLength(size);
	}
}

void QString::clear()
{
	delete[] _string;
	_length = 0;
	_maxLength = 0;
}

QBool QString::equals(const Character* other) const
{
	size_t len = strlen(other) + 1;
	if (_length == len)
	{
		return !strncmp(_string, other, len);
	}
	return false;
}

QBool QString::equals(const QString& other) const
{
	size_t len = other._length;
	if (_length == len)
	{
		return !strncmp(_string, other._string, len);
	}
	return false;
}

QBool QString::isEmpty() const
{
	return _length == 0;
}

QBool QString::isFull() const
{
	return _length == _maxLength;
}

int32 QString::toInt32() const
{
	return atoi(_string);
}

float64 QString::toFloat64() const
{
	return atof(_string);
}

const QString::Character* QString::toCString() const
{
	return _string;
}

QString::Reference QString::at(size_t index) const
{
	return _checkIndex(index) ? _string[index] : EMPTY_CHAR;
}

QString::Reference QString::operator[](size_t index) const
{
	return at(index);
}

QString::operator bool() const
{
	return _string && _length && _maxLength;
}

QBool QString::operator==(const Character* other) const
{
	size_t len = strlen(other) + 1;
	if (_length == len)
	{
		return !strncmp(_string, other, len);
	}
	return false;
}

QBool QString::operator!=(const Character* other) const
{
	size_t len = strlen(other) + 1;
	if (_length == len)
	{
		return strncmp(_string, other, len);
	}
	return true;
}

QBool QString::operator<(const Character* other) const
{
	size_t otherLen = strlen(other) + 1;
	size_t len = _length > otherLen ? _length : otherLen;
	return strncmp(_string, other, len) == -1;
}

QBool QString::operator<=(const Character* other) const
{
	size_t otherLen = strlen(other) + 1;
	size_t len = _length > otherLen ? _length : otherLen;
	int32 result = strncmp(_string, other, len);
	return result == -1 || result == 0;
}

QBool QString::operator>(const Character* other) const
{
	size_t otherLen = strlen(other) + 1;
	size_t len = _length > otherLen ? _length : otherLen;
	return strncmp(_string, other, len) == 1;
}

QBool QString::operator>=(const Character* other) const
{
	size_t otherLen = strlen(other) + 1;
	size_t len = _length > otherLen ? _length : otherLen;
	int32 result = strncmp(_string, other, len);
	return result == 1 || result == 0;
}

QBool QString::operator==(const QString& other) const
{
	size_t len = other._length;
	if (_length == len)
	{
		return !strncmp(_string, other._string, len);
	}
	return false;
}

QBool QString::operator!=(const QString& other) const
{
	size_t len = other._length;
	if (_length == len)
	{
		return strncmp(_string, other._string, len);
	}
	return true;
}

QBool QString::operator<(const QString& other) const
{
	size_t len = _length > other._length ? _length : other._length;
	return strncmp(_string, other._string, len) == -1;
}

QBool QString::operator<=(const QString& other) const
{
	size_t len = _length > other._length ? _length : other._length;
	int32 result = strncmp(_string, other._string, len);
	return result == -1 || result == 0;
}

QBool QString::operator>(const QString& other) const
{
	size_t len = _length > other._length ? _length : other._length;
	return strncmp(_string, other._string, len) == 1;
}

QBool QString::operator>=(const QString& other) const
{
	size_t len = _length > other._length ? _length : other._length;
	int32 result = strncmp(_string, other._string, len);
	return result == 1 || result == 0;
}

const QString::Iterator QString::getBegin() const
{
	return Iterator(&_string[0]);
}

const QString::Iterator QString::getEnd() const
{
	return Iterator(&_string[_length - 1]);
}

const QString::ConstIterator QString::getCBegin() const
{
	return ConstIterator(&_string[0]);
}

const QString::ConstIterator QString::getCEnd() const
{
	return ConstIterator(&_string[_length - 1]);
}

const QString::ReverseIterator QString::getRBegin() const
{
	return ReverseIterator(&_string[_length - 2]);
}

const QString::ReverseIterator QString::getREnd() const
{
	return ReverseIterator(&_string[-1]);
}

const QString::ConstReverseIterator QString::getCRBegin() const
{
	return ConstReverseIterator(&_string[_length - 2]);
}

const QString::ConstReverseIterator QString::getCREnd() const
{
	return ConstReverseIterator(&_string[-1]);
}

size_t QString::getLength() const
{
	return _length - 1;
}

size_t QString::getMaxLength() const
{
	return _maxLength - 1;
}

size_t QString::getSize() const
{
	return _length - 1;
}

size_t QString::getMaxSize() const
{
	return _maxLength - 1;
}

void QString::_setLength(size_t len)
{
	_length = len;
}

void QString::_setMaxLength(size_t max)
{
	_maxLength = max;
}

void QString::_incrementLength()
{
	_length++;
}

void QString::_decrementLength()
{
	_length--;
}

void QString::_addLength(size_t amount)
{
	_length += amount;
}

void QString::_subtractLength(size_t amount)
{
	_length -= amount;
}

QBool QString::_hasSpaceFor(size_t num) const
{
	return _length + num <= _maxLength;
}

void QString::_setTerminatingZero(size_t index)
{
	_string[index] = '\0';
}

QBool QString::_checkIndex(size_t index) const
{
	return index < _length - 1 && index > 0 || (index == 0 && _length - 1 > 0);
}

QBool QString::_checkIndicies(size_t begin, size_t end) const
{
	return _checkIndex(begin) && end <= _length - 1 && _comesBefore(begin, end);
}

QBool QString::_checkIterator(const Iterator iterator) const
{
	return iterator >= getBegin() && iterator < getEnd();
}

QBool QString::_checkIterators(const Iterator begin, const Iterator end) const
{
	return _checkIterator(begin) && end <= getEnd() && _comesBefore(begin, end);
}

QBool QString::_checkReverseIterator(const ReverseIterator iterator) const
{
	return iterator >= getRBegin() && iterator < getREnd();
}

QBool QString::_checkReverseIterators(const ReverseIterator begin, const ReverseIterator end) const
{
	return _checkReverseIterator(begin) && _checkReverseIterator(end) && _comesBefore(begin, end);
}

QString::Comparison QString::_compare(const Character* A, const Character* B, size_t length, const CharChecker& checker) const
{
	const Character* c1 = A;
	const Character* c2 = B;
	size_t count = 0;
	while (length)
	{
		if (!checker(*c1, *c2))
		{
			break;
		}

		c1++;
		c2++;
		count++;
		length--;
	}
	return Comparison(count, length == 0);
}

QString::CharChecker QString::_getCharChecker(Sensitivity sensitivity) const
{
	return isSensitive(sensitivity) ? CharChecker(chrcmp) : CharChecker(chricmp);
}

QString::StrChecker QString::_getStrChecker(Sensitivity sensitivity) const
{
	return isSensitive(sensitivity) ? StrChecker(strncmp) : StrChecker(_strnicmp);
}

std::ostream& operator<<(std::ostream& os, const QString& string)
{
	return os << string._string;
}