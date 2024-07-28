#ifndef __PROPERTY_H__
#define __PROPERTY_H__

template <typename T>
class Property {
public:
    ~Property() {}  //C++11: use override and =default;
    T& operator= (const T& f) { return value = f; }
    const T& operator() () const { return value; }
    operator const T& () const { return value; }
    T& operator->() { return value; }
protected:
    T value;
};

#if 0
template<typename T>
class Property
{
private:
   T& _value;

public:
   Property(void) :_value(NULL){ }
   Property(T& value) : _value(value) {}

   Property<T>& operator = (const T& val)
   {
      _value = val;
      return *this;
   };  // eo operator =

   T& operator->() const
   {
      return _value;
   }

   operator const T&() const
   {
      return _value;
   };  // eo operator ()
};
#endif

#endif // __PROPERTY_H__
