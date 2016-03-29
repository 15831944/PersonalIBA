#include "stdafx.h"

#include "WX_base.h"
#include "stdlib.h"

bool isBlankChar(const char ch){
	return ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n';
}

void appendTabByDepth(std::string& strResult, int depth)
{
	if (depth == 0) return;
	char * p = new char[ depth + 1 ];
	for (int i = 0; i < depth; i++) {
		p[ i ] = '\t';
	}
	p[ depth ] = 0;
	strResult.append(p);
	delete []p;
}

void appendStringDoesEscape(std::string& strResult, const char* pString)
{
	int nC = (int)strlen(pString);
	char* p = new char[nC*2+1];
	if (p) {
		memset(p,0,nC*2+1);
		int i = 0;
		while(*pString) {
			if (*pString == '\"' || *pString == '{' || *pString == '}' || *pString == '[' || *pString == ']' || *pString == '\\' || *pString ==':'){
				p[i++] = '\\';
			}
			p[i++] = *pString;
			pString++;
		}
		strResult.append(p);
		delete []p;
	}
}

#define GXX_INNER_CYCLE(_point) {if (*_point == '\\'){_point += 2;continue;}const char* _p = _point;while(((unsigned char)*_p) & 0x80)_p++;if (_p != _point){ _point += (_p - _point); continue;}}

GxxObject::GxxObject()
:retainCount(0)
{
	
}

GxxObject::~GxxObject()
{

}

void GxxObject::retain()
{
	retainCount += 1;
}

void GxxObject::release()
{
	retainCount -= 1;
	if (retainCount == 0)
	{
		objectReleased();
		delete this;
	}
}

void GxxObject::_print( const char* x )
{
	GXX_PRINT(x);
}

void GxxObject::_println( const char* x )
{
	GXX_PRINT(x); GXX_PRINT("\n");
}

//-------------------------------------------------------------------------

bool GxxDictionary::init()
{
	keyValues = GxxValueMap::create();
	return true;
}

void GxxDictionary::objectReleased()
{
	
}

bool GxxDictionary::initWithJsonText(const char* jsonText) 
{
	const char* p = jsonText;
	int nStack = 0;
	const char* pFirst = 0;
	const char* pLast = 0;
	int nQutCount = 0; // ���Ÿ���

	while(*p)
	{
		GXX_INNER_CYCLE(p); // ����������ANSII�ַ�,ת���
		if (*p == '{' && nQutCount%2 == 0){
			if ( (++nStack) == 1) pFirst = p;
		}
		else if(*p == '}' && nQutCount%2 == 0){
			--nStack;
			if (nStack < 0){
				throw GxxException("{} does not match.");// {}������ƥ��
				return false;
			}
			if (nStack == 0){
				pLast = p;
				break;
			}
		}
		else if(*p == '\"'){
			nQutCount++;
		}
		p++;
	}
	if (!pFirst || !pLast){
		throw GxxException("{} does not match.");// {}������ƥ��
		return false;
	}
	if (pLast - pFirst < 2){
		return true; // �հ���Ϣ
	}

	// �Ѿ��ҵ���������λ��, ��ȡkey���ŵ�λ��
	while(pFirst)
	{
		// 1: �ҵ�key�������ź�β���ŵ�λ��
		int nQuoMark = 0;
		const char* pFirstQuo = 0;
		const char* pLastQuo = 0;
		const char* pColon = 0; // ð�ŵ�λ��
		p = pFirst + 1;
		while (p != pLast)
		{
			GXX_INNER_CYCLE(p);
			if (*p == '\"'){
				++nQuoMark;
				if (nQuoMark == 1){
					pFirstQuo = p;
				}
				else
				{
					pLastQuo = p;
				}
			}
			else if(*p == ':' && nQuoMark >= 2)
			{
				pColon = p;
				break;
			}
			p++;
		}
		if (!pFirstQuo || !pLastQuo || !pColon)
		{
			throw GxxException("invalid json text.");
			return false;
		}

		GxxKey key;
		key.resize((unsigned int)(pLastQuo - pFirstQuo -1));
		for (int i = 1,j = 0; i <= pLastQuo - pFirstQuo -1; i++){
			if (pFirstQuo[i] == '\\'){
				key[j++] = pFirstQuo[++i];
			} else {
				key[j++] = pFirstQuo[i];
			}
		}

		// 2: �õ�value
		p = pColon + 1;
		const char* pFirstValue = 0;
		while(p != pLast)
		{
			if (!isBlankChar(*p))
			{
				pFirstValue = p;
				break;
			}
			p++;
		}
		const char* pEndValue = 0;
		switch(*pFirstValue)
		{
		case '\"':
			{
				p = pFirstValue + 1;
				int nReCount = 1;
				while(p != pLast)
				{
					GXX_INNER_CYCLE(p);
					if (*p == '\"')
					{
						nReCount++;
						pEndValue = p;
					}
					if (*p == ',' && nReCount % 2 == 0)
					{
						p--;break;
					}
					p++;
				}
				if (!pEndValue)
				{
					throw GxxException("invalid json text.");
					return false;
				}
				std::string tmp;
				tmp.resize(pEndValue - pFirstValue -1);
				for (int i = 1,j = 0; i <= pEndValue - pFirstValue -1; i++){
					if (pFirstValue[i] == '\\'){
						tmp[j++] = pFirstValue[++i];
					} else {
						tmp[j++] = pFirstValue[i];
					}
				}

				GxxValuePtr ptrV = GxxValue::create(tmp.c_str());
				keyValues->PutValue(key, ptrV);
			}
			break;
		case '{':
			{
				p = pFirstValue;
				nStack = 0;
				while(p != pLast)
				{
					GXX_INNER_CYCLE(p);
					if (*p == '{') ++nStack;
					else if(*p == '}')
					{
						--nStack;
						if (nStack == 0)
						{
							pEndValue = p;
							break;
						}
					}
					p++;
				}
				if (!pEndValue)
				{
					throw GxxException("invalid json text.");
					return false;
				}
				std::string strDic;
				strDic.append(pFirstValue, pEndValue - pFirstValue + 1);
				GxxDictionaryPtr dictionary = GxxDictionary::create();
				dictionary->initWithJsonText(strDic.c_str());
				keyValues->PutValue(key, GxxValue::create(dictionary));
			}
			break;
		case '[':
			{
				p = pFirstValue;

				
				nStack = 0;
				while(p != pLast)
				{
					GXX_INNER_CYCLE(p);
					if (*p == '[')
					{
						++nStack;
					}
					else if(*p == ']')
					{
						--nStack;
						if (nStack == 0)
						{
							pEndValue = p;
							break;
						}
					}
					p++;
				}
				if (!pEndValue)
				{
					throw GxxException("[] does not match.");
					return false;
				}
				std::string strDic;
				strDic.append(pFirstValue, pEndValue - pFirstValue + 1);
				GxxArrayPtr array = GxxArray::create();
				array->initWithJsonText(strDic.c_str());
				keyValues->PutValue(key, GxxValue::create(array));
			}
			break;
		default:
			{
				// ��ֵ, �ҵ����һ������ֵ
				p = pFirstValue;
				if (!(*p == '-' || *p == '+' || *p == '.' || ('0' <= *p && *p <= '9')))
				{
					throw GxxException("invalid json text.");
					return false;
				}
				if (*p == '-' || *p == '+') p++;
				while(*p)
				{
					if (!(('0' <= *p && *p <= '9') || *p == '.'))
					{
						if (*p == 'f' || *p == 'F')
							pEndValue = p;
						else
							pEndValue = p - 1;
						break;
					}
					p++;
				}
				std::string tmp;
				tmp.append(pFirstValue, pEndValue - pFirstValue + 1);
				keyValues->PutValue(key, GxxValue::create(tmp.c_str()));
			}
			break;
		}
		// �ҵ� , or }
		p = pEndValue + 1;
		pFirst = 0;
		while(p != pLast)
		{
			if (*p == ',')
			{
				pFirst = p;
				break;
			}
			p++;
		}

	}
	return true;
}

void GxxDictionary::print()
{
	_println("{");
	for (GxxValueMap::Iterator it = keyValues->FirstValue(); !keyValues->IsLastValue(it); keyValues->NextValue(it))
	{
		GxxString* pString = it->second->getString();
		_print(it->first);
		_print(": ");
		if (pString)
		{
			pString->print();
		}
		if (it->second->getDictionary())
		{
			it->second->getDictionary()->print();
		}
		if (it->second->getArray())
		{
			it->second->getArray()->print();
		}
	}
	_println("}");
}

bool GxxDictionary::isKeyExist(const char* key)
{
	return keyValues->ValueForKey(key) != NULL ;
}

GxxValue* GxxDictionary::valueForKey(const char* key)
{
	return keyValues->ValueForKey(key);
}

GxxArrayPtr GxxDictionary::names()
{
	GxxArrayPtr arr = GxxArray::create();
	
	for (GxxValueMap::Iterator it = keyValues->FirstValue(); !keyValues->IsLastValue(it); keyValues->NextValue(it)) {
		arr->addValue(GxxValue::create(it->first));
	}
	return arr;
}

int GxxDictionary::count()
{
	return (int)keyValues->count();
}

GxxValue* GxxDictionary::operator[](const char* key)
{
	return valueForKey(key);
}

void GxxDictionary::setValueForKey(GxxValuePtr& pValue, const char* key)
{
	keyValues->PutValue(GxxKey(key), pValue);
}

GxxValue* GxxDictionary::valueForKeyPath(const char* keyPath)
{
	std::string segment;
	const char* first = keyPath;
	const char* p = keyPath;
	GxxValue* pCurrentValue = 0;
	while(*p)
	{
		if (*p == '.'){
			if (p - first < 1){
				++p;
				continue;
			}

			bool bIsArray = false;
			const char* temp = p;
			if (*(temp - 1) == ']'){
				while(temp != first)
				{
					if (*temp == '['){
						bIsArray = true;
						break;
					}
					--temp;
				}
			}
			if (bIsArray){
				segment.clear();
				segment.append(first, temp - first);
				if (pCurrentValue == 0) {
					pCurrentValue = this->valueForKey(segment.c_str());
				}
				else {
					if (pCurrentValue->getDictionary() == 0){
						return 0;
					}
					pCurrentValue = pCurrentValue->getDictionary()->valueForKey(segment.c_str());
				}

				if (pCurrentValue == 0){
					_println("valueForKeyPath error: key not exist!");
					return 0;
				}
				if (pCurrentValue->getArray() == 0){
					_println("valueForKeyPath error: key is invalid, it is not a array object");
					return 0;
				}
				std::string strTmp;
				strTmp.append(temp+1, p - temp - 2);
				int nIndex = atoi(strTmp.c_str());
				pCurrentValue = pCurrentValue->getArray()->valueAtIndex(nIndex);
				if (pCurrentValue == 0){
					_println("valueForKeyPath error: invalid index");
					return 0;
				}
			}
			else{
				segment.clear();
				segment.append(first, temp - first);
				if (pCurrentValue == 0) {
					pCurrentValue = this->valueForKey(segment.c_str());
				}
				else {
					if (pCurrentValue->getDictionary() == 0){
						return 0;
					}
					pCurrentValue = pCurrentValue->getDictionary()->valueForKey(segment.c_str());
				}

				if (pCurrentValue == 0){
					_println("valueForKeyPath error: key not exist!");
					return 0;
				}
			}
			if (pCurrentValue->getDictionary() == 0){
				_println("valueForKeyPath error: key is invalid, it is not a dictionary object");
				return 0;
			}
			first = p + 1;
		}
		++p;
	}
	if (p - first < 1){
		return pCurrentValue;
	}

	bool bIsArray = false;
	const char* temp = p;
	if (*(temp - 1) == ']'){
		while(temp != first)
		{
			if (*temp == '['){
				bIsArray = true;
				break;
			}
			--temp;
		}
	}
	if (bIsArray){
		segment.clear();
		segment.append(first, temp - first);
		if (pCurrentValue == 0) {
			pCurrentValue = this->valueForKey(segment.c_str());
		}
		else {
			if (pCurrentValue->getDictionary() == 0){
				return 0;
			}
			pCurrentValue = pCurrentValue->getDictionary()->valueForKey(segment.c_str());
		}

		if (pCurrentValue == 0){
			_println("valueForKeyPath error: key not exist!");
			return 0;
		}
		if (pCurrentValue->getArray() == 0){
			_println("valueForKeyPath error: key is invalid, it is not a array object");
			return 0;
		}
		std::string strTmp;
		strTmp.append(temp+1, p - temp - 2);
		int nIndex = atoi(strTmp.c_str());
		pCurrentValue = pCurrentValue->getArray()->valueAtIndex(nIndex);
		if (pCurrentValue == 0){
			_println("valueForKeyPath error: invalid index");
			return 0;
		}
	}
	else{
		segment.clear();
		segment.append(first, temp - first);
		if (pCurrentValue == 0) {
			pCurrentValue = this->valueForKey(segment.c_str());
		}
		else {
			if (pCurrentValue->getDictionary() == 0){
				return 0;
			}
			pCurrentValue = pCurrentValue->getDictionary()->valueForKey(segment.c_str());
		}

		if (pCurrentValue == 0){
			_println("valueForKeyPath error: key not exist!");
			return 0;
		}
	}

	return pCurrentValue;
}

void GxxDictionary::setValueForKeyPath(GxxValuePtr& pValue, const char* keyPath)
{
	std::string segment;
	const char* first = keyPath;
	const char* p = keyPath;
	GxxValue* pCurrentValue = 0;
	while(*p)
	{
		if (*p == '.'){
			if (p - first < 1){
				++p;
				continue;
			}

			bool bIsArray = false;
			const char* temp = p;
			if (*(temp - 1) == ']'){
				while(temp != first)
				{
					if (*temp == '['){
						bIsArray = true;
						break;
					}
					--temp;
				}
			}
			if (bIsArray){
				segment.clear();
				segment.append(first, temp - first);
				if (pCurrentValue == 0) {
					pCurrentValue = this->valueForKey(segment.c_str());
				}
				else {
					if (pCurrentValue->getDictionary() == 0){
						return ;
					}
					pCurrentValue = pCurrentValue->getDictionary()->valueForKey(segment.c_str());
				}

				if (pCurrentValue == 0){
					_println("valueForKeyPath error: key not exist!");
					return;
				}
				if (pCurrentValue->getArray() == 0){
					_println("valueForKeyPath error: key is invalid, it is not a array object");
					return;
				}


				std::string strTmp;
				strTmp.append(temp+1, p - temp - 2);
				int nIndex = atoi(strTmp.c_str());
				pCurrentValue = pCurrentValue->getArray()->valueAtIndex(nIndex);
				if (pCurrentValue == 0){
					_println("valueForKeyPath error: invalid index");
					return;
				}
			}
			else{
				segment.clear();
				segment.append(first, temp - first);
				if (pCurrentValue == 0) {
					pCurrentValue = this->valueForKey(segment.c_str());
				}
				else {
					if (pCurrentValue->getDictionary() == 0){
						return ;
					}
					pCurrentValue = pCurrentValue->getDictionary()->valueForKey(segment.c_str());
				}

				if (pCurrentValue == 0){
					_println("valueForKeyPath error: key not exist!");
					return ;
				}
			}
			if (pCurrentValue->getDictionary() == 0){
				_println("valueForKeyPath error: key is invalid, it is not a dictionary object");
				return ;
			}
			first = p + 1;
		}
		++p;
	}
	if (p - first < 1){
		return;
	}

	bool bIsArray = false;
	const char* temp = p;
	if (*(temp - 1) == ']'){
		while(temp != first)
		{
			if (*temp == '['){
				bIsArray = true;
				break;
			}
			--temp;
		}
	}
	if (bIsArray){
		segment.clear();
		segment.append(first, temp - first);
		if (pCurrentValue == 0) {
			pCurrentValue = this->valueForKey(segment.c_str());
		}
		else {
			if (pCurrentValue->getDictionary() == 0){
				return;
			}
			pCurrentValue = pCurrentValue->getDictionary()->valueForKey(segment.c_str());
		}

		if (pCurrentValue == 0){
			_println("valueForKeyPath error: key not exist!");
			return;
		}
		if (pCurrentValue->getArray() == 0){
			_println("valueForKeyPath error: key is invalid, it is not a array object");
			return;
		}
		std::string strTmp;
		strTmp.append(temp+1, p - temp - 2);
		int nIndex = atoi(strTmp.c_str());

		GxxArray* pArray = pCurrentValue->getArray();
		if (nIndex >= 0 && nIndex < pArray->count()){
			pArray->setValue(pValue, nIndex);
		}else{
			pArray->addValue(pValue);
		}
	}
	else{
		segment.clear();
		segment.append(first, temp - first);
		if (pCurrentValue == 0) {
			this->setValueForKey(pValue, segment.c_str());
		}
		else {
			if (pCurrentValue->getDictionary() == 0){
				return ;
			}
			pCurrentValue->getDictionary()->setValueForKey(pValue, segment.c_str());
		}
	}
}

int GxxDictionary::intValueForKey( const char* key, int defaultValue /*= 0*/ )
{
	GxxValue* pValue = valueForKey(key);
	if (pValue)
	{
		return pValue->intValue();
	}
	return defaultValue;
}


bool GxxDictionary::boolValueForKey( const char* key, bool defaultValue /*= false*/ )
{
	GxxValue* pValue = valueForKey(key);
	if (pValue)
	{
		return pValue->intValue() > 0;
	}
	return defaultValue;
}


std::string GxxDictionary::describe()
{
	return describe_d (0, true);
}

std::string GxxDictionary::describe_d( int depth, bool bFormat)
{
	std::string strResult;
	if (bFormat) {
		appendTabByDepth(strResult, depth);
	}
	strResult.append("{");
	if (bFormat)
		strResult.append("\n");
	
	int i = 0;
	int nCount = count();
	GxxValueMap::Iterator pos = this->keyValues->FirstValue();
	for (; !keyValues->IsLastValue(pos); keyValues->NextValue(pos),i++ )
	{
		if (bFormat) {
			appendTabByDepth(strResult, depth+1);
		}
		strResult.append("\"");
		appendStringDoesEscape(strResult, pos->first);
		strResult.append("\":");
		if (pos->second->getString()) {
			strResult.append("\"");
			appendStringDoesEscape(strResult, pos->second->stringValue());
			strResult.append("\"");
			if (i != nCount - 1)
				strResult.append(","); // ��Ӷ���
			if (bFormat)
				strResult.append("\n");
		}
		else if(pos->second->getDictionary()) {
			if (bFormat)
				strResult.append("\n");
			strResult.append( pos->second->getDictionary()->describe_d(depth + 1, bFormat));
			if (i != nCount-1) {
				if (strResult[strResult.length()-1] == '\n'){
					strResult[strResult.length()-1] = ',';
				}else {
					strResult.append(",");
				}
				if (bFormat) {
					strResult.append("\n");
				}
			}
		}
		else if(pos->second->getArray()) {
			if (bFormat)
				strResult.append("\n");
			strResult.append( pos->second->getArray()->describe_d(depth + 1, bFormat));
			if (i != nCount-1) {
				if (strResult[strResult.length()-1] == '\n'){
					strResult[strResult.length()-1] = ',';
				}else {
					strResult.append(",");
				}
				if (bFormat) {
					strResult.append("\n");
				}
			}
		}
	}
	if (bFormat) {
		appendTabByDepth(strResult, depth);
	}
	strResult.append("}");
	if (bFormat) {
		strResult.append("\n");
	}

	return strResult;
}

std::string GxxDictionary::toJsonText()
{
	return describe_d(0, false);
}

const char* GxxDictionary::stringValueForKey( const char* key )
{
	return valueForKey(key)->stringValue();
}



//-------------------------------------------------------------------------

void GxxValue::objectReleased()
{
	
}

void GxxValue::_init()
{
}

const char* GxxValue::stringValue()
{
	if (ptrStringData){
		return ptrStringData->getString();
	}
	return 0;
}

int GxxValue::intValue()
{
	if (ptrStringData){
		return atoi(stringValue());
	}
	return 0;
}

float GxxValue::floatValue()
{
	if (ptrStringData){
		return (float)atof(stringValue());
	}
	return 0;
}

double GxxValue::doubleValue()
{
	if (ptrStringData){
		return atof(stringValue());
	}
	return 0;
}

GxxValuePtr GxxValue::create(const GxxDictionaryPtr& pValue)
{
	GxxValuePtr ptrObj = create();
	if (!ptrObj){
		return ptrObj;
	}
	ptrObj->ptrDictionaryData = pValue;
	return ptrObj;
}

GxxValuePtr GxxValue::create(GxxArrayPtr& pValue)
{
	GxxValuePtr ptrObj = create();
	if (!ptrObj){
		return ptrObj;
	}
	ptrObj->ptrArrayData = pValue;
	return ptrObj;
}

GxxValuePtr GxxValue::create(GxxStringPtr& pValue)
{
	GxxValuePtr ptrObj = create();
	if (!ptrObj){
		return ptrObj;
	}
	ptrObj->ptrStringData = pValue;
	return ptrObj;
}

GxxValuePtr GxxValue::create(const char* pValue)
{
	GxxValuePtr ptrObj = create();
	if (!ptrObj){
		return ptrObj;
	}
	ptrObj->ptrStringData = GxxString::create(pValue);
	return ptrObj;
}

GxxValuePtr GxxValue::create(GxxObjectPtr& ptrOtherValue)
{
	GxxValuePtr ptrObj = create();
	if (!ptrObj){
		return ptrObj;
	}
	ptrObj->ptrOtherData = ptrOtherValue;
	return ptrObj;
}

GxxValuePtr GxxValue::create( long intValue )
{
	char tmp[20];
	sprintf(tmp, "%ld", intValue);
	return GxxValue::create((const char*)tmp);
}

GxxValuePtr GxxValue::create( long long int64Value )
{
	char tmp[40];
	sprintf(tmp, "%lld", int64Value);
	return GxxValue::create((const char*)tmp);
}

GxxValuePtr GxxValue::create( double floatValue)
{
	char tmp[20];
	sprintf(tmp, "%f", floatValue);
	return GxxValue::create((const char*)tmp);
}


GxxValuePtr GxxValue::create( unsigned long uValue)
{
	char tmp[20];
	sprintf(tmp, "%ud", uValue);
	return GxxValue::create((const char*)tmp);
}
void GxxValue::print()
{
	if		 (ptrStringData)			ptrStringData->print();
	else if (ptrDictionaryData)			ptrDictionaryData->print();
	else if (ptrArrayData) 				ptrArrayData->print();
	else if (ptrOtherData)				ptrOtherData->print();
}

std::string GxxValue::describe()
{
	if		 (ptrStringData)			return ptrStringData->describe();
	else if (ptrDictionaryData)			return ptrDictionaryData->describe();
	else if (ptrArrayData) 				return ptrArrayData->describe();
	else if (ptrOtherData)				return ptrOtherData->describe();
	return std::string("(null)");
}


//-------------------------------------------------------------------------

bool GxxArray::initWithJsonText(const char* jsonText)
{
	if (strlen(jsonText) == 0) 
		return false;

	const char* p = jsonText;
	int nStack = 0;
	const char* pFirst = 0;
	const char* pLast = 0;
	int nQutCount = 0; // ���Ÿ���

	while(*p)
	{
		GXX_INNER_CYCLE(p);
		if (*p == '[' && nQutCount%2 == 0){
			if ( (++nStack) == 1) pFirst = p;
		}
		else if(*p == ']' && nQutCount%2 == 0){
			--nStack;
			if (nStack < 0){
				throw GxxException("[] does not match.");// []��ƥ��
				return false;
			}
			if (nStack == 0){
				pLast = p;
				break;
			}
		}
		else if(*p == '\"'){
			nQutCount++;
		}
		p++;
	}

	if (!pFirst || !pLast){
		throw GxxException("invalid json text.");
		return false;
	}
	if (pLast - pFirst == 1) {
		// ������
		return true;
	}

	while(pFirst)
	{
		const char* pFirstValue= 0;
		p = pFirst + 1;
		while(p != pLast)
		{
			GXX_INNER_CYCLE(p);
			if (!isBlankChar(*p))
			{
				pFirstValue = p;
				break;
			}
			p++;
		}
		const char* pEndValue = 0;
		if (pFirstValue == 0){
			break;
		}
		if (*pFirstValue == ','){
			arrayValues.push_back(GxxValue::create(""));
			pFirst = pFirstValue;
			continue;
		}
		switch(*pFirstValue)
		{
		case '\"':
			{
				p = pFirstValue + 1;
				int nReCount = 1;
				while(p != pLast)
				{
					GXX_INNER_CYCLE(p);
					if (*p == '\"')
					{
						nReCount ++;
						pEndValue = p;
					}
					else if(*p == ',' && nReCount >= 2)
					{
						p--;
						break;
					}
					p++;
				}
				if (!pEndValue)
				{
					throw GxxException("\" does not match.");
					return false;
				}
				std::string tmp;
				tmp.resize(pEndValue - pFirstValue -1);
				for (int i = 1,j = 0; i <= pEndValue - pFirstValue -1; i++){
					if (pFirstValue[i] == '\\'){
						tmp[j++] = pFirstValue[++i];
					} else {
						tmp[j++] = pFirstValue[i];
					}
				}

				arrayValues.push_back(GxxValue::create(tmp.c_str()));
			}
			break;
		case '{':
			{
				p = pFirstValue;
				nStack = 0;
				while(p != pLast)
				{
					GXX_INNER_CYCLE(p);
					if (*p == '{') 
					{
						++nStack;
					}
					else if(*p == '}')
					{
						--nStack;
						if (nStack == 0)
						{
							pEndValue = p;
							break;
						}
					}
					
					p++;
				}
				if (!pEndValue)
				{
					throw GxxException("{} does not match.");
					return false;
				}
				std::string strDic;
				strDic.append(pFirstValue, pEndValue - pFirstValue + 1);
				GxxDictionaryPtr dictionary = GxxDictionary::create();
				dictionary->initWithJsonText(strDic.c_str());
				arrayValues.push_back(GxxValue::create(dictionary));
			}
			break;
		case '[':
			{
				p = pFirstValue;
				nStack = 0;
				while(p != pLast)
				{
					GXX_INNER_CYCLE(p);
					if (*p == '[') ++nStack;
					else if(*p == ']')
					{
						--nStack;
						if (nStack == 0)
						{
							pEndValue = p;
							break;
						}
					}
					p++;
				}
				if (!pEndValue)
				{
					throw GxxException("[] does not match.");
					return false;
				}
				std::string strDic;
				strDic.append(pFirstValue, pEndValue - pFirstValue + 1);
				GxxArrayPtr array = GxxArray::create();
				array->initWithJsonText(strDic.c_str());
				arrayValues.push_back(GxxValue::create(array));
			}
			break;
		default:
			{
				// ��ֵ, �ҵ����һ������ֵ
				p = pFirstValue;
				if (!(*p == '-' || *p == '+' || *p == '.' || ('0' <= *p && *p <= '9')))
				{
					throw GxxException("invalid json text.");
					return false;
				}
				if (*p == '-' || *p == '+') p++;
				while(*p)
				{
					if (!(('0' <= *p && *p <= '9') || *p == '.'))
					{
						if (*p == 'f' || *p == 'F')
							pEndValue = p;
						else
							pEndValue = p - 1;
						break;
					}
					p++;
				}
				std::string tmp;
				tmp.append(pFirstValue, pEndValue - pFirstValue + 1);
				arrayValues.push_back(GxxValue::create(tmp.c_str()));
			}
			break;
		}
		// �ҵ� , or }
		p = pEndValue + 1;
		pFirst = 0;
		while(p != pLast)
		{
			if (*p == ',')
			{
				pFirst = p;
				break;
			}
			p++;
		}
	}
	return true;
}

void GxxArray::print()
{
	_println("[");
	for (ArrayValue::iterator it = arrayValues.begin(); it != arrayValues.end(); it++)
	{
		GxxString* pString = (*it)->getString();
		if (pString)
		{
			pString->print();
		}
		if ((*it)->getDictionary())
		{
			(*it)->getDictionary()->print();
		}
		if ((*it)->getArray())
		{
			(*it)->getArray()->print();
		}
	}
	_println("]");
}



void GxxArray::objectReleased()
{
	
}

int GxxArray::count()
{
	return (int)arrayValues.size();
}

GxxValue* GxxArray::valueAtIndex(int iIndex)
{
	if (iIndex < 0 || iIndex >= count())
		return 0;
	return arrayValues[iIndex];
}

void GxxArray::addValue(const GxxValuePtr& pValue)
{
	arrayValues.push_back(pValue);
}

void GxxArray::setValue(GxxValuePtr& pValue, int iIndex)
{
	if (iIndex < 0 || iIndex >= count())
		return;

	arrayValues[iIndex] = pValue;
}

void GxxArray::removeValue( int iIndex )
{
	arrayValues.erase(arrayValues.begin() + iIndex);
}

void GxxArray::removeAllValues()
{
	arrayValues.clear();
}

std::string GxxArray::describe()
{
	return describe_d(0, true);
}

std::string GxxArray::describe_d( int depth, bool bFormat )
{
	std::string strResult;
	if (bFormat) {
		appendTabByDepth(strResult, depth);
	}
	strResult.append("[");
	if (bFormat)
		strResult.append("\n");

	for (int i = 0; i <  this->count(); i++) 
	{
		GxxValue* pValue = this->valueAtIndex(i);
		
		if (pValue->getString()) {
			if (bFormat) {
				appendTabByDepth(strResult, depth + 1);
			}
			strResult.append("\"");
			appendStringDoesEscape(strResult, pValue->stringValue());
			strResult.append("\"");
			if (i != this->count()-1)
				strResult.append(","); // ��Ӷ���
			if (bFormat)
				strResult.append("\n"); // ��ӻ���
		}
		else if(pValue->getDictionary()) {
			strResult.append( pValue->getDictionary()->describe_d(depth + 1, bFormat));
			if (i != this->count()-1) {
				if (strResult[strResult.length()-1] == '\n'){
					strResult[strResult.length()-1] = ',';
				}else {
					strResult.append(",");
				}
				if (bFormat) {
					strResult.append("\n");
				}
			}
		}
		else if(pValue->getArray()) {
			strResult.append( pValue->getArray()->describe_d(depth + 1, bFormat));
			if (i != this->count()-1) {
				if (strResult[strResult.length()-1] == '\n'){
					strResult[strResult.length()-1] = ',';
				}else {
					strResult.append(",");
				}
				if (bFormat) {
					strResult.append("\n");
				}
			}
		}
	}
	if (bFormat) {
		appendTabByDepth(strResult, depth);
	}
	strResult.append("]");
	if (bFormat) {
		strResult.append("\n");
	}

	return strResult;
}

std::string GxxArray::toJsonText()
{
	return describe_d(0, false);
}

//-------------------------------------------------------------------------

void GxxString::print()
{
	_println(_string.c_str());
}

const char* GxxString::getString()
{
	return _string.c_str();
}

GxxStringPtr GxxString::create(const char* str)
{
	GxxStringPtr ptrObj = GxxString::create();
	if (!ptrObj){
		return ptrObj;
	}
	ptrObj->_string = str;
	return ptrObj;
}
//--------------------------------------------------------------------------
bool GxxValueMap::init()
{
	__pMap = new TyMap;
	return true;
}

void GxxValueMap::objectReleased()
{
	__pMap->clear();
	delete __pMap;
}

void GxxValueMap::PutValue(const GxxKey& _key, const GxxValuePtr& _value )
{
	(*__pMap)[_key] = _value;
}

GxxValuePtr GxxValueMap::ValueForKey(const GxxKey& _key )
{
	Iterator it = __pMap->find(_key);
	if (it != __pMap->end()) {
		return it->second;
	}
	return GxxValuePtr();
}

GxxValueMap::Iterator GxxValueMap::FirstValue()
{
	return __pMap->begin();
}

void GxxValueMap::NextValue( Iterator& pos )
{
	if (pos != __pMap->end())
	{
		++pos;
	}
}

int GxxValueMap::count()
{
	return (int)__pMap->size();
}

bool GxxValueMap::IsLastValue( Iterator& it )
{
	return it == __pMap->end();
}
