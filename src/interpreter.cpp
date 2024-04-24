#include "interpreter.h"
#include <utility>
#include "value.h"

namespace clox
{

// interpret Binary expression
ReturnValType
Interpreter::visit(const Binary &expr) {
  Value left = std::get<Value>(evaluate(*expr.left.get()));
  Value right = std::get<Value>(evaluate(*expr.right.get()));
  switch (expr.op._type)
  {
  case TokenType::MINUS:
    if (Value::whichType(left) == ValueType::NUMBER && Value::whichType(right) == ValueType::NUMBER) {
        return {Value(left.number() - right.number())};
    }
    checkNumber(expr.op, left, right);
    break;
  case TokenType::PLUS:
    if (Value::whichType(left) == ValueType::NUMBER && Value::whichType(right) == ValueType::NUMBER) {
        return {Value(left.number() + right.number())};
    } 
    if (Value::whichType(left) == ValueType::STRING && Value::whichType(right) == ValueType::STRING) {
        return {Value(left.string() + right.string())};
    }
    throw RuntimeError(expr.op, "Operands must be two numbers or two strings.");
    break;
  case TokenType::SLASH:
    if (Value::whichType(left) == ValueType::NUMBER && Value::whichType(right) == ValueType::NUMBER) {
        return {Value(left.number() / right.number())};
    }
    checkNumber(expr.op, left, right);
    break;
  case TokenType::STAR:
    if (Value::whichType(left) == ValueType::NUMBER && Value::whichType(right) == ValueType::NUMBER) {
        return {Value(left.number() * right.number())};
    }
    checkNumber(expr.op, left, right);
    break;
  case TokenType::GREATER:
    if (Value::whichType(left) == ValueType::NUMBER && Value::whichType(right) == ValueType::NUMBER) {
        return {Value(left.number() > right.number())};
    }
    checkNumber(expr.op, left, right);
    break;
  case TokenType::GREATER_EQUAL:
    if (Value::whichType(left) == ValueType::NUMBER && Value::whichType(right) == ValueType::NUMBER) {
        return {Value(left.number() >= right.number())};
    }
    checkNumber(expr.op, left, right);
    break;
  case TokenType::LESS:
    if (Value::whichType(left) == ValueType::NUMBER && Value::whichType(right) == ValueType::NUMBER) {
        return {Value(left.number() < right.number())};
    }
    checkNumber(expr.op, left, right);
    break;
  case TokenType::LESS_EQUAL:
    if (Value::whichType(left) == ValueType::NUMBER && Value::whichType(right) == ValueType::NUMBER) {
        return {Value(left.number() <= right.number())};
    }
    checkNumber(expr.op, left, right);
    break;
  case TokenType::BANG_EQUAL:
    return {Value(left._value != right._value)};
  case TokenType::EQUAL_EQUAL:
    return {Value(left._value == right._value)};

  default:
      break;
  }
}

// interpret Grouping expression
ReturnValType
Interpreter::visit(const Grouping &expr) {
  return evaluate(*expr.expression.get());
}

// interpret Literal expression
ReturnValType
Interpreter::visit(const Literal &expr) {
  return {expr.value};
}

// interpret Unary expression
ReturnValType
Interpreter::visit(const Unary &expr) {
  Value right = std::get<Value>(evaluate(*expr.right.get()));
  switch (expr.op._type) {
    case TokenType::MINUS:
      if (Value::whichType(right) == ValueType::NUMBER) {
          return {Value(-right.number())};
      }
      checkNumber(expr.op, right);
      break;
    case TokenType::BANG:
      return {Value(!Value::isTruthy(right))};
      break;
    default:
      break;
  }
  return {};
}

// interpret Logical expression
ReturnValType
Interpreter::visit(const Logical &expr) {
  bool left = Value::isTruthy(std::get<Value>(evaluate(*expr.left.get())));

  // early return if the expression is already fixed with half side
  if (expr.op._type == TokenType::AND) {
    if (!left) return {Value{false}};
  } else {
    if (left) return {Value{true}};
  }

  bool right = Value::isTruthy(std::get<Value>(evaluate(*expr.right.get())));
  return {Value{right}};
}

//interpret Var expression
ReturnValType
Interpreter::visit(const Variable& expr) {
  return {environment->get(expr.name)};
}

//interpret assignment expression
ReturnValType
Interpreter::visit(const Assignment& expr) {
  Value value = std::get<Value>(evaluate(*expr.value.get()));
  environment->assign(expr.name, value);
  return {value};
}

// interpret print statement
ReturnValType
Interpreter::visit(const Print &stmt) {
  // return type of expression must be value
  Value value = std::get<Value>(evaluate(*stmt.expression.get()));
  std::cout << Value::toString(value) << '\n';
  return {};
}

// interpret var declaration
ReturnValType
Interpreter::visit(const Var& stmt) {
  Value value = Value();
  if (stmt.initializer != nullptr)
    value = std::get<Value>(evaluate(*stmt.initializer.get()));
  environment->define(stmt.name, value);
  return {};
}

// interpret expression statement
ReturnValType
Interpreter::visit(const Expression &stmt) {
  evaluate(*stmt.expression.get());
  return {};
}

ReturnValType
Interpreter::visit(const Block &stmt) {
  std::shared_ptr<Environment> block_environment = std::make_shared<Environment>(environment);
  environment = block_environment;
  try {
    for (auto& st : stmt.statements) 
      if (st != nullptr)
        execute(*st.get());
  } catch (RuntimeError e) {
    environment = block_environment->_enclosing;
    throw(e);
  }
  environment = block_environment->_enclosing;
  return {};
}

ReturnValType
Interpreter::visit(const IF &stmt) {
  Value condition = std::get<Value>(evaluate(*stmt.condition.get()));
  if (Value::isTruthy(condition)) 
    return execute(*stmt.thenBranch.get());
  if (stmt.elseBranch != nullptr)
    execute(*stmt.elseBranch.get());
  return {};
}

ReturnValType
Interpreter::visit(const WHILE &stmt) {
  // when the condition isn't met, keep looping
  while(Value::isTruthy(std::get<Value>(evaluate(*stmt.condition.get())))) {
    execute(*stmt.body.get());
  }
  return {};
}

ReturnValType
Interpreter::evaluate(const Expr& expr) {
  return expr.accept(*this);
}

ReturnValType
Interpreter::execute(const Stmt& stmt) {
  return stmt.accept(*this);
}

void
Interpreter::interpret(std::vector<std::unique_ptr<Stmt>> stmts) {
  try {
    for (auto& stmt: stmts)
      if (stmt != nullptr)
        execute(*stmt.get());
  } catch (const RuntimeError &error) {
    runtime_error(error);
  }
}


} // namespace clox
