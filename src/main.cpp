#include <iostream>
#include <rttr/registration>
using namespace rttr;

struct MyStruct
{
    MyStruct(){};
    void func(double input){
        std::cout << "hi from func in a MyStruct instance with parameter: " << input << std::endl;
    };
    int data;
};

/*
    Manual registration
    Register your properties, methods or constructors in your source files.

    Adjusting the class interface is only necessary when using inheritance.
*/
RTTR_REGISTRATION
{
    registration::class_<MyStruct>("MyStruct")
        .constructor<>()
        .property("data", &MyStruct::data)
        .method("func", &MyStruct::func);
}

int main()
{
    {
        /*
            Iterate over members
            With the type object you can query your previously registered members.

            Of course including all previously registered base classes.
        */
        type t = type::get<MyStruct>();
        for (auto &prop : t.get_properties())
        {
            std::cout << "name: " << prop.get_name() << std::endl;
        }

        for (auto &meth : t.get_methods())
        {
            std::cout << "name: " << meth.get_name() << std::endl;
        }
    }

    {
        /*
            Constructing types
            You don't have to include the actual class declaration to work with a type.

            Via the type object or constructor object you can create instances of your type, wrapped inside a variant object.
        */
        type t = type::get_by_name("MyStruct");
        variant var = t.create(); // will invoke the previously registered ctor

        constructor ctor = t.get_constructor(); // 2nd way with the constructor class
        var = ctor.invoke();
        std::cout << var.get_type().get_name() << std::endl; // prints 'MyStruct'
    }

    {
        /*
            Set/get properties
            It is pretty simple to set or get a property. Take a look at the following code:
        */
        MyStruct obj;
        property prop = type::get(obj).get_property("data");
        prop.set_value(obj, 23);

        variant var_prop = prop.get_value(obj);
        std::cout << var_prop.to_int() << std::endl; // prints '23'
    }

    {
        /*
            Invoke methods
            Calling a method is straight forward.

            It is also possible to use the variant object to invoke a method.
        */
        MyStruct obj;
        method meth = type::get(obj).get_method("func");
        meth.invoke(obj, 42.0);

        variant var = type::get(obj).create();
        meth.invoke(var, 45.0);
    }

    std::cin.get();
    return 0;
}