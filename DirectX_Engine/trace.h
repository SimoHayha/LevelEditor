/**************************************************/
/***
 * Copyright (C) $Date$
 * E. Scott Larsen and NVIDIA
 **/
/**************************************************/
#pragma once
/**************************************************/
/***
 * \file
 **/
/**************************************************/
#include <vector>
#include <string>
#ifdef max
#undef max
#endif
#ifdef min
#undef min
#endif
/**************************************************/
/// CTrace: 
/**
 * \class CTrace trace.h "trace.h"
 **
 *
 **/
/**************************************************/
template <class T>
class CTrace
{
public:
    CTrace(const size_t capacity_=1000,const std::string name_="")
        :_capacity(capacity_),
        _max(0),
        _min(0),
        _name(name_),
        _front(0),
        _full(false)
    {
        _data.resize(_capacity);
    }
    ~CTrace(){}
    void insert(const T d)
    {
        _max = _max < d ? d : _max;
        _min = _min > d ? d : _min;
        _data[_front++] = d;
        if (_front == _capacity)
        {
            _front = 0;
            _full = true;
        }
    }
    const size_t capacity()const				{return _capacity;}
    void capacity(const size_t& c)				{_capacity = c;}
    const size_t size()const					{return _full?_capacity:_front;}
    const T operator[](const size_t i)const {return _data[i];}
    const T operator()(const size_t i)const {return _data[(i+_full*_front)%_capacity];}
    const T last()                          {return _data[(_front-1+_capacity)%_capacity];}
    const T secondToLast()                  {return _data[(_front-2+_capacity)%_capacity];}
    const T max()const                      {return _max;}
    const T min()const                      {return _min;}
    void max(const T m)                     {_max = m;}
    void min(const T m)                     {_min = m;}
    const std::string name()const           {return _name;}
    void name(const std::string&s)          {_name = s;}
protected:
    size_t _capacity;
    T _max;
    T _min;
    std::string _name;
private://for implementing the circularQ
    std::vector<T> _data;
    size_t _front;
    bool _full;
private:
//    CTrace();
    CTrace(const CTrace &that);
    const CTrace &operator=(const CTrace &that);
};
/**************************************************/
/**************************************************/
/**************************************************/
/**$Author$                            **/
/**$Date$                  **/
/**$Revision$                              **/
/**************************************************/
/***************************************************
 * $Id$
 **************************************************/
