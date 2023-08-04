#pragma once
#include <compare>
#include <concepts>

/**
 * @brief Concept of a config structure for a strong type
 */
template <typename config>
concept isStrongTypeConfig = requires() {
  typename config::underlyingType;  // requires an underlying type in a using or
                                    // typedef decl
  // Maybe enforce compile time expressions
  //
  // Which Operators shall be available (Either Old Operators or Spaceship)
  { config::spaceship } -> std::convertible_to<bool>;
  { config::equal } -> std::convertible_to<bool>;
  { config::notEqual } -> std::convertible_to<bool>;

  { config::lessThen } -> std::convertible_to<bool>;
  { config::lessEqual } -> std::convertible_to<bool>;
  { config::greaterThen } -> std::convertible_to<bool>;
  { config::greaterEqual } -> std::convertible_to<bool>;
  // Allows comparision against the underlying type
  { config::allowUnderlyingTypeInOperator } -> std::convertible_to<bool>;
};

/**
 * @brief Concept if 2 const objects are comparable via spaceship operator
 */
template <typename a, typename b = a>
concept isSpaceshipComparable = requires(const a& lhs, const b& rhs) {
  { lhs <=> rhs } -> std::convertible_to<std::partial_ordering>;
};
/**
 * @brief Concept if 2 const objects are comparable via operator==
 */
template <typename a, typename b = a>
concept isEqualComparable = requires(const a& lhs, const b& rhs) {
  { lhs == rhs } -> std::convertible_to<bool>;
};
/**
 * @brief Concept if 2 const objects are comparable via operator!=. Apparently
 * the compiler might use == instead!
 */
template <typename a, typename b = a>
concept isNotEqualComparable = requires(const a& lhs, const b& rhs) {
  { lhs != rhs } -> std::convertible_to<bool>;
};

/**
 * @brief Concept if 2 const objects are comparable via operator>
 */
template <typename a, typename b = a>
concept isGreaterThenComparable = requires(const a& lhs, const b& rhs) {
  { lhs > rhs } -> std::convertible_to<bool>;
};
/**
 * @brief Concept if 2 const objects are comparable via operator>=
 */
template <typename a, typename b = a>
concept isGreaterEqualComparable = requires(const a& lhs, const b& rhs) {
  { lhs >= rhs } -> std::convertible_to<bool>;
};
/**
 * @brief Concept if 2 const objects are comparable via operator<
 */
template <typename a, typename b = a>
concept isLessThenComparable = requires(const a& lhs, const b& rhs) {
  { lhs < rhs } -> std::convertible_to<bool>;
};
/**
 * @brief Concept if 2 const objects are comparable via operator<=
 */
template <typename a, typename b = a>
concept isLessEqualComparable = requires(const a& lhs, const b& rhs) {
  { lhs <= rhs } -> std::convertible_to<bool>;
};
/**
 * @brief Class for a StrongType, can be used either as inheritance or
    composition if you want to extend this class via customization points!
 * @tparam config Configuration Structure for which Operator should be available
*/
template <isStrongTypeConfig config>
class StrongType {
 public:
  // Removing any possible cvrefs
  using type = std::remove_cvref_t<typename config::underlyingType>;
  // If the underlying type is smaller then a uintptr_t (assuming thats the size
  // of a register, which probabbly isn't correct for NUMA architectures) this
  // class will use copy instead of a const reference
  using type_cref = typename std::
      conditional_t<sizeof(type) >= sizeof(std::uintptr_t), const type&, type>;

  /**
   * @brief Explicit Conversion Operator, no implicit conversion allowed so we
   * can retain the value of strong types
   * @param in current value
   */
  explicit StrongType(type_cref in) : data{in} {}
  /**
   * @brief Method to convert to the underlying value. This could be argued to
   * be a cast in future or only the const overload
   * @return current value
   */
  [[nodiscard]] auto get() noexcept -> type& { return data; }
  /**
   * @brief Retrieving the underlying value
   * @return current value
   */
  [[nodiscard]] auto get() const noexcept -> type_cref { return data; }

#pragma region Compare with StrongType<config>
  /**
   * @brief Spaceship operator for comparision via StrongType<config>. This is
   * only a template to check the requirements. This function can be disabled
   * via config!
   * @tparam otherType Same as StrongType
   * @param rhs comparision object
   * @return result of the comparision
   */
  template <typename otherType>
    requires std::is_same_v<StrongType<config>, otherType> &&
             config::spaceship && isSpaceshipComparable<type>
  [[nodiscard]] auto operator<=>(const otherType& rhs) const noexcept {
    return this->data <=> rhs.data;
  }
  /**
   * @brief Equal operator for comparision via StrongType<config>. This is
   * only a template to check the requirements. This function can be disabled
   * via config!
   * @tparam otherType Same as StrongType
   * @param rhs Comparision object
   * @return result of the comparision
   */
  template <typename otherType>
    requires std::is_same_v<StrongType<config>, otherType> && config::equal &&
             isEqualComparable<type>
  [[nodiscard]] auto operator==(const otherType& rhs) const -> bool {
    return this->data == rhs.data;
  };
  /**
   * @brief Not Equal operator for comparision via StrongType<config>. This is
   * only a template to check the requirements. This function can be disabled
   * via config!
   * @tparam otherType Same as StrongType
   * @param rhs Comparision object
   * @return result of the comparision
   */
  template <typename otherType>
    requires std::is_same_v<StrongType<config>, otherType> &&
             config::notEqual && isNotEqualComparable<type>
  [[nodiscard]] auto operator!=(const otherType& rhs) const -> bool {
    return this->data != rhs.data;
  };
  /**
   * @brief Less then operator for comparision via StrongType<config>. This is
   * only a template to check the requirements. This function can be disabled
   * via config!
   * @tparam otherType Same as StrongType
   * @param rhs Comparision object
   * @return result of the comparision
   */
  template <typename otherType>
    requires std::is_same_v<StrongType<config>, otherType> &&
             (config::lessThen && !config::spaceship) &&
             isLessThenComparable<type>
  [[nodiscard]] auto operator<(const otherType& rhs) const -> bool {
    return this->data < rhs.data;
  };
  /**
   * @brief Less or Equal then operator for comparision via StrongType<config>.
   * This is only a template to check the requirements. This can be disabled via
   * config.
   * @tparam otherType Same as StrongType
   * @param rhs Comparision object
   * @return result of the comparision
   */
  template <typename otherType>
    requires std::is_same_v<StrongType<config>, otherType> &&
             (config::lessEqual && !config::spaceship) &&
             isLessEqualComparable<type>
  [[nodiscard]] auto operator<=(const otherType& rhs) const -> bool {
    return this->data <= rhs.data;
  };
  /**
   * @brief Greater then operator for comparision via StrongType<config>. This
   * is only a template to check the requirements. This can be disabled via
   * config.
   * @tparam otherType Same as StrongType
   * @param rhs Comparision object
   * @return result of the comparision
   */
  template <typename otherType>
    requires std::is_same_v<StrongType<config>, otherType> &&
             (config::greaterThen && !config::spaceship) &&
             isGreaterThenComparable<type>
  [[nodiscard]] auto operator>(const otherType& rhs) const -> bool {
    return this->data > rhs.data;
  };
  /**
   * @brief Greater or equal then operator for comparision via
   * StrongType<config>. This is only a template to check the requirements. This
   * can be disabled via config.
   * @tparam otherType Same as StrongType
   * @param rhs Comparision object
   * @return result of the comparision
   */
  template <typename otherType>
    requires std::is_same_v<StrongType<config>, otherType> &&
             (config::greaterEqual && !config::spaceship) &&
             isGreaterEqualComparable<type>
  [[nodiscard]] auto operator>=(const otherType& rhs) const -> bool {
    return this->data >= rhs.data;
  };
#pragma endregion
#pragma region Compare with underlying Type
  /**
   * @brief Spaceship operator for comparision via the underlying type. This is
   * only a template to check the requirements. This function can be disabled
   * via config!
   * @tparam otherType Same as StrongType
   * @param rhs Comparision object
   * @return result of the comparision
   */
  template <typename otherType>
    requires std::is_same_v<type, otherType> &&
             (config::spaceship && config::allowUnderlyingTypeInOperator) &&
             isSpaceshipComparable<type>
  [[nodiscard]] auto operator<=>(const otherType& rhs) const noexcept {
    return this->data <=> rhs.data;
  }
  /**
   * @brief Equal operator for comparision via the underlying type. This is
   * only a template to check the requirements. This function can be disabled
   * via config!
   * @tparam otherType Same as StrongType
   * @param rhs Comparision object
   * @return result of the comparision
   */
  template <typename otherType>
    requires std::is_same_v<type, otherType> &&
             (config::equal && config::allowUnderlyingTypeInOperator) &&
             isEqualComparable<type>
  [[nodiscard]] auto operator==(const otherType& rhs) const -> bool {
    return this->data == rhs;
  };
  /**
   * @brief Not equal operator for comparision via the underlying type. This is
   * only a template to check the requirements. This function can be disabled
   * via config!
   * @tparam otherType Same as StrongType
   * @param rhs Comparision object
   * @return result of the comparision
   */
  template <typename otherType>
    requires std::is_same_v<type, otherType> &&
             (config::notEqual && config::allowUnderlyingTypeInOperator) &&
             isNotEqualComparable<type>
  [[nodiscard]] auto operator!=(const otherType& rhs) const -> bool {
    return this->data != rhs;
  };
  /**
   * @brief Less then operator for comparision via the underlying type. This is
   * only a template to check the requirements. This function can be disabled
   * via config!
   * @tparam otherType Same as StrongType
   * @param rhs Comparision object
   * @return result of the comparision
   */
  template <typename otherType>
    requires std::is_same_v<type, otherType> &&
             (config::lessThen && !config::spaceship &&
              config::allowUnderlyingTypeInOperator) &&
             isLessThenComparable<type>
  [[nodiscard]] auto operator<(const otherType& rhs) const -> bool {
    return this->data < rhs;
  };
  /**
   * @brief Less or equal then operator for comparision via the underlying type.
   * This is only a template to check the requirements. This function can be
   * disabled via config!
   * @tparam otherType Same as StrongType
   * @param rhs Comparision object
   * @return result of the comparision
   */
  template <typename otherType>
    requires std::is_same_v<type, otherType> &&
             (config::lessEqual && !config::spaceship &&
              config::allowUnderlyingTypeInOperator) &&
             isLessEqualComparable<type>
  [[nodiscard]] auto operator<=(const otherType& rhs) const -> bool {
    return this->data <= rhs;
  };

  /**
   * @brief Greater then operator for comparision via the underlying type. This
   * is only a template to check the requirements. This function can be disabled
   * via config!
   * @tparam otherType Same as StrongType
   * @param rhs Comparision object
   * @return result of the comparision
   */
  template <typename otherType>
    requires std::is_same_v<type, otherType> &&
             (config::greaterThen && !config::spaceship &&
              config::allowUnderlyingTypeInOperator) &&
             isGreaterThenComparable<type>
  [[nodiscard]] auto operator>(const otherType& rhs) const -> bool {
    return this->data > rhs;
  };
  /**
   * @brief Greater or equal then operator for comparision via the underlying
   * type. This is only a template to check the requirements. This function can
   * be disabled via config!
   * @tparam otherType Same as StrongType
   * @param rhs Comparision object
   * @return result of the comparision
   */
  template <typename otherType>
    requires std::is_same_v<type, otherType> &&
             (config::greaterEqual && !config::spaceship &&
              config::allowUnderlyingTypeInOperator) &&
             isGreaterEqualComparable<type>
  [[nodiscard]] auto operator>=(const otherType& rhs) const -> bool {
    return this->data >= rhs;
  };
#pragma endregion

 protected:
  type data;
};
