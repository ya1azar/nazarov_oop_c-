#pragma once

#include <memory>
#include <typeinfo>
#include <unordered_map>


namespace composite {

namespace internal {

struct HolderBase {
  virtual ~HolderBase() = default;
};

template <typename Object>
struct Holder final : public HolderBase {
  explicit Holder(const Object& object) : object_(new Object(object)) {}
  ~Holder() final = default;

  const Object* GetObject() const { return object_.get(); }

  Object* GetObject() { return object_.get(); }

  std::unique_ptr<Object> object_;
};

}  // namespace internal

template <typename T>
class Composite {
 public:
  template <typename Object>
  void SetObject(const Object& obj) {
    objects_[typeid(obj)] = std::make_unique<internal::Holder<Object>>(obj);
  }

  template <typename Object>
  void RemoveObject() {
    objects_.erase(typeid(Object));
  }

  template <typename Object>
  Object* GetObject() {
    auto fnd = objects_.find(typeid(Object));
    if (fnd == objects_.end())
      return nullptr;
    return dynamic_cast<internal::Holder<Object>*>(fnd->second.get())
        ->GetObject();
  }

  template <typename Object>
  const Object* GetObject() const {
    auto fnd = objects_.find(typeid(Object));
    if (fnd == objects_.end())
      return nullptr;
    return dynamic_cast<internal::Holder<Object>*>(fnd->second.get())
        ->GetObject();
  }

 private:
  using TypeInfoRef = std::reference_wrapper<const std::type_info>;

  struct Hasher {
    std::size_t operator()(TypeInfoRef code) const {
      return code.get().hash_code();
    }
  };

  struct EqualTo {
    bool operator()(TypeInfoRef lhs, TypeInfoRef rhs) const {
      return lhs.get() == rhs.get();
    }
  };

  std::unordered_map<TypeInfoRef,
                     std::unique_ptr<internal::HolderBase>,
                     Hasher,
                     EqualTo>
      objects_;
};

}  // namespace composite
