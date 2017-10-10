#pragma once
#include <vector>
//Declaration.
//Circular buffers are containers with a fixed size but a variable end. It will always hold the last _buffer.size() entries, with older ones getting replaced by new ones.
template <class T>
class CircularBuffer
{
	std::vector<T>* _buffer;
	int _bufferEnd;
public:

	T* at(int index);//Relative to the beginning of the vector.
	T* at_relative(int index);//Relative to the latest entry
	T* back();//Latest entry
	T* end();//Earliest entry, to be overwritten.
	int position();

	int InternalIndex(int index);

	void Advance();
	void Advance(T& placedVar);//Quick helper function for adding an element and incrementing _bufferEnd
	void SetPosition(int pos);

	void Resize(int size);//Really simple resize function. Will not shift the elements to maintain order.

	void Reset();//Helper function for returning all of the buffer's values to default.

	CircularBuffer(int size);
	~CircularBuffer();
};

template <class T>
T* CircularBuffer<T>::at(int index)
{
	if(_buffer->size() == 0)
		return nullptr;
	return &_buffer->at(InternalIndex(index));
}

template <class T>
T* CircularBuffer<T>::at_relative(int index)
{
	if(_buffer->size() == 0)
		return nullptr;
	return &_buffer->at(InternalIndex(_bufferEnd - 1 + index));
}

template <class T>
T* CircularBuffer<T>::back()
{
	if(_buffer->size() == 0)
		return nullptr;
	return &_buffer->at(InternalIndex(_bufferEnd - 1));
}

template <class T>
T* CircularBuffer<T>::end()
{
	if(_buffer->size() == 0)
		return nullptr;
	return &_buffer->at(_bufferEnd);
}

template <class T>
int CircularBuffer<T>::InternalIndex(int index)
{
	if(index < 0)
	{
		return _buffer->size() - 1 - index%_buffer->size();
	}
	else
		return index % _buffer->size();
}

template <class T>
void CircularBuffer<T>::Advance()
{
	_bufferEnd = InternalIndex(_bufferEnd + 1);
}

template <class T>
void CircularBuffer<T>::Advance(T& placedVar)
{
	_buffer->at(_bufferEnd) = placedVar;
	_bufferEnd = InternalIndex(_bufferEnd + 1);
}

template <class T>
void CircularBuffer<T>::SetPosition(int pos)
{
	_bufferEnd = InternalIndex(pos);
}

template <class T>
int CircularBuffer<T>::position()
{
	return _bufferEnd;
}

template <class T>
void CircularBuffer<T>::Resize(int size)
{
	_buffer->resize(size);
	_bufferEnd = InternalIndex(_bufferEnd);
}

template <class T>
void CircularBuffer<T>::Reset()
{
	for(auto& i : _buffer)
	{
		i = T();
	}
	_bufferEnd = 0;
}

template <class T>
CircularBuffer<T>::CircularBuffer(int size)
{
	_buffer = new std::vector<T>();
	_buffer->resize(size);
	_bufferEnd = 0;
}

template<class T>
CircularBuffer<T>::~CircularBuffer()
{
	delete _buffer;
}
