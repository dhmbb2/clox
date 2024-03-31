import os

def define_visitor(f, base_name, types):
    f.write("template <typename T>\n")
    f.write("class Visitor {\npublic:\n")
    for t in types:
        type_name = t.split("=")[0].strip()
        f.write("    virtual T visit(const " + type_name + " &" + base_name.lower() + ") = 0;\n")
    f.write("};\n")

def define_type(f, base_name, class_name, fields):
    f.write("// Expression " + class_name + "\n")
    f.write("template <typename T>\n")
    f.write("class " + class_name + " : public " + base_name + " {\npublic:\n")
    f.write("  explicit " + class_name + "(" + fields + ") : ")
    # Store parameters in fields
    field_list = fields.split(", ")
    for field in field_list[:-1]:
        name = field.split(" ")[1]
        f.write(name + "(std::move(" + name + ")), ")
    name = field_list[-1].split(" ")[1]
    f.write(name + "(std::move(" + name + ")) ")
    f.write(" {}\n\n")

    # Define accept method
    f.write("  T accept(Visitor<T> &visitor) const override {\n")
    f.write("    return visitor.visit(*this);\n")
    f.write("  }\n\n")

    # Define fields
    for field in field_list:
        f.write("  " + field + ";\n")
    f.write("};\n")

    f.write("\n")

def define_ast(output_dir, base_name, types):
    path = os.path.join(output_dir, base_name + ".h")
    with open(path, "w") as f:
        f.write("#pragma once\n\n")
        f.write("#include <memory>\n")
        f.write("#include <vector>\n")
        f.write("#include <string>\n")
        f.write("#include \"token.h\"\n")
        f.write("\n")
        f.write("namespace clox {\n\n")
        f.write("template <typename T>\n")
        f.write("class " + base_name + " {\npublic:\n  virtual ~" + base_name + "() = default;\n")
        f.write("  virtual T\n  accept(Visitor<T> &visitor) const = 0;\n")
        f.write("};\n\n")
        
        # define visitor class
        define_visitor(f, base_name, types)
        f.write("\n\n")

        for t in types:
            class_name = t.split("=")[0].strip()
            fields = t.split("=")[1].strip()
            define_type(f, base_name, class_name, fields)

        f.write("}\n") # namespace clox

if __name__ == "__main__":
    import sys

    if len(sys.argv) != 2:
        print("Usage: generate_ast <output directory>")
        sys.exit(64)

    output_dir = sys.argv[1]
    os.makedirs(output_dir, exist_ok=True)

    define_ast(output_dir, "Expr", [
        "Binary   = std::unique_ptr<Expr> left, Token op, std::unique_ptr<Expr> right",
        "Grouping = std::unique_ptr<Expr> expression",
        "Literal  = Value value",
        "Unary    = Token op, std::unique_ptr<Expr> right"
    ])


    