#pragma once

#define OS11_HTAPI_API extern "C" __declspec(dllexport)

namespace ht
{
	OS11_HTAPI_API struct Element   // элемент 
	{
		Element();
		Element(const void* key, int keyLength);                                             // for get
		Element(const void* key, int keyLength, const void* payload, int  payloadLength);    // for insert
		Element(const Element* oldElement, const void* newPayload, int  newPayloadLength);   // for update
		const void* key;                 // значение ключа 
		int         keyLength;           // рахмер ключа
		const void* payload;             // данные 
		int         payloadLength;       // размер данных
	};

	OS11_HTAPI_API Element* createGetElement(const void* key, int keyLength);
	OS11_HTAPI_API Element* createInsertElement(const void* key, int keyLength, const void* payload, int  payloadLength);

	int getSizeElement
	(
		int   maxKeyLength,                // максимальный размер ключа
		int   maxPayloadLength			   // максимальный размер данных
	);           
}