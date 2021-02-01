// Copyright (C) 2021 Seyyed Hossein Sajjadi
//
// This program is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free Software
// Foundation; either version 3 of the License, or (at your option) any later
// version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
// details.
//
// You should have received a copy of the GNU General Public License along with
// this program; if not, see <http://www.gnu.org/licenses/>.

#include <algorithm>
#include <vector>
#include <octave/oct.h>

namespace stats
{

template<typename Type>
  class value_iterator
  {
  public:

    using iterator_category = std::random_access_iterator_tag;
    using value_type = Type;
    using difference_type = std::ptrdiff_t;
    using pointer = Type*;
    using reference = Type&;

  public:

    value_iterator(Type* ptr = nullptr, octave_idx_type stride = 1, octave_idx_type counter = 0)
    : m_ptr (ptr), stride(stride), counter(counter) {}

    value_iterator(const value_iterator<Type>& it) = default;

    ~value_iterator(){}

    value_iterator<Type>& operator=(const value_iterator<Type>& it) = default;

    operator bool()const
    { return bool(m_ptr);}

    bool operator==(const value_iterator<Type>& it)const
    {return (counter == it.get_counter());}

    bool operator!=(const value_iterator<Type>& it)const
    {return (counter != it.get_counter());}

    value_iterator<Type>& operator+=(const difference_type& movement)
    {counter += movement;return (*this);}

    value_iterator<Type>& operator-=(const difference_type& movement)
    {counter -= movement;return (*this);}

    value_iterator<Type>& operator++()
    {counter ++;return (*this);}

    value_iterator<Type>& operator--()
    {counter --;return (*this);}

    value_iterator<Type> operator++(int)
    {auto temp(*this);counter ++;return temp;}

    value_iterator<Type> operator--(int)
    {auto temp(*this);counter --;return temp;}

    value_iterator<Type> operator+(const difference_type& movement)
    {auto counter_val = counter;counter+=movement;auto temp(*this);counter = counter_val;return temp;}

    value_iterator<Type> operator-(const difference_type& movement)
    {auto counter_val = counter;counter-=movement;auto temp(*this);counter = counter_val;return temp;}

    difference_type  operator-(const value_iterator<Type>& it)
    {return counter - it.get_counter();}

    reference operator*()
    {return *(m_ptr + (counter * stride));}

    const reference operator*()const
    {return *(m_ptr + (counter * stride));}

    pointer operator->()
    {return (m_ptr + (counter * stride));}

    octave_idx_type get_counter()const
    {return counter;}

    pointer get_ptr()const
    {return (m_ptr + (counter * stride));}

    const pointer get_const_ptr()const
    {return (m_ptr + (counter * stride));}

  protected:

    pointer m_ptr;
    const octave_idx_type stride;
    octave_idx_type counter;
  };

template<typename Type>
  class index_value_iterator
  {
  public:

    using iterator_category = std::random_access_iterator_tag;
    using value_type = std::pair <octave_idx_type, typename std::remove_cv<Type>::type>;
    using difference_type = std::ptrdiff_t;
    using pointer = Type*;
    using reference = std::pair <octave_idx_type, typename std::remove_cv<Type>::type>;

  public:

    index_value_iterator(Type* ptr = nullptr, octave_idx_type stride = 1, octave_idx_type counter = 0)
    : m_ptr (ptr), stride(stride), counter(counter) {}

    index_value_iterator(const index_value_iterator<Type>& it) = default;

    ~index_value_iterator(){}

    index_value_iterator<Type>& operator=(const index_value_iterator<Type>& it) = default;

    operator bool()const
    {return bool(m_ptr);}

    bool operator==(const index_value_iterator<Type>& it)const
    {return (counter == it.get_counter());}

    bool  operator!=(const index_value_iterator<Type>& it)const
    {return (counter != it.get_counter());}

    index_value_iterator<Type>& operator+=(const difference_type& movement)
    {counter += movement;return (*this);}

    index_value_iterator<Type>& operator-=(const difference_type& movement)
    {counter -= movement;return (*this);}

    index_value_iterator<Type>& operator++()
    {counter ++;return (*this);}

    index_value_iterator<Type>& operator--()
    {counter --;return (*this);}

    index_value_iterator<Type>  operator++(int)
    {auto temp(*this);counter ++;return temp;}

    index_value_iterator<Type>  operator--(int)
    {auto temp(*this);counter --;return temp;}

    index_value_iterator<Type>  operator+(const difference_type& movement)
    {auto counter_val = counter;counter+=movement;auto temp(*this);counter = counter_val;return temp;}

    index_value_iterator<Type>  operator-(const difference_type& movement)
    {auto counter_val = counter;counter-=movement;auto temp(*this);counter = counter_val;return temp;}

    difference_type operator-(const index_value_iterator<Type>& it)
    {return counter - it.get_counter();}

    reference operator*()
    {return {counter, *(m_ptr + (counter * stride))};}

    const reference operator*()const
    {return {counter, *(m_ptr + (counter * stride))};}

    pointer operator->()
    {return (m_ptr + (counter * stride));}

    octave_idx_type get_counter()const
    {return counter;}

    pointer get_ptr()const
    {return (m_ptr + (counter * stride));}

    const pointer get_const_ptr()const
    {return (m_ptr + (counter * stride));}

  protected:

    pointer m_ptr;
    const octave_idx_type stride;
    octave_idx_type counter;
  };

  class index_iterator
  {
  public:

    using iterator_category = std::random_access_iterator_tag;
    using value_type = octave_idx_type;
    using difference_type = octave_idx_type;
    using pointer = octave_idx_type*;
    using reference = octave_idx_type;

  public:

    index_iterator(octave_idx_type ptr = 0): counter (ptr) {}

    index_iterator(const index_iterator& it) = default;

    ~index_iterator(){}

    index_iterator& operator=(const index_iterator& it) = default;

    operator bool()const
    {return bool(counter);}

    bool operator==(const index_iterator& it)const
    {return (counter == it.get_counter());}

    bool operator!=(const index_iterator& it)const
    {return (counter != it.get_counter());}

    index_iterator& operator+=(const difference_type& movement)
    {counter += movement;return (*this);}

    index_iterator& operator-=(const difference_type& movement)
    {counter -= movement;return (*this);}

    index_iterator& operator++()
    {counter ++;return (*this);}

    index_iterator& operator--()
    {counter --;return (*this);}

    index_iterator operator++(int)
    {auto temp(*this);counter ++;return temp;}

    index_iterator operator--(int)
    {auto temp(*this);counter --;return temp;}

    index_iterator operator+(const difference_type& movement)
    {auto counter_val = counter;counter+=movement;auto temp(*this);counter = counter_val;return temp;}

    index_iterator operator-(const difference_type& movement)
    {auto counter_val = counter;counter-=movement;auto temp(*this);counter = counter_val;return temp;}

    difference_type operator-(const index_iterator& it)
    {return counter - it.get_counter();}

    reference operator*(){return counter;}

    const reference operator*()const{return counter;}

    pointer operator->(){return &counter;}

    octave_idx_type get_counter()const{return counter;}

  protected:

    octave_idx_type counter;
  };

  template <typename>
  struct is_pair : std::false_type
  { };

  template <typename T>
  struct is_pair<std::pair<octave_idx_type, T>> : std::true_type
  { };

  template <typename>
  struct is_complex : std::false_type
  { };

  template <typename T>
  struct is_complex<std::complex<T>> : std::true_type
  { };

  template <typename T>
  struct remove
  {
    using type = typename std::remove_cv<typename std::remove_pointer<typename std::iterator_traits<T>::pointer>::type>::type;
  };

  template <typename InpuIt,
  typename std::enable_if<
  ! (std::is_same<typename remove<InpuIt>::type, double>::value
  || std::is_same<typename remove<InpuIt>::type, float>::value
  || std::is_same<typename remove<InpuIt>::type, std::complex<double>>::value
  || std::is_same<typename remove<InpuIt>::type, std::complex<float>>::value)
  >::type* = nullptr>
  bool
  has_nan(InpuIt first, InpuIt last)
  {
    return false;
  }

  template <typename InpuIt,
  typename std::enable_if<
  std::is_same<typename remove<InpuIt>::type, double>::value
  || std::is_same<typename remove<InpuIt>::type, float>::value
  || std::is_same<typename remove<InpuIt>::type, std::complex<double>>::value
  || std::is_same<typename remove<InpuIt>::type, std::complex<float>>::value
  >::type* = nullptr>
  bool
  has_nan(InpuIt first, InpuIt last)
  {

    return std::find_if (first , last,
      [] (const typename  std::iterator_traits<InpuIt>::value_type& a)
      {
        return octave::math::isnan (a);
      }) != last;
  }

  template <typename InpuIt,
  typename std::enable_if<
    !(std::is_same<InpuIt, octave_int8>::value
  ||  std::is_same<InpuIt, octave_int16>::value
  ||  std::is_same<InpuIt, octave_int32>::value
  ||  std::is_same<InpuIt, octave_int64>::value
  ||  std::is_floating_point<InpuIt>::value
  ||  is_complex<InpuIt>::value)
  >::type* = nullptr>
  bool
  has_neg(InpuIt first, InpuIt last)
  {
    return false;
  }

  template <typename InpuIt,
  typename std::enable_if<
    is_complex<InpuIt>::value
  >::type* = nullptr>
  bool
  has_neg(InpuIt first, InpuIt last)
  {
    return true;
  }

  template <typename InpuIt,
  typename std::enable_if<
    std::is_same<InpuIt, octave_int8>::value
  ||  std::is_same<InpuIt, octave_int16>::value
  ||  std::is_same<InpuIt, octave_int32>::value
  ||  std::is_same<InpuIt, octave_int64>::value
  ||  std::is_floating_point<InpuIt>::value
  >::type* = nullptr>
  bool
  has_neg(InpuIt first, InpuIt last)
  {
    return std::find_if (first , last,
      [] (const typename  std::iterator_traits<InpuIt>::value_type& a)
      {
        return a < 0;
      }) != last;
  }

  struct comp_auto
  {
  template <typename Order>
    struct less_impl
    {
    template <typename T,
      typename std::enable_if<
       !( is_pair<T>::value
      ||  is_complex<T>::value)
      >::type* = nullptr>
      bool operator ()(const T& a, const T& b)
      {
        return a < b;
      }

      bool operator ()(const char& a, const char& b)
      {
        return static_cast<unsigned char>(a) < static_cast<unsigned char>(b);
      }

    template <typename Cmplx,
      typename std::enable_if<
      is_complex<Cmplx>::value
      >::type* = nullptr>
      bool operator ()(const Cmplx& a, const Cmplx& b)
      {
        return std::abs (a) < std::abs (b);
      }

    template <typename T,
      typename std::enable_if<
       ! is_complex<T>::value
      >::type* = nullptr>
      bool operator ()(const std::pair<octave_idx_type, T>& a, const std::pair<octave_idx_type, T>& b)
      {
        if (a.second < b.second)
          return true;

        if (b.second < a.second)
          return false;

        return Order{} (a.first , b.first);
      }

      bool operator ()(const std::pair<octave_idx_type, char>& a, const std::pair<octave_idx_type, char>& b)
      {
        if (static_cast<unsigned char>(a.second) < static_cast<unsigned char>(b.second))
          return true;

        if (static_cast<unsigned char>(b.second) < static_cast<unsigned char>(a.second))
          return false;

        return Order{} (a.first , b.first);
      }

    template <typename Cmplx,
      typename std::enable_if<
        is_complex<Cmplx>::value
      >::type* = nullptr>
      bool operator ()(const std::pair<octave_idx_type, Cmplx>& a, const std::pair<octave_idx_type, Cmplx>& b)
      {
        auto absa = std::abs (a.second);
        auto absb = std::abs (b.second);

        if (absa < absb)
          return true;

        if (absb < absa)
          return false;

        return Order{} (a.first , b.first);
      }
    };

  template <typename Order>
    struct less_nan_impl
    {
    template <typename Int,
      typename std::enable_if<
        !(std::is_floating_point<Int>::value
      ||  is_complex<Int>::value
      ||  is_pair<Int>::value )
      >::type* = nullptr>
      bool operator ()(const Int& a, const Int& b)
      {
        return a < b;
      }

    template <typename Float,
      typename std::enable_if<
      std::is_floating_point<Float>::value
      >::type* = nullptr>
      bool operator ()(const Float& a, const Float& b)
      {
        return a < b || octave::math::isnan (b);
      }

     template <typename T,
      typename std::enable_if<
        !(std::is_floating_point<T>::value
      ||  is_complex<T>::value)
      >::type* = nullptr>
      bool operator ()(const std::pair<octave_idx_type, T>& a, const std::pair<octave_idx_type, T>& b)
      {
        if (a.second < b.second)
          return true;

        if (b.second < a.second)
          return false;

        return Order{} (a.first , b.first);
      }

      bool operator ()(const std::pair<octave_idx_type, char>& a, const std::pair<octave_idx_type, char>& b)
      {
        if (static_cast<unsigned char>(a.second) < static_cast<unsigned char>(b.second))
          return true;

        if (static_cast<unsigned char>(b.second) < static_cast<unsigned char>(a.second))
          return false;

        return Order{} (a.first , b.first);
      }

    template <typename Float,
      typename std::enable_if<
      std::is_floating_point<Float>::value
      >::type* = nullptr>
      bool operator ()(const std::pair<octave_idx_type, Float>& a, const std::pair<octave_idx_type, Float>& b)
      {
        if (a.second < b.second)
          return true;

        if (b.second < a.second)
          return false;

        if (a.second == b.second)
          return Order{} (a.first , b.first);

        if (octave::math::isnan(b.second))
          {
            if ( octave::math::isnan(a.second))
              return Order{} (a.first , b.first);

            return true;
          }

        return false;
      }

    template <typename Cmplx,
      typename std::enable_if<
      is_complex<Cmplx>::value
      >::type* = nullptr>
      bool operator ()(const std::pair<octave_idx_type, Cmplx>& a, const std::pair<octave_idx_type, Cmplx>& b)
      {
        auto absa = std::abs (a.second);
        auto absb = std::abs (b.second);

        if (absa < absb)
          return true;

        if (absb < absa)
          return false;

        if (absa == absb)
          return Order{} (a.first , b.first);

        if (octave::math::isnan(absb))
          {
            if ( octave::math::isnan(absa))
              return Order{} (a.first , b.first);

            return true;
          }

        return false;
      }

    template <typename Cmplx,
      typename std::enable_if<
      is_complex<Cmplx>::value
      >::type* = nullptr>
      bool operator ()(const Cmplx& a, const Cmplx& b)
      {
        auto absa = std::abs (a);
        auto absb = std::abs (b);

        return absa < absb || octave::math::isnan (absb);
      }
    };

    struct equal
    {
    template <typename T,
      typename std::enable_if<
      ! is_complex<T>::value
      >::type* = nullptr>
      bool operator ()(const T& a, const T& b)
      {
        return a == b;
      }

    template <typename Cmplx,
      typename std::enable_if<
      is_complex<Cmplx>::value
      >::type* = nullptr>
      bool operator ()(const Cmplx& a, const Cmplx& b)
      {
        return std::abs (a) == std::abs (b);
      }
    };

    struct greater
    {
    template <typename T>
      bool operator () (const T& a, const T& b)
      {
        return less_impl<std::greater<octave_idx_type>>{} (b, a);
      }
    };

    struct greater_nan
    {
     template <typename T>
      bool operator () (const T& a, const T& b)
      {
        return less_nan_impl<std::greater<octave_idx_type>>{} (b, a);
      }
    };

    using less = less_impl<std::less<octave_idx_type>>;

    using less_nan = less_nan_impl<std::less<octave_idx_type>>;
  };

  struct comp_real
  {
  template <typename Order>
    struct less_impl
    {
    template <typename T,
      typename std::enable_if<
       ! is_complex<T>::value
       &&
       ! is_pair<T>::value
      >::type* = nullptr>
      bool operator ()(const T& a, const T& b)
      {
        return a < b;
      }

      bool operator ()(const char& a, const char& b)
      {
        return static_cast<unsigned char>(a) < static_cast<unsigned char>(b);
      }

    template <typename T,
      typename std::enable_if<
       ! is_complex<T>::value
      >::type* = nullptr>
      bool operator ()(const std::pair<octave_idx_type, T>& a, const std::pair<octave_idx_type, T>& b)
      {
        if (a.second < b.second)
          return true;

        if (b.second < a.second)
          return false;

        return Order{} (a.first , b.first);
      }

      bool operator ()(const std::pair<octave_idx_type, char>& a, const std::pair<octave_idx_type, char>& b)
      {
        if (static_cast<unsigned char>(a.second) < static_cast<unsigned char>(b.second))
          return true;

        if (static_cast<unsigned char>(b.second) < static_cast<unsigned char>(a.second))
          return false;

        return Order{} (a.first , b.first);
      }

    template <typename Cmplx,
      typename std::enable_if<
          is_complex<Cmplx>::value
      >::type* = nullptr>
      bool operator ()(const std::pair<octave_idx_type, Cmplx>& a, const std::pair<octave_idx_type, Cmplx>& b)
      {
        const auto reala = std::real (a.second);
        const auto realb = std::real (b.second);

        if (reala < realb)
          return true;

        if (realb < reala)
          return false;

        const auto imaga = std::imag (a.second);
        const auto imagb = std::imag (b.second);

        if (imaga < imagb)
          return true;

        if (imagb < imaga)
          return false;

        return Order{} (a.first , b.first);
      }

    template <typename Cmplx,
      typename std::enable_if<
          is_complex<Cmplx>::value
      >::type* = nullptr>
      bool operator ()(const Cmplx& a, const Cmplx& b)
      {
        const auto reala = std::real (a);
        const auto realb = std::real (b);

        if (reala < realb)
          return true;

        if (realb < reala)
          return false;

        return std::imag (a) < std::imag (b);
      }
    };

  template <typename Order>
    struct less_nan_impl
    {
    template <typename Int,
      typename std::enable_if<
        ! std::is_floating_point<Int>::value
        &&
        ! is_complex<Int>::value
        &&
        ! is_pair<Int>::value
      >::type* = nullptr>
      bool operator ()(const Int& a, const Int& b)
      {
        return a < b;
      }

      bool operator ()(const char& a, const char& b)
      {
        return static_cast<unsigned char>(a) < static_cast<unsigned char>(b);
      }

    template <typename Float,
      typename std::enable_if<
      std::is_floating_point<Float>::value
      >::type* = nullptr>
      bool operator ()(const Float& a, const Float& b)
      {
        return a < b || octave::math::isnan (b);
      }

    template <typename Cmplx,
      typename std::enable_if<
      is_complex<Cmplx>::value
      >::type* = nullptr>
      bool operator ()(const Cmplx& a, const Cmplx& b)
      {
        if (octave::math::isnan (b))
          return true;

        if (octave::math::isnan (a))
          return false;

        const auto reala = std::real (a);
        const auto realb = std::real (b);

        if (reala < realb)
          return true;

        if (realb < reala)
          return false;

        return std::imag (a) < std::imag (b);
      }

    template <typename T,
      typename std::enable_if<
        ! std::is_floating_point<T>::value
        &&
        ! is_complex<T>::value
      >::type* = nullptr>
      bool operator ()(const std::pair<octave_idx_type, T>& a, const std::pair<octave_idx_type, T>& b)
      {
        if (a.second < b.second)
          return true;

        if (b.second < a.second)
          return false;

        return Order{} (a.first , b.first);
      }

      bool operator ()(const std::pair<octave_idx_type, char>& a, const std::pair<octave_idx_type, char>& b)
      {

        if (static_cast<unsigned char>(a.second) < static_cast<unsigned char>(b.second))
          return true;

        if (static_cast<unsigned char>(b.second) < static_cast<unsigned char>(a.second))
          return false;

        return Order{} (a.first , b.first);

      }

    template <typename Float,
      typename std::enable_if<
      std::is_floating_point<Float>::value
      >::type* = nullptr>
      bool operator ()(const std::pair<octave_idx_type, Float>& a, const std::pair<octave_idx_type, Float>& b)
      {
        if (a.second < b.second)
          return true;

        if (b.second < a.second)
          return false;

        if (a.second == b.second)
          return Order{} (a.first , b.first);

        if (octave::math::isnan(b.second))
          {
            if ( octave::math::isnan(a.second))
              return Order{} (a.first , b.first);

            return true;
          }

        return false;
      }

    template <typename Cmplx,
      typename std::enable_if<
      is_complex<Cmplx>::value
      >::type* = nullptr>
      bool operator ()(const std::pair<octave_idx_type, Cmplx>& a, const std::pair<octave_idx_type, Cmplx>& b)
      {
        if (octave::math::isnan(b.second))
          {
            if ( octave::math::isnan(a.second))
              return Order{} (a.first , b.first);

            return true;
          }

        if (octave::math::isnan(a.second))
          {
            return false;
          }

        const auto reala = std::real (a.second);
        const auto realb = std::real (b.second);

        if (reala < realb)
          return true;

        if (realb < reala)
          return false;

        const auto imaga = std::imag (a.second);
        const auto imagb = std::imag (b.second);

        if (imaga < imagb)
          return true;

        if (imagb < imaga)
          return false;

        return Order{} (a.first , b.first);
      }
    };

    using less = less_impl<std::less<octave_idx_type>>;

    using less_nan = less_nan_impl<std::less<octave_idx_type>>;

    struct equal
    {
    template <typename T>
      bool operator ()(const T& a, const T& b)
      {
        return a == b;
      }
    };

    struct greater
    {
    template <typename T>
      bool operator () (const T& a, const T& b)
      {
        return less_impl<std::greater<octave_idx_type>>{} (b, a);
      }
    };

    struct greater_nan
    {
     template <typename T>
      bool operator () (const T& a, const T& b)
      {
        return less_nan_impl<std::greater<octave_idx_type>>{} (b, a);
      }
    };
  };

  struct comp_abs
  {
  template <typename Order>
    struct less_impl
    {
    template <typename T,
      typename std::enable_if<
        !(
          std::is_floating_point<T>::value
      ||  is_complex<T>::value
      ||  is_pair<T>::value
      ||  std::is_same<T, octave_int8>::value
      ||  std::is_same<T, octave_int16>::value
      ||  std::is_same<T, octave_int32>::value
      ||  std::is_same<T, octave_int64>::value)
      >::type* = nullptr>
      bool operator ()(const T& a, const T& b)
      {
        return a < b;
      }

      bool operator ()(const char& a, const char& b)
      {
        return static_cast<unsigned char>(a) < static_cast<unsigned char>(b);
      }

    template <typename Cmplx,
      typename std::enable_if<
          is_complex<Cmplx>::value
      >::type* = nullptr>
      bool operator ()(const Cmplx& a, const Cmplx& b)
      {
        auto absa = std::abs (a);

        auto absb = std::abs (b);

        if (absa < absb)
          return true;

        if (absb < absa)
          return false;

        return std::arg (a) < std::arg (b);
      }

    template <typename Val,
      typename std::enable_if<
          std::is_same<Val, octave_int8>::value
      ||  std::is_same<Val, octave_int16>::value
      ||  std::is_same<Val, octave_int32>::value
      ||  std::is_same<Val, octave_int64>::value
      ||  std::is_floating_point<Val>::value
      >::type* = nullptr>
      bool operator ()(const Val& a, const Val& b)
      {
        return abs (a) < abs (b);
      }

    template <typename T,
      typename std::enable_if<
        !(std::is_same<T, octave_int8>::value
      ||  std::is_same<T, octave_int16>::value
      ||  std::is_same<T, octave_int32>::value
      ||  std::is_same<T, octave_int64>::value
      ||  std::is_floating_point<T>::value
      ||  is_complex<T>::value)
      >::type* = nullptr>
      bool operator ()(const std::pair<octave_idx_type, T>& a, const std::pair<octave_idx_type, T>& b)
      {
        if (a.second < b.second)
          return true;

        if (b.second < a.second)
          return false;

        return Order{} (a.first , b.first);
      }

      bool operator ()(const std::pair<octave_idx_type, char>& a, const std::pair<octave_idx_type, char>& b)
      {
        if (static_cast<unsigned char>(a.second) < static_cast<unsigned char>(b.second))
          return true;

        if (static_cast<unsigned char>(b.second) < static_cast<unsigned char>(a.second))
          return false;

        return Order{} (a.first , b.first);
      }

    template <typename Val,
      typename std::enable_if<
          std::is_same<Val, octave_int8>::value
      ||  std::is_same<Val, octave_int16>::value
      ||  std::is_same<Val, octave_int32>::value
      ||  std::is_same<Val, octave_int64>::value
      ||  std::is_floating_point<Val>::value
      >::type* = nullptr>
      bool operator ()(const std::pair<octave_idx_type, Val>& a, const std::pair<octave_idx_type, Val>& b)
      {
        auto absa = abs (a.second);
        auto absb = abs (b.second);

        if (absa < absb)
          return true;

        if (absb < absa)
          return false;

        return Order{} (a.first , b.first);
      }

    template <typename Cmplx,
      typename std::enable_if<
      is_complex<Cmplx>::value
      >::type* = nullptr>
      bool operator ()(const std::pair<octave_idx_type, Cmplx>& a, const std::pair<octave_idx_type, Cmplx>& b)
      {
        auto absa = std::abs (a.second);
        auto absb = std::abs (b.second);

        if (absa < absb)
          return true;

        if (absb < absa)
          return false;

        auto arga = std::arg (a.second);
        auto argb = std::arg (b.second);

        if (arga < argb)
          return true;

        if (argb < arga)
          return false;

        return Order{} (a.first , b.first);
      }
    };

  template <typename Order>
    struct less_nan_impl
    {
    template <typename T,
      typename std::enable_if<
        !(is_complex<T>::value
      ||  is_pair<T>::value
      ||  std::is_same<T, octave_int8>::value
      ||  std::is_same<T, octave_int16>::value
      ||  std::is_same<T, octave_int32>::value
      ||  std::is_same<T, octave_int64>::value
      ||  std::is_floating_point<T>::value)
      >::type* = nullptr>
      bool operator ()(const T& a, const T& b)
      {
        return a < b;
      }

      bool operator ()(const char& a, const char& b)
      {
        return static_cast<unsigned char>(a) < static_cast<unsigned char>(b);
      }

    template <typename Cmplx,
      typename std::enable_if<
      is_complex<Cmplx>::value
      >::type* = nullptr>
      bool operator ()(const Cmplx& a, const Cmplx& b)
      {
        if (octave::math::isnan (b))
          return true;

        auto absa = std::abs (a);

        auto absb = std::abs (b);

        if (absa < absb)
          return true;

        if (absa == absb)
          return std::arg (a) < std::arg (b);

        return false;
      }

    template <typename Val,
      typename std::enable_if<
          std::is_same<Val, octave_int8>::value
      ||  std::is_same<Val, octave_int16>::value
      ||  std::is_same<Val, octave_int32>::value
      ||  std::is_same<Val, octave_int64>::value
      >::type* = nullptr>
      bool operator ()(const Val& a, const Val& b)
      {
        return abs (a) < abs (b);
      }

    template <typename Val,
      typename std::enable_if<
       std::is_floating_point<Val>::value
      >::type* = nullptr>
      bool operator ()(const Val& a, const Val& b)
      {
        return abs (a) < abs (b) || octave::math::isnan (b);
      }

    template <typename T,
      typename std::enable_if<
        !(std::is_same<T, octave_int8>::value
      ||  std::is_same<T, octave_int16>::value
      ||  std::is_same<T, octave_int32>::value
      ||  std::is_same<T, octave_int64>::value
      ||  std::is_floating_point<T>::value
      ||  is_complex<T>::value)
      >::type* = nullptr>
      bool operator ()(const std::pair<octave_idx_type, T>& a, const std::pair<octave_idx_type, T>& b)
      {
        if (a.second < b.second)
          return true;

        if (b.second < a.second)
          return false;

        return Order{} (a.first , b.first);
      }

      bool operator ()(const std::pair<octave_idx_type, char>& a, const std::pair<octave_idx_type, char>& b)
      {
        if (static_cast<unsigned char>(a.second) < static_cast<unsigned char>(b.second))
          return true;

        if (static_cast<unsigned char>(b.second) < static_cast<unsigned char>(a.second))
          return false;

        return Order{} (a.first , b.first);
      }

    template <typename Val,
      typename std::enable_if<
          std::is_same<Val, octave_int8>::value
      ||  std::is_same<Val, octave_int16>::value
      ||  std::is_same<Val, octave_int32>::value
      ||  std::is_same<Val, octave_int64>::value
      >::type* = nullptr>
      bool operator ()(const std::pair<octave_idx_type, Val>& a, const std::pair<octave_idx_type, Val>& b)
      {
        auto absa = abs (a.second);
        auto absb = abs (b.second);

        if (absa < absb)
          return true;

        if (absb < absa)
          return false;

        return Order{} (a.first , b.first);
      }

    template <typename Float,
      typename std::enable_if<
       std::is_floating_point<Float>::value
      >::type* = nullptr>
      bool operator ()(const std::pair<octave_idx_type, Float>& a, const std::pair<octave_idx_type, Float>& b)
      {
        auto absa = abs (a.second);
        auto absb = abs (b.second);

        if (absa < absb)
          return true;

        if (absb < absa)
          return false;

        if (absa == absb)
          return Order{} (a.first , b.first);

        if (octave::math::isnan(b.second))
          {
            if ( octave::math::isnan(a.second))
              return Order{} (a.first , b.first);

            return true;
          }

        return false;
      }

    template <typename Cmplx,
      typename std::enable_if<
          is_complex<Cmplx>::value
      >::type* = nullptr>
      bool operator ()(const std::pair<octave_idx_type, Cmplx>& a, const std::pair<octave_idx_type, Cmplx>& b)
      {
        if (octave::math::isnan(b.second))
          {
            if ( octave::math::isnan(a.second))
              return Order{} (a.first , b.first);

            return true;
          }

        if (octave::math::isnan(a.second))
          {
            return false;
          }

        const auto absa = std::abs (a.second);
        const auto absb = std::abs (b.second);

        if (absa < absb)
          return true;

        if (absb < absa)
          return false;

        const auto arga = std::arg (a.second);
        const auto argb = std::arg (b.second);

        if (arga < argb)
          return true;

        if (argb < arga)
          return false;

        return Order{} (a.first , b.first);
      }
    };

    using less = less_impl<std::less<octave_idx_type>>;

    using less_nan = less_nan_impl<std::less<octave_idx_type>>;

    struct equal
    {
    template <typename T,
      typename std::enable_if<
        !(std::is_same<T, octave_int8>::value
      ||  std::is_same<T, octave_int16>::value
      ||  std::is_same<T, octave_int32>::value
      ||  std::is_same<T, octave_int64>::value
      ||  std::is_floating_point<T>::value
      ||  is_complex<T>::value)
      >::type* = nullptr>
      bool operator ()(const T& a, const T& b)
      {
        return a == b;
      }

    template <typename Val,
      typename std::enable_if<
          std::is_same<Val, octave_int8>::value
      ||  std::is_same<Val, octave_int16>::value
      ||  std::is_same<Val, octave_int32>::value
      ||  std::is_same<Val, octave_int64>::value
      ||  std::is_floating_point<Val>::value
      >::type* = nullptr>
      bool operator ()(const Val& a, const Val& b)
      {
        return abs (a) == abs (b);
      }

    template <typename Cmplx,
      typename std::enable_if<
          is_complex<Cmplx>::value
      >::type* = nullptr>
      bool operator ()(const Cmplx& a, const Cmplx& b)
      {
        return std::abs (a) == std::abs (b) && std::arg (a) == std::arg (b);
      }
    };

    struct greater
    {
    template <typename T>
      bool operator () (const T& a, const T& b)
      {
        return less_impl<std::greater<octave_idx_type>>{} (b, a);
      }
    };

    struct greater_nan
    {
     template <typename T>
      bool operator () (const T& a, const T& b)
      {
        return less_nan_impl<std::greater<octave_idx_type>>{} (b, a);
      }
    };
  };

  struct start_elem_indexer
  {
    start_elem_indexer (const dim_vector& dims, octave_idx_type k , octave_idx_type dim)
    : dims(dims),bounds (), indexes(dim_vector(1,dims.length()))
    {
      bounds.reserve(dims.length());
      for (octave_idx_type i = 0 ;i < dims.length(); i++)
        {
          bounds.emplace_back (0, dims(i));
        }

      bounds[dim] = {0, 1};

      for (octave_idx_type s = 0 ;s < dims.length(); s++)
        {
          indexes.xelem (s) = bounds[s].first;
        }
    }

    octave_idx_type operator++ (int)
    {
      octave_idx_type result = compute_index(indexes,dims);

      for (int i = 0; i < dims.length(); i++)
        {
          if (++indexes.xelem(i) == bounds[i].second)
            {
              indexes.xelem(i) = bounds[i].first;
            }
          else
            {
              break;
            }
        }

      return result;
    }

    dim_vector dims;

    std::vector<std::pair<octave_idx_type,octave_idx_type>> bounds;

    Array<octave_idx_type> indexes;
  };

  octave_idx_type
  compute_offset (const dim_vector& dims, octave_idx_type dim)
  {
    octave_idx_type offset = 1;

    for (octave_idx_type i = 0; i < dim ; ++i)
      offset *= dims(i);

    return offset;
  }

  template <typename Comp>
  octave_value_list
  do_maxk (const Range& input, octave_idx_type k , octave_idx_type dim, bool compute_index)
  {
    auto base = input.base ();
    auto inc = input.inc ();
    auto num = input.numel ();

    if (dim > 1)
      {
        if (k == 0)
          {
            dim_vector dims_output (num>0, num);

            dims_output.resize (dim + 1, 1);

            dims_output (dim) = k;

            octave_value_list retval {octave_value (NDArray (dims_output))};

            if (compute_index)
              retval.append (octave_value (NDArray (dims_output)));

            return retval;
          }

        octave_value_list retval {octave_value (input)};

        if (compute_index)
          retval.append (octave_value (NDArray (dim_vector (num>0, num), 1)));

        return retval;
      }

    if (k == 0)
      {
        dim_vector dims_output (1, num);

        dims_output (dim) = k;

        octave_value_list retval {octave_value (NDArray (dims_output))};

        if (compute_index)
          retval.append (octave_value (NDArray (dims_output)));

        return retval;
      }

    if (num == 0)
      {
        octave_value_list retval {octave_value (input)};

        if (compute_index)
          retval.append (octave_value (NDArray(dim_vector(num>0, num))));

        return retval;
      }

    if (num == 1)
      {
        octave_value_list retval {octave_value (input)};

        if (compute_index)
          {
              retval.append (octave_value (1.0));
          }

        return retval;
      }

    k = std::min (k, num);

    if (dim == 0)
      {
        octave_value_list retval {octave_value (input)};

        if (compute_index)
          retval.append (octave_value (NDArray (dim_vector (num>0, num), 1.0)));

        return retval;
      }

    if (std::is_same<Comp,comp_abs>::value)
      {
        if ((base < 0 && input.elem(num-1) > 0) || (base > 0 && input.elem(num-1) < 0))
          {
            index_iterator it {0};
            NDArray output(dim_vector (1, k));
            NDArray second_output;
            auto* output_data = output.fortran_vec ();

            if (!compute_index)
              {
                std::partial_sort_copy (it, it+num, output_data, output_data+k,
                  [&input] (octave_idx_type a, octave_idx_type b)
                  {
                    auto A = std::make_pair(a, input.elem(a));
                    auto B = std::make_pair(b, input.elem(b));
                    return typename Comp::greater{} (A, B);
                  }
                );

                for (octave_idx_type s = 0; s < k; s++)
                  {
                    output_data[s] = input.elem((octave_idx_type)output_data[s]);
                  }
              }
            else
              {
                second_output = NDArray (dim_vector (1, k));

                auto * second_output_data = second_output.fortran_vec ();

                std::partial_sort_copy (it, it+num, second_output_data, second_output_data + k,
                  [&input] (octave_idx_type a, octave_idx_type b)
                  {
                    auto A = std::make_pair(a, input.elem(a));
                    auto B = std::make_pair(b, input.elem(b));
                    return typename Comp::greater{} (A, B);
                  }
                );

                for (octave_idx_type s = 0; s < k; s++)
                  {
                    output_data[s] = input.elem((octave_idx_type)second_output_data[s]);
                    ++second_output_data[s];
                  }
              }
            if (!compute_index)
              {
                return ovl (octave_value (output));
              }
            else
              {
                return ovl (octave_value (output), octave_value (second_output));
              }
          }

        if (base < 0 || input.elem(num-1) < 0)
          {
            if (inc < 0)
              {
                octave_value_list retval {octave_value (Range (input.elem(num-1),input.elem(num-k),-inc))};

                if (compute_index)
                  {
                    retval.append (octave_value (Range (num, num - k + 1, -1.0)));
                  }

                return retval;
              }

            if (inc > 0)
              {
                octave_value_list retval {octave_value (Range (base,input.elem(k-1),inc))};

                if (compute_index)
                  {
                    retval.append (octave_value (Range (1.0, k, 1.0)));
                  }

                return retval;
              }
          }
      }

    if (inc < 0)
      {
        octave_value_list retval {octave_value (Range (base,input.elem(k-1),inc))};

        if (compute_index)
          {
            retval.append (octave_value (Range (1.0, k, 1.0)));
          }

        return retval;

      }
    else if (inc > 0)
      {
        octave_value_list retval {octave_value (Range (input.elem(num-1),input.elem(num-k),-inc))};

        if (compute_index)
          {
            retval.append (octave_value (Range (num, num - k + 1, -1.0)));
          }

        return retval;
      }

    return octave_value_list ();
  }

  template <typename Comp, typename sparse_matrix>
  octave_value_list
  do_maxk_sparse (const sparse_matrix& input_matrix, octave_idx_type k , octave_idx_type dim, bool compute_index)
  {
    auto dims_input = input_matrix.dims ();

    if ((dim + 1) > 2)
      {
        octave_value_list retval;

        if (k == 0)
          {
            dim_vector dims = dims_input;

            dims.resize (dim + 1, 1);

            dims(dim) = k;

            retval.append (octave_value (Array <typename sparse_matrix::element_type> (dims)));

            if (compute_index)
              {
                retval.append (octave_value (NDArray (dims)));
              }
          }
        else
          {
            retval.append (octave_value (input_matrix));

            if (compute_index)
              retval.append (octave_value (NDArray (dims_input, 1)));
          }

        return retval;
      }

    if (k == 0)
      {
        octave_value_list retval;

        if (dim == 0)
          {
            retval.append (octave_value(sparse_matrix (k, input_matrix.cols ())));

            if (compute_index)
              retval.append (octave_value(NDArray (dim_vector (k, input_matrix.cols ()))));
          }
        else if (dim == 1)
          {
            retval.append (octave_value(sparse_matrix (input_matrix.rows (), k)));

            if (compute_index)
              retval.append (octave_value(NDArray (dim_vector (input_matrix.rows (), k))));
          }

        return retval;
      }

    if (input_matrix.numel () == 0)
      {
        auto dims_output = dims_input;

        dims_output (dim) = std::min (k, dims_input (dim));

        octave_value_list retval {octave_value (input_matrix.reshape (dims_output))};

        if (compute_index)
          retval.append (octave_value (NDArray (dims_output)));

        return retval;
      }

    if (dim > 1 || dims_input (dim) == 1 )
      {
        octave_value_list retval {octave_value (input_matrix)};

        if (compute_index)
          {
            retval.append (octave_value (NDArray (dims_input,1)));
          }

        return retval;
      }


    sparse_matrix input = input_matrix;

    bool should_transpose = false;

    if (dim == 1)
      {
        should_transpose = true;

        input = input_matrix.transpose ();
      }

    const octave_idx_type * cidx = input.cidx ();
    const octave_idx_type * ridx = input.ridx ();
    const typename sparse_matrix::element_type * idata = input.data ();

    auto cols = input.cols ();
    auto rows = input.rows ();

    k = std::min (k , rows);

    sparse_matrix output (k, cols);

    octave_idx_type * ocidx = output.xcidx ();

    std::vector <octave_idx_type> positives;

    positives.reserve (cols);

    octave_idx_type nout = 0;

    typename sparse_matrix::element_type zero {0};

    for (octave_idx_type i = 0 ; i < cols; i++)
      {
        auto n = cidx[i+1] - cidx[i];

        octave_idx_type zer = rows - n;
        octave_idx_type pos = 0;

        for (octave_idx_type j = cidx[i] ; j < cidx[i+1]; j++)
          {
            if (! typename Comp::less {} (idata[j], zero))
              pos++;
          }

        positives.push_back (pos);

        ocidx[i] = nout;

        if (pos >= k)
          {
            nout += k;
          }
        else
          {
            auto nonnegative = pos + zer;

            if (nonnegative >= k)
              {
                nout += pos;
              }
            else
              nout += k - zer;
          }
      }

    ocidx[cols] = nout;

    output.change_capacity(nout);

    octave_idx_type * oridx = output.xridx ();

    typename sparse_matrix::element_type * odata = output.xdata ();

    octave_idx_type j = 0;

    if (! compute_index)
      for (octave_idx_type i = 0 ; i < cols; i++)
        {
          auto no = ocidx[i+1] - ocidx[i];

          auto pos = std::min(positives[i], no);

          auto zer = k - no;

          if (! has_nan (idata + cidx[i], idata + cidx [i+1]))
            std::partial_sort_copy (idata + cidx[i], idata + cidx [i+1], odata + j, odata + j + no, typename Comp::greater {});
          else
            std::partial_sort_copy (idata + cidx[i], idata + cidx [i+1], odata + j, odata + j + no, typename Comp::greater_nan {});

          for (octave_idx_type s = 0; s < pos;s++)
            oridx[j++] = s;

          for (octave_idx_type s = pos+zer; s < k;s++)
            oridx[j++] = s;

          OCTAVE_QUIT;
        }

    NDArray second_output;

    if (compute_index)
      {
        auto idata_col = idata;
        auto ridx_col = ridx;
        if (should_transpose)
          second_output = NDArray(dim_vector(cols , k));
        else
          second_output = NDArray(dim_vector(k , cols));

        double * second_data = second_output.fortran_vec ();

        const octave_idx_type offset = should_transpose ? cols : 1;
        const octave_idx_type next_block = should_transpose ? 1 : k;

        for (octave_idx_type i = 0 ; i < cols; i++)
          {
            const auto no = ocidx[i+1] - ocidx[i];
            const auto ni = cidx[i+1] - cidx[i];
            const auto pos = std::min(positives[i], no);
            const auto zer = k - no;

            const auto idx_bound = (zer+pos)*offset;

            index_iterator colit (0);
            if (! has_nan (idata_col , idata_col + ni))
              std::partial_sort_copy (colit, colit+ni, oridx + j, oridx + j + no,
                [&idata_col] (octave_idx_type a, octave_idx_type b)
                {
                  auto A = std::make_pair(a, idata_col[a]);
                  auto B = std::make_pair(b, idata_col[b]);
                  return typename Comp::greater {} (A, B);
                }
              );
            else
              std::partial_sort_copy (colit, colit+ni, oridx + j, oridx + j + no,
                [&idata_col] (octave_idx_type a, octave_idx_type b)
                {
                  auto A = std::make_pair(a, idata_col[a]);
                  auto B = std::make_pair(b, idata_col[b]);
                  return typename Comp::greater_nan {} (A, B);
                }
              );

            octave_idx_type ct = 0;

            for (octave_idx_type s = 0; s < pos;s++)
              {
                odata[j] = idata_col[oridx[j]];
                second_data[ct] = ridx_col[oridx[j]]+1;
                oridx[j++] = s;
                ct += offset;
              }

            if (ni > 0)
              for (octave_idx_type s = 0; s < ridx_col[0] && ct < idx_bound; s++)
                {
                  second_data[ct] = s+1;
                  ct += offset;
                }
            else
              for (octave_idx_type s = 0; s < k && ct < idx_bound; s++)
                {
                  second_data[ct] = s+1;
                  ct += offset;
                }

            for (octave_idx_type s = 0; s < ni-1; s++)
              {
                for (octave_idx_type w = ridx_col[s] + 1; w < ridx_col[s+1] && ct < idx_bound; w++)
                  {
                    second_data[ct] = w+1;
                    ct += offset;
                  }
              }

            if (ni > 0)
              for (octave_idx_type s = ridx_col[ni-1] + 1; s < rows && ct < idx_bound; s++)
                {
                  second_data[ct] = s+1;
                  ct += offset;
                }

            for (octave_idx_type s = pos+zer; s < k;s++)
              {
                odata[j] = idata_col[oridx[j]];
                second_data[ct] = ridx_col[oridx[j]]+1;
                oridx[j++] = s;
                ct += offset;
              }

            second_data += next_block;
            idata_col += ni;
            ridx_col += ni;

            OCTAVE_QUIT;
          }
      }

    assert (j == nout);

    input = sparse_matrix ();

    octave_value_list retval (octave_value(should_transpose ? output.transpose () : output));

    if (compute_index)
      retval.append (second_output);

    return retval;
  }

  template <typename Comp>
  octave_value_list
  do_maxk(const SparseMatrix& input_matrix, octave_idx_type k , octave_idx_type dim, bool compute_index)
  {
    return do_maxk_sparse <Comp>(input_matrix, k, dim, compute_index);
  }

  template <typename Comp>
  octave_value_list
  do_maxk(const SparseComplexMatrix& input_matrix, octave_idx_type k , octave_idx_type dim, bool compute_index)
  {
    return do_maxk_sparse <Comp>(input_matrix, k, dim, compute_index);
  }

  template <typename Comp>
  octave_value_list
  do_maxk(const SparseBoolMatrix& input_matrix, octave_idx_type k , octave_idx_type dim, bool compute_index)
  {
    auto dims_input = input_matrix.dims ();

    if ((dim + 1) > 2)
      {
        octave_value_list retval;

        if (k == 0)
          {
            dim_vector dims = dims_input;

            dims.resize (dim + 1, 1);

            dims(dim) = k;

            retval.append (octave_value (Array <bool> (dims)));

            if (compute_index)
              {
                retval.append (octave_value (NDArray (dims)));
              }
          }
        else
          {
            retval.append (octave_value (input_matrix));

            if (compute_index)
              retval.append (octave_value (NDArray (dims_input, 1)));
          }

        return retval;
      }

    if (k == 0)
      {
        octave_value_list retval;

        if (dim == 0)
          {
            retval.append (octave_value(SparseBoolMatrix (k, input_matrix.cols ())));

            if (compute_index)
              retval.append (octave_value(NDArray (dim_vector (k, input_matrix.cols ()))));
          }
        else if (dim == 1)
          {
            retval.append (octave_value(SparseBoolMatrix (input_matrix.rows (), k)));

            if (compute_index)
              retval.append (octave_value(NDArray (dim_vector (input_matrix.rows (), k))));
          }

        return retval;
      }

    if (input_matrix.numel () == 0)
      {
        auto dims_output = dims_input;

        dims_output (dim) = std::min (k, dims_input (dim));

        octave_value_list retval {octave_value (input_matrix.reshape (dims_output))};

        if (compute_index)
          retval.append (octave_value (NDArray (dims_output)));

        return retval;
      }

    if (dim > 1 || dims_input (dim) == 1)
      {
        octave_value_list retval {octave_value (input_matrix)};

        if (compute_index)
          {
            retval.append (octave_value (NDArray (dims_input,1)));
          }

        return retval;
      }

    SparseBoolMatrix input = input_matrix;

    bool should_transpose = false;

    if (dim == 1)
      {
        should_transpose = true;

        input = input_matrix.transpose ();

        dim = 0;
      }
    else if (dim != 0)
      error ("dim is greater than matrix dimensions");

    const octave_idx_type * cidx = input.cidx ();
    const octave_idx_type * ridx = input.ridx ();

    auto cols = input.cols ();
    auto rows = input.rows ();

    k = std::min (k, rows);

    SparseBoolMatrix output (k, cols);

    octave_idx_type * ocidx = output.xcidx ();

    octave_idx_type nout = 0;

    for (octave_idx_type i = 0 ; i < cols; i++)
      {
        auto n = cidx[i+1] - cidx[i];

        ocidx[i] = nout;

        nout += std::min (k , n);
      }

    ocidx[cols] = nout;

    output.change_capacity(nout);

    octave_idx_type * oridx = output.xridx ();

    typename SparseBoolMatrix::element_type * odata = output.xdata ();

    std::fill_n (odata, nout, true);

    octave_idx_type j = 0;

    if (! compute_index)
      for (octave_idx_type i = 0 ; i < cols; i++)
        {
          auto no = ocidx[i+1] - ocidx[i];

          for (octave_idx_type s = 0; s < no;s++)
            {
              oridx[j++] = s;
            }
        }

    NDArray second_output;

    if (compute_index)
      {
        auto ridx_col = ridx;

        if (should_transpose)
          second_output = NDArray(dim_vector(rows , k));
        else
          second_output = NDArray(dim_vector(k , cols));

        double * second_data = second_output.fortran_vec ();

        const octave_idx_type offset = should_transpose ? rows : 1;

        const octave_idx_type next_block = should_transpose ? 1 : k;

        for (octave_idx_type i = 0 ; i < cols; i++)
          {
            const auto no = ocidx[i+1] - ocidx[i];

            const auto ni = cidx[i+1] - cidx[i];

            const auto idx_bound = k * offset;

            octave_idx_type ct = 0;

            for (octave_idx_type s = 0; s < no;s++)
              {
                second_data[ct] = ridx_col[s]+1;
                oridx[j++] = s;
                ct += offset;
              }

            if (ni > 0)
              for (octave_idx_type s = 0; s < ridx_col[0] && ct < idx_bound; s++)
                {
                  second_data[ct] = s+1;
                  ct += offset;
                }
            else
              for (octave_idx_type s = 0; s < k && ct < idx_bound; s++)
                {
                  second_data[ct] = s+1;
                  ct += offset;
                }

            for (octave_idx_type s = 0; s < ni-1; s++)
              {
                for (octave_idx_type w = ridx_col[s] + 1; w < ridx_col[s+1] && ct < idx_bound; w++)
                  {
                    second_data[ct] = w+1;
                    ct += offset;

                  }
              }

            if (ni > 0)
              for (octave_idx_type s = ridx_col[ni-1] + 1; s < rows && ct < idx_bound; s++)
                {
                  second_data[ct] = s+1;
                  ct += offset;
                }

            second_data += next_block;
            ridx_col += ni;

            OCTAVE_QUIT;
          }
      }


    if (should_transpose)
      input = SparseBoolMatrix ();

    octave_value_list retval {octave_value (should_transpose ? output.transpose () : output)};

    if (compute_index)
      retval.append (second_output);

    return retval;
  }

template <typename Comp>
  octave_value_list
  do_maxk (const PermMatrix& input, octave_idx_type k , octave_idx_type dim, bool compute_index)
  {
    dim_vector dims = input.dims ();

    if (dim > 1)
      {
        if (k == 0)
          {
            auto dims_output = dims;

            dims_output.resize (dim + 1, 1);

            dims_output (dim) = k;

            octave_value_list retval;

            retval.append (octave_value (NDArray (dims_output)))  ;

            if (compute_index)
              retval.append (octave_value (NDArray (dims_output)))  ;

            return retval;
          }

        octave_value_list retval {octave_value (input)};

        if (compute_index)
          retval.append (octave_value (Matrix (dims,1)));

        return retval;
      }

    if (k == 0)
      {
        auto dims_output = dims;

        dims_output (dim) = k;

        octave_value_list retval;

        retval.append (octave_value (NDArray (dims_output)))  ;

        if (compute_index)
          retval.append (octave_value (NDArray (dims_output)))  ;

        return retval;
      }

    auto cols = dims(1);

    auto rows = dims(0);

    k = std::min(k, dims(dim));

    dims(dim) = k;

    Matrix output (dims, 0);

    double* data = output.fortran_vec ();

    if (dim == 0)
      for (octave_idx_type i = 0; i < output.numel (); i+=k)
        data[i] = 1;
    else if (dim == 1)
      for (octave_idx_type i = 0; i < rows; i++)
        data[i] = 1;

    octave_value_list  retval {octave_value(output)};

    if (compute_index)
      {
        NDArray second_output = NDArray (dims);

        auto* second_data = second_output.fortran_vec ();

        if (dim == 0 )
          {
            const octave_idx_type* idata = input.col_perm_vec ().fortran_vec ();

            for (octave_idx_type c = 0 ; c < cols; c++)
              {
                const octave_idx_type rownum = idata[c];

                *second_data++ = rownum+1;

                for (octave_idx_type r = 0 ; r < std::min (rownum, k-1); r++)
                  *second_data++ = r+1;

                for (octave_idx_type r = std::min (rownum, k-1) + 1 ; r < k; r++)
                  *second_data++ = r+1;
              }
          }
        else
          {
            auto tr = input.transpose ();

            const octave_idx_type* idata = tr.col_perm_vec ().fortran_vec ();

            for (octave_idx_type r = 0 ; r < rows; r++)
              {
                *second_data++ = idata[r] + 1;
              }

            for (octave_idx_type c = 1 ; c < k; c++)
              {
                for (octave_idx_type r = 0 ; r < rows; r++)
                  {
                    if (c <= idata[r])
                      *second_data++ = c;
                    else
                      *second_data++ = c+1;
                  }
              }
          }

        retval.append (second_output);
      }

    return retval;
  }

template <typename Comp, typename diag_matrix>
  octave_value_list
  do_maxk_diag (const diag_matrix& input, octave_idx_type k , octave_idx_type dim, bool compute_index)
  {
    dim_vector dims = input.dims ();

    if (dim > 1)
      {
        if (k == 0)
          {
            auto dims_output = dims;

            dims_output.resize (dim + 1, 1);

            dims_output (dim) = k;

            octave_value_list retval;

            retval.append (octave_value (typename diag_matrix::full_matrix_type (dims_output)))  ;

            if (compute_index)
              retval.append (octave_value (NDArray (dims_output)))  ;

            return retval;
          }

        octave_value_list retval {octave_value (input)};

        if (compute_index)
          retval.append (octave_value (Matrix (dims,1)));

        return retval;
      }

    if (k == 0)
      {
        auto dims_output = dims;

        dims_output (dim) = k;

        octave_value_list retval;

        retval.append (octave_value (typename diag_matrix::full_matrix_type (dims_output)))  ;

        if (compute_index)
          retval.append (octave_value (NDArray (dims_output)))  ;

        return retval;
      }

    const bool full_sort =  dims(dim) == k;

    k = std::min (dims(dim), k);

    dims(dim) = k;

    typename diag_matrix::full_matrix_type output (dims, 0);

    const typename diag_matrix::element_type* idata = input.fortran_vec ();

    typename diag_matrix::element_type* odata = output.fortran_vec ();

    octave_idx_type j = 0;

    const bool contains_nan = has_nan (idata, idata + input.length ());

    typename diag_matrix::element_type zero {0};

    if (! compute_index)
      {
        if (dim == 0)
          {
            const auto offset = k - 1;

            for (octave_idx_type i = 0; i < output.numel (); i+=k)
              {
                auto dat = idata[j];

                bool is_pos = contains_nan ? typename Comp::greater_nan {} (dat, zero) : typename Comp::greater {} (dat, zero);

                if (is_pos)
                  odata[i] = dat;
                else if (full_sort)
                  odata[i + offset] = dat;

                j++;
              }
          }
        else if (dim == 1)
          {
            const auto offset = output.numel () - k;

            for (octave_idx_type i = 0; i < dims (0); i++)
              {
                auto dat = idata[i];
                bool is_pos = contains_nan ? typename Comp::greater_nan {} (dat, zero) : typename Comp::greater {} (dat, zero);

                if (is_pos)
                  odata[i] = dat;
                else if (full_sort)
                  odata[i + offset] = dat;
              }
          }

        return octave_value (output);
      }

    if (compute_index)
      {
        NDArray second_output(dims);

        auto *  second_data = second_output.fortran_vec ();

        if (dim == 0)
          {
            const auto offset = k - 1;

            for (octave_idx_type i = 0; i < output.numel (); i+=k)
              {
                auto dat = idata[j];

                bool is_pos = contains_nan ? typename Comp::greater_nan {} (dat, zero) : typename Comp::greater {} (dat, zero);

                if (is_pos)
                  {
                    *second_data++ = j + 1;

                    for (octave_idx_type s = 0; s < k-1; s++)
                      {
                        *second_data++ = s + (s < j ? 1 : 2);
                      }

                    odata[i] = dat;
                  }
                else if (typename Comp::equal {} (dat, zero))
                  {
                    for (octave_idx_type s = 0; s < k ; s++)
                      *second_data++ = s + 1;
                  }
                else
                  {
                    for (octave_idx_type s = 0; s < k; s++)
                      *second_data++ = s + (s < j ? 1 : 2);

                    if (full_sort)
                      {
                        *(second_data-1) = j + 1;

                        odata[i + offset] = dat;
                      }
                  }
                j++;
              }
          }
        else if (dim == 1)
          {
            const auto offset = output.numel () - k;

            for (octave_idx_type i = 0; i < dims(0); i++)
              {
                auto dat = idata[i];

                bool is_pos = contains_nan ? typename Comp::greater_nan {} (dat, zero) : typename Comp::greater {} (dat, zero);

                if (is_pos)
                  {
                    *second_data++ = i + 1;

                    odata[i] = dat;
                  }
                else if (typename Comp::equal {} (dat ,zero))
                  {
                    *second_data++ = 1;
                  }
                else
                  {
                    *second_data++ = (i==0? 2 : 1);

                    if (full_sort)
                      odata[i + offset] = dat;
                  }
              }

            for (octave_idx_type j = 1; j < k-1; j++)
              for (octave_idx_type i = 0; i < dims(0); i++)
                {
                  auto dat = idata[i];

                  bool is_pos = contains_nan ? typename Comp::greater_nan {} (dat, zero) : typename Comp::greater {} (dat, zero);

                  if (is_pos)
                    {
                      *second_data++ = (j <= i ? j : j+1);
                    }
                  else if (typename Comp::equal {} (dat ,zero))
                    {
                      *second_data++ = j+1;
                    }
                  else
                    {
                      *second_data++ = (j < i ? j+1 : j+2);
                    }
                }

            if (k > 1)
              for (octave_idx_type i = 0; i < dims(0); i++)
                {
                  auto dat = idata[i];

                  bool is_pos = contains_nan ? typename Comp::greater_nan {} (dat, zero) : typename Comp::greater {} (dat, zero);

                  if (is_pos)
                    {
                      *second_data++ = (k-1 <= i ? k-1 : k);
                    }
                  else if (typename Comp::equal {} (dat ,zero))
                    {
                      *second_data++ = k;
                    }
                  else
                    {
                      if (full_sort)
                        {
                          *second_data++ = i + 1;
                        }
                      else
                        {
                          *second_data++ = (k <= i ? k : k+1);
                        }
                    }
                }
          }

        return ovl (octave_value (output), octave_value (second_output));
      }

    return octave_value_list ();
  }

template <typename Comp>
  octave_value_list
  do_maxk (const DiagMatrix& input, octave_idx_type k , octave_idx_type dim, bool compute_index)
  {
    return do_maxk_diag <Comp>(input, k, dim, compute_index);
  }

template <typename Comp>
  octave_value_list
  do_maxk (const FloatDiagMatrix& input, octave_idx_type k , octave_idx_type dim, bool compute_index)
  {
    return do_maxk_diag <Comp>(input, k, dim, compute_index);
  }

template <typename Comp>
  octave_value_list
  do_maxk (const ComplexDiagMatrix& input, octave_idx_type k , octave_idx_type dim, bool compute_index)
  {
    return do_maxk_diag <Comp>(input, k, dim, compute_index);
  }

template <typename Comp>
  octave_value_list
  do_maxk (const FloatComplexDiagMatrix& input, octave_idx_type k , octave_idx_type dim, bool compute_index)
  {
    return do_maxk_diag <Comp>(input, k, dim, compute_index);
  }

template <typename Comp,typename ArrayType>
  octave_value_list
  do_maxk_vec_stable(const ArrayType& input, octave_idx_type k , octave_idx_type dim)
  {
    auto dims_input = input.dims ();

    auto dims_output = input.dims ();

    k = std::min (dims_output (dim) , k);

    dims_output (dim) = k;

    ArrayType output (dims_output);

    const typename ArrayType::element_type* input_data = input.fortran_vec ();

    typename ArrayType::element_type* output_data = output.fortran_vec ();

    std::vector <octave_idx_type> buf (k);

    for (const typename ArrayType::element_type* i = input_data; i < input_data + input.numel (); i+=dims_input (dim))
      {
        if (! has_nan (i, i+dims_input (dim)))
          {
            index_iterator it{0};

            std::partial_sort_copy (it, it +dims_input (dim), buf.begin (), buf.end (), [i]
              (octave_idx_type a, octave_idx_type b)
              {
                auto A = std::make_pair(a, i[a]);
                auto B = std::make_pair(b, i[b]);
                return typename Comp::greater{} (A, B);
              }
            );
          }
        else
          {
            index_iterator it{0};

            std::partial_sort_copy (it, it+dims_input (dim), buf.begin (), buf.end (), [i]
              (octave_idx_type a, octave_idx_type b)
              {
                auto A = std::make_pair(a, i[a]);
                auto B = std::make_pair(b, i[b]);
                return typename Comp::greater_nan{} (A, B);
              }
            );
          }

        for (octave_idx_type s = 0; s < dims_output (dim); s++)
          {
            output_data[s] = i[buf[s]];
          }

        output_data += dims_output (dim);

        OCTAVE_QUIT;
      }

    return ovl (octave_value (output));
  }

template <typename Comp, typename ArrayType>
  octave_value_list
  do_maxk_stable(const ArrayType& input, octave_idx_type k , octave_idx_type dim)
  {
    auto dims_input = input.dims ();

    auto dims_output = input.dims ();

    if ((dim + 1) > input.ndims ())
      {
        if (k == 0)
          {
            dims_output.resize (dim + 1, 1);

            dims_output (dim) = k;

            return ovl (octave_value (ArrayType (dims_output)));
          }

        return ovl (octave_value (input));
      }

    k = std::min (dims_output (dim) , k);

    dims_output (dim) = k;

    if (k == 0)
      {
        return ovl (octave_value (ArrayType (dims_output)));
      }

    if (input.numel () == 0)
      {
        return ovl (octave_value (input.reshape (dims_output)));
      }

    if (dims_input (dim) == 1)
      {
        return ovl (octave_value (input));
      }

    if (dim == 0 || dims_input.is_nd_vector ())
      {
        return do_maxk_vec_stable<Comp> (input, k, dim);
      }

    octave_idx_type sz = input.numel () / dims_input (dim);

    ArrayType output (dims_output);

    start_elem_indexer input_indexer (dims_input, k, dim);
    start_elem_indexer output_indexer (dims_output, k, dim);

    octave_idx_type offset_input = compute_offset (dims_input, dim);

    octave_idx_type offset_output = compute_offset (dims_output, dim);

    const typename ArrayType::element_type* input_data = input.fortran_vec ();

    typename ArrayType::element_type* output_data = output.fortran_vec ();

    std::vector <std::pair<octave_idx_type, typename ArrayType::element_type>> buf (dims_output (dim));

    const bool contain_nan = has_nan (input_data, input_data+input.numel ());

    for (octave_idx_type j = 0 ; j < sz; j++)
      {
        auto idxin = input_indexer++;

        auto input_begin = index_value_iterator<const typename ArrayType::element_type> {input_data + idxin, offset_input};

        auto input_end = input_begin + (dims_input (dim) );

        auto idxout = output_indexer++;

        auto output_begin = value_iterator<typename ArrayType::element_type> {output_data + idxout, offset_output};

        if (! contain_nan)
          std::partial_sort_copy (input_begin, input_end, buf.begin(), buf.end(), typename Comp::greater {});
        else
          std::partial_sort_copy (input_begin, input_end, buf.begin(), buf.end(), typename Comp::greater_nan {});

        std::transform (buf.begin(), buf.end(), output_begin,
          [] (const std::pair<octave_idx_type, typename ArrayType::element_type>& a)
          {
            return a.second;
          }
        );

        OCTAVE_QUIT;
      }

    return ovl (octave_value (output));
  }

template <typename Comp,typename ArrayType>
  octave_value_list
  do_maxk_vec_index(const ArrayType& input, octave_idx_type k , octave_idx_type dim)
  {
    auto dims_input = input.dims ();

    auto dims_output = input.dims ();

    dims_output (dim) = std::min (dims_output (dim) , k);

    ArrayType output (dims_output);

    NDArray second_output (dims_output);

    const typename ArrayType::element_type* input_data = input.fortran_vec ();

    typename ArrayType::element_type* output_data = output.fortran_vec ();

    double* second_output_data = second_output.fortran_vec ();

    for (const typename ArrayType::element_type* i = input_data; i < input_data + input.numel (); i+=dims_input (dim))
      {
        if (! has_nan (i, i+dims_input (dim)))
          {
            index_iterator it{0};

            std::partial_sort_copy (it, it +dims_input (dim), second_output_data, second_output_data + dims_output (dim), [i]
              (octave_idx_type a, octave_idx_type b)
              {
                auto A = std::make_pair(a, i[a]);
                auto B = std::make_pair(b, i[b]);
                return typename Comp::greater{} (A, B);
              }
            );
          }
        else
          {
            index_iterator it{0};

            std::partial_sort_copy (it, it+dims_input (dim), second_output_data, second_output_data + dims_output (dim), [i]
              (octave_idx_type a, octave_idx_type b)
              {
                auto A = std::make_pair(a, i[a]);
                auto B = std::make_pair(b, i[b]);
                return typename Comp::greater_nan{} (A, B);
              }
            );
          }

        for (octave_idx_type s = 0; s < dims_output (dim); s++)
          {
            output_data[s] = i[(octave_idx_type)second_output_data[s]];
            ++second_output_data[s];
          }

        second_output_data += dims_output (dim);
        output_data += dims_output (dim);

        OCTAVE_QUIT;
      }

    return ovl (octave_value (output), octave_value (second_output));
  }

template <typename Comp, typename ArrayType>
  octave_value_list
  do_maxk_with_index(const ArrayType& input, octave_idx_type k , octave_idx_type dim)
  {
    auto dims_input = input.dims ();

    auto dims_output = input.dims ();

    if ((dim + 1) > input.ndims ())
      {
        if (k == 0)
          {
            dims_output.resize (dim + 1, 1);

            dims_output (dim) = k;

            return ovl (octave_value (ArrayType (dims_output)), octave_value (NDArray (dims_output, 1)));
          }

        return ovl (octave_value (input), octave_value (NDArray (dims_output, 1)));
      }

    k = std::min (dims_output (dim) , k);

    dims_output (dim) = k;

    if (k == 0)
      {
        return ovl (octave_value (ArrayType (dims_output)), octave_value (NDArray (dims_output, 1)));
      }

    if (input.numel () == 0)
      {
        return ovl (octave_value (input.reshape (dims_output)), octave_value (NDArray (dims_output)));
      }

    if (dims_input (dim) == 1)
      {
        return ovl (octave_value (input),octave_value (NDArray (dims_output,1)));
      }

    if (dim == 0 || dims_input.is_nd_vector ())
      {
        return do_maxk_vec_index<Comp> (input, k, dim);
      }

    octave_idx_type sz = input.numel () / dims_input (dim);

    NDArray second_output (dims_output);
    ArrayType output (dims_output);

    start_elem_indexer input_indexer (dims_input, k, dim);
    start_elem_indexer output_indexer (dims_output, k, dim);

    octave_idx_type offset_input = compute_offset (dims_input, dim);

    octave_idx_type offset_output = compute_offset (dims_output, dim);

    const typename ArrayType::element_type* input_data = input.fortran_vec ();

    typename ArrayType::element_type* output_data = output.fortran_vec ();

    typename NDArray::element_type* second_output_data = second_output.fortran_vec ();

    std::vector <std::pair<octave_idx_type, typename ArrayType::element_type>> buf (dims_output (dim));

    const bool contain_nan = has_nan (input_data, input_data+input.numel ());

    for (octave_idx_type j = 0 ; j < sz; j++)
      {
        auto idxin = input_indexer++;

        auto input_begin = index_value_iterator<const typename ArrayType::element_type> {input_data + idxin, offset_input};

        auto input_end = input_begin + (dims_input (dim) );

        auto idxout = output_indexer++;

        auto output_begin = value_iterator<typename ArrayType::element_type> {output_data + idxout, offset_output};

        auto second_output_begin = value_iterator<typename NDArray::element_type> {second_output_data + idxout, offset_output};

        if (! contain_nan)
          std::partial_sort_copy (input_begin, input_end, buf.begin(), buf.end(), typename Comp::greater {});
        else
          std::partial_sort_copy (input_begin, input_end, buf.begin(), buf.end(), typename Comp::greater_nan {});

        std::transform (buf.begin(), buf.end(), output_begin,
          [] (const std::pair<octave_idx_type, typename ArrayType::element_type>& a)
          {
            return a.second;
          }
        );

        std::transform (buf.begin(), buf.end(), second_output_begin,
          [] (const std::pair<octave_idx_type, typename ArrayType::element_type>& a)
          {
            return a.first + 1;
          }
        );

        OCTAVE_QUIT;
      }

    return ovl (octave_value (output), octave_value (second_output));
  }

template <typename Comp, typename ArrayType>
  octave_value_list
  do_maxk_vec(const ArrayType& input, octave_idx_type k , octave_idx_type dim)
  {
    auto dims_input = input.dims ();

    auto dims_output = input.dims ();

    if ((dim + 1) > input.ndims ())
      error ("dim is larger than array dimensions");

    dims_output (dim) = std::min (dims_output (dim) , k);

    ArrayType output (dims_output);

    const typename ArrayType::element_type* input_data = input.fortran_vec ();

    typename ArrayType::element_type* output_data = output.fortran_vec ();

    for (const typename ArrayType::element_type* i = input_data; i < input_data + input.numel (); i+=dims_input (dim))
      {
        if (! has_nan (i, i+dims_input (dim)))
          std::partial_sort_copy (i, i+dims_input (dim), output_data, output_data + dims_output (dim), typename Comp::greater {});
        else
          std::partial_sort_copy (i, i+dims_input (dim), output_data, output_data + dims_output (dim), typename Comp::greater_nan {});

        output_data += dims_output (dim);

        OCTAVE_QUIT;
      }

    return octave_value (output);
  }

template <typename Comp>
  octave_value_list
  do_maxk(const ComplexNDArray& input, octave_idx_type k , octave_idx_type dim, bool compute_index)
  {
    if (compute_index)
      return do_maxk_with_index<Comp> (input, k, dim);

    return do_maxk_stable<Comp> (input, k, dim);
  }

template <typename Comp>
  octave_value_list
  do_maxk(const FloatComplexNDArray& input, octave_idx_type k , octave_idx_type dim, bool compute_index)
  {
    if (compute_index)
      return do_maxk_with_index<Comp> (input, k, dim);

    return do_maxk_stable<Comp> (input, k, dim);
  }

template <typename Comp, typename ArrayType>
  octave_value_list
  do_maxk(const ArrayType& input, octave_idx_type k , octave_idx_type dim, bool compute_index)
  {
    if (compute_index)
      return do_maxk_with_index<Comp> (input, k, dim);

    auto dims_input = input.dims ();

    auto dims_output = input.dims ();

    if ((dim + 1) > input.ndims ())
      {
        if (k == 0)
          {
            dims_output.resize (dim + 1, 1);

            dims_output (dim) = k;

            return ovl (octave_value (ArrayType (dims_output)));
          }

        return ovl (octave_value (input));
      }

    k = std::min (dims_output (dim) , k);

    dims_output (dim) = k;

    if (k == 0)
      {
        return octave_value (ArrayType (dims_output));
      }

    if (input.numel () == 0)
      {
        return octave_value (input.reshape (dims_output));
      }

    if (dims_input (dim) == 1)
      {
        return octave_value (input);
      }

    const typename ArrayType::element_type* input_data = input.fortran_vec ();

    bool contain_neg = false;

    if (std::is_same<Comp, comp_abs>::value)
      contain_neg = has_neg (input_data, input_data + input.numel ());

    if (contain_neg)
      return do_maxk_stable<Comp> (input, k, dim);

    if (dim == 0 || dims_input.is_nd_vector ())
      {
        return do_maxk_vec<Comp> (input, k, dim);
      }

    octave_idx_type sz = input.numel () / dims_input (dim);

    ArrayType output (dims_output);

    start_elem_indexer input_indexer (dims_input, k, dim);

    start_elem_indexer output_indexer (dims_output, k, dim);

    typename ArrayType::element_type* output_data = output.fortran_vec ();

    octave_idx_type offset_input = compute_offset (dims_input, dim);

    octave_idx_type offset_output = compute_offset (dims_output, dim);

    std::unique_ptr<typename ArrayType::element_type[]> buf{new typename ArrayType::element_type[k]};

    const bool contain_nan = has_nan (input_data, input_data + input.numel ());

    for (octave_idx_type j = 0 ; j < sz; j++)
      {
        auto input_begin = value_iterator<const typename ArrayType::element_type> {input_data + input_indexer++, offset_input};

        auto input_end = input_begin + (dims_input (dim) );

        auto output_begin = value_iterator<typename ArrayType::element_type> {output_data + output_indexer++, offset_output};
        auto output_end = output_begin + (dims_output (dim) );

        if (contain_nan)
          {
            std::partial_sort_copy (input_begin, input_end, buf.get(), buf.get() + k, typename Comp::greater_nan {});
          }
        else
          {
            std::partial_sort_copy (input_begin, input_end, buf.get(), buf.get() + k, typename Comp::greater {});
          }

        std::copy (buf.get(), buf.get() + k, output_begin);

        OCTAVE_QUIT;
      }

    return octave_value (output);
  }

  std::string
  get_comparison_method (const octave_value_list& args, int idx)
  {
    if (args.length () != idx+2)
      error ("invalid number of inputs");

    if (args (idx).string_value () != "ComparisonMethod")
      error ("Invalid argument. Dp you mean ComparisonMethod?");

    std::string ComparisonMethod = args (idx+1).xstring_value ("argument number %s should be string", std::to_string (idx +1).c_str ());

    if (! (ComparisonMethod == "auto" || ComparisonMethod == "real" || ComparisonMethod == "abs" ))
      error ("ComparisonMethod should be one of auto, real or abs");

    return ComparisonMethod;
  }

template <typename ArrayType>
  dim_vector
  get_dims (const ArrayType& array)
  {
    return array.dims ();
  }

  dim_vector
  get_dims (const Range& array)
  {
    octave_idx_type n = array.numel ();
    return dim_vector (n > 0, n);
  }

template <typename ArrayType>
  octave_value_list
  dispatch2 (const ArrayType& array, const octave_value_list& args, int nargout)
  {
    octave_idx_type nargin = args.length ();

    if (! args (1).isnumeric ())
      error ("k should be of numeric type");

    if (args (1).numel () != 1)
      error ("k should be scalar");

    octave_idx_type k = args (1).idx_type_value ();

    if (k < 0)
      error ("k should be non negative integer");

    octave_idx_type dim = 0;

    auto dims = get_dims (array);

    for (int i = 0; i < dims.length (); i++)
      {
        if (dims (i) != 1)
          {
            dim = i;

            break;
          }
      }

    std::string ComparisonMethod = "auto";

    if (nargin > 2)
      {
        if (args (2).isnumeric ())
          {
            if (args (2).numel () != 1)
              error ("dim should be scalar");

            dim = args (2).idx_type_value ();

            if (dim <= 0)
              error ("dim should be positive integer");

            dim--;
          }
        else if (args (2).is_string ())
          {
            ComparisonMethod = get_comparison_method (args, 2);
          }
      }

    if (nargin > 3)
      {
        if (args (3).is_string ())
          {
            if (! args (2).is_string ())
              ComparisonMethod = get_comparison_method (args, 3);
          }
        else
          error ("invalid argument 4");
      }

    if (ComparisonMethod == "auto")
      return do_maxk<comp_auto> (array, k, dim, nargout == 2);
    else if (ComparisonMethod == "real")
      return do_maxk<comp_real> (array, k, dim, nargout == 2);

    return do_maxk<comp_abs> (array, k, dim, nargout == 2);
  }

  octave_value_list
  dispatch (const octave_value_list& args, int nargout)
  {
    octave_idx_type nargin = args.length ();

    if (nargin < 2 || nargin > 5)
      error ("invalid number of arguments");

    octave_value im = args(0);

    if (im.is_range ())
      {
        return stats::dispatch2 (im.range_value (), args, nargout);
      }
    else if (im.issparse ())
      {
        if (im.islogical ())
          return stats::dispatch2 (im.sparse_bool_matrix_value (), args, nargout);
        else if (im.iscomplex ())
          return stats::dispatch2 (im.sparse_complex_matrix_value (), args, nargout);
        else
          return stats::dispatch2 (im.sparse_matrix_value (), args, nargout);
      }
    else if (im.is_diag_matrix ())
      {
        if (im.iscomplex ())
          {
            if (im.is_single_type ())
              return stats::dispatch2 (im.float_complex_diag_matrix_value (), args, nargout);
            else
              return stats::dispatch2 (im.complex_diag_matrix_value (), args, nargout);
          }
        else
          {
            if (im.is_single_type ())
              return stats::dispatch2 (im.float_diag_matrix_value (), args, nargout);
            else
              return stats::dispatch2 (im.diag_matrix_value (), args, nargout);
          }
      }
    else if (im.is_perm_matrix ())
      return stats::dispatch2 (im.perm_matrix_value (), args, nargout);
    else if (im.islogical ())
      return stats::dispatch2 (im.bool_array_value (), args, nargout);
    else if (im.is_char_matrix ())
      return stats::dispatch2 (im.char_array_value (), args, nargout);
    else if (im.is_int8_type ())
      return stats::dispatch2 (im.int8_array_value (), args, nargout);
    else if (im.is_int16_type ())
      return stats::dispatch2 (im.int16_array_value (), args, nargout);
    else if (im.is_int32_type ())
      return stats::dispatch2 (im.int32_array_value (), args, nargout);
    else if (im.is_int64_type ())
      return stats::dispatch2 (im.int64_array_value (), args, nargout);
    else if (im.is_uint8_type ())
      return stats::dispatch2 (im.uint8_array_value (), args, nargout);
    else if (im.is_uint16_type ())
      return stats::dispatch2 (im.uint16_array_value (), args, nargout);
    else if (im.is_uint32_type ())
      return stats::dispatch2 (im.uint32_array_value (), args, nargout);
    else if (im.is_uint64_type ())
      return stats::dispatch2 (im.uint64_array_value (), args, nargout);
    else if (im.isreal ())
      {
        if (im.is_single_type ())
          return stats::dispatch2 (im.float_array_value (), args, nargout);
        else
          return stats::dispatch2 (im.array_value (), args, nargout);
      }
    else if (im.iscomplex ())
      {
        if (im.is_single_type ())
          return stats::dispatch2 (im.float_complex_array_value (), args, nargout);
        else
          return stats::dispatch2 (im.complex_array_value (), args, nargout);
      }
    else
      error ("invalid input type");

    return octave_value_list ();
  }
}

DEFUN_DLD (maxk, args, nargout,
               R"helpdoc(-*- texinfo -*-
@deftypefn {Loadable Function} {B =} maxk(@var{A}, @var{k})
@deftypefnx {Loadable Function} {B =} maxk(@var{A}, @var{k}, @var{dim})
@deftypefnx {Loadable Function} {B =} maxk(@var{___}, 'ComparisonMethod', @var{c})
@deftypefnx {Loadable Function} {[B, I] =} maxk(@var{___})

Partially sort and return @var{k} largest elements of @var{A}.

@var{A} can be a multidimensional array.

If @var{dim} is provided do computation along dimension @var{dim} otherwise do along the first non singleton dimension.

@var{k} should be non negative. If it is greater than or equal the size of the operating dimension full sort along the dimension.

If 'ComparisonMethod' is provided @var{c} can be one of the following options:

@table @asis
@item @qcode{'auto'} (default)
If @var{A} is complex compare elements using 'abs'. If @var{A} is real do ordinary comparison. It is the default method.
@item @qcode{'real'}
If @var{A} is complex compare elements using 'real' and for elements with equal real components use 'imag'. If @var{A} is real do ordinary comparison.
@item @qcode{'abs'}
Use 'abs' for comparing both real and complex @var{A} . In the case of complex @var{A} for elements with equal 'abs' use 'angle'.
@end table

In all of the methods 'nan' is assumed to be the greatest element and order of equal elements are preserved.

The return value @var{B} contains the @var{k} largest elements that are sorted in descending order.

If @var{I} is requested it contains the indices of the sorted elements.

@seealso{sort, maxk}

@end deftypefn)helpdoc")

{
  return stats::dispatch (args, nargout);
}