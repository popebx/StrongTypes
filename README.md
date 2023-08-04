# StrongTypes

A Strong Type Implementation which should lessen the pain of creating new Strong Types for your Project.
 - Comparision Operators are fully configurable
 - Extendable via Inheritance
 - No extra Space in the class Layout (unless you add something)
 - Copy used instead of cref when its smaller then a pointer

Missing:
 - No explicit R-Value Semantics
 - Untested with move only types
 - Doesn't retain trivially aspects of its underlying type
 - noexcept isn't implemented yet