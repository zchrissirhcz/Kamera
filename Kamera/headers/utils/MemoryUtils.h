#pragma once

template <typename T>
T** Create2DTable(unsigned int w, unsigned int h, bool clear_table = false)
{
	T **object = new T*[w];
	for (int i=0; i<w; i++)
	{
		object[i] = new T[h];
		if (clear_table)
			memset(object[i], 0, sizeof(T)*h);
	}
	return object;
}

template <typename T>
void DestroyObject(T **object)
{
	if (*object!=NULL)
	{
		delete *object;
		*object = NULL;
	}
}

template <typename T>
void DestroyTable(T **object)
{
	if (*object!=NULL)
	{
		delete [](*object);
		*object = NULL;
	}
}

template <typename T>
void Destroy2DTable(T **object, int w)
{
	if (object!=NULL)
	{
		for (int i=0; i<w; i++)
			delete [](object[i]);
		delete []object;
		object = NULL;
	}
}
