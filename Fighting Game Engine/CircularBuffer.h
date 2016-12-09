#pragma once
#include <vector>
//Declaration
template <class T>
class CircularBuffer
{
	std::vector<T>* _buffer;
	int _bufferEnd;
public:

	T* at(int index);
	T* back();
	T* end();
	int position();

	int InternalIndex(int index);

	void Advance();
	void Advance(T placedVar);
	void SetPosition(int pos);
	
	void Resize(int size);

	CircularBuffer(int size);
	~CircularBuffer();
};
//Definition.
//C++ WHYYYYYYYYYYYYYYYYYYYYYYYYYY
template <class T>
T* CircularBuffer<T>::at(int index){
	if (_buffer->size() == 0)
		return nullptr;
	return &_buffer->at(InternalIndex(index));
}

template <class T>
T* CircularBuffer<T>::back(){
	if (_buffer->size() == 0)
		return nullptr;
	return &_buffer->at(InternalIndex(_bufferEnd - 1));
}

template <class T>
T* CircularBuffer<T>::end(){
	if (_buffer->size() == 0)
		return nullptr;
	return &_buffer->at(_bufferEnd);
}

template <class T>
int CircularBuffer<T>::InternalIndex(int index){
	if (index < 0){
		return _buffer->size() - 1 - index%_buffer->size();
	}
	else
		return index%_buffer->size();
}

template <class T>
void CircularBuffer<T>::Advance(){
	_bufferEnd = InternalIndex(_bufferEnd + 1);
}

template <class T>
void CircularBuffer<T>::Advance(T placedVar){
	_buffer->at(_bufferEnd) = placedVar;
	_bufferEnd = InternalIndex(_bufferEnd + 1);
}
template <class T>
void CircularBuffer<T>::SetPosition(int pos){
	_bufferEnd = InternalIndex(pos);
}

template <class T>
int CircularBuffer<T>::position(){
	return _bufferEnd;
}

template <class T>
void CircularBuffer<T>::Resize(int size){
	_buffer->resize(size);
}

template <class T>
CircularBuffer<T>::CircularBuffer(int size){
	_buffer = new std::vector<T>();
	_buffer->resize(size);
	_bufferEnd = 0;
}

template<class T>
CircularBuffer<T>::~CircularBuffer(){
	delete _buffer;
}
