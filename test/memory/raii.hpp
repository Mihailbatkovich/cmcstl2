#ifndef RAII_HPP_INCLUDED
#define RAII_HPP_INCLUDED
#include <stl2/memory.hpp>

template <typename T>
class raii {
public:
   raii(const auto size)
      : data_{allocator_.allocate(size)},
        size_{size}
   {
   }

   raii(const raii&) = delete;
   raii(raii&&) = default;

   raii& operator=(const raii&) = delete;
   raii& operator=(raii&&) = default;

   ~raii()
   {
      allocator_.deallocate(data_, size());
   }

   T* begin() noexcept
   {
      return data_;
   }

   const T* begin() const noexcept
   {
      return data_;
   }

   auto cbegin() const noexcept
   {
      return begin();
   }

   T* end() noexcept
   {
      return data_ + size();
   }

   const T* end() const noexcept
   {
      return data_ + size();
   }

   auto cend() const noexcept
   {
      return end();
   }

   std::size_t size() const noexcept
   {
      return size_;
   }
private:
   std::allocator<T> allocator_;
   T* data_;
   std::size_t size_;
};
#endif // RAII_HPP_INCLUDED
