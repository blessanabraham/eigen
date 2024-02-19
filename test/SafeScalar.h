
// A Scalar that asserts for uninitialized access.
template <typename T>
class SafeScalar {
 public:
  SafeScalar() : initialized_(false) {}
  SafeScalar(const T& val) : val_(val), initialized_(true) {}
  SafeScalar& operator=(const T& val) {
    val_ = val;
    initialized_ = true;
    return *this;
  }

  operator T() const {
    VERIFY(initialized_ && "Uninitialized access.");
    return val_;
  }

  template <typename Target>
  explicit operator Target() const {
    return static_cast<Target>(this->operator T());
  }

  template <typename Source>
  explicit SafeScalar(const Source& val) : SafeScalar(static_cast<T>(val)) {}

 private:
  T val_;
  bool initialized_;
};

namespace Eigen {
template <typename T>
struct NumTraits<SafeScalar<T>> : GenericNumTraits<T> {
  enum { RequireInitialization = 1 };
};
}  // namespace Eigen