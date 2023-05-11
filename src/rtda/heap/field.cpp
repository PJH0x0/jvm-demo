#include "field.h"

namespace rtda{
void createFields(std::shared_ptr<Class> classPtr, std::vector<std::shared_ptr<classfile::MemberInfo>>& cfFields, 
  std::vector<std::shared_ptr<Field>>& fields) {
  for (auto cfField : cfFields) {
    std::shared_ptr<Field> field = std::make_shared<Field>(cfField, classPtr);
    fields.push_back(field);
  }
}
} // namespace rtda