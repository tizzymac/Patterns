
// Looking into Monods

#include <string>
#include <iostream>

template <typename T>
struct Maybe;

template <typename T>
Maybe<T> maybe_maker(T *context)
{
    return Maybe<T>(context);
};

template <typename T>
struct Maybe
{
    T *context;

    explicit Maybe(T *const context) 
        : context{context}
    {
    }

    template <typename Func>
    auto With(Func evaluator)
    {
        if (context == nullptr)
        {
            // return the Maybe of same type we would get from the evaluation minus the pointer
            return Maybe<typename std::remove_pointer<decltype(evaluator(context))>::type>(nullptr);
        }
        else
        {
            return maybe_maker(evaluator(context));
        }
    }

    template <typename Func>
    auto Do(Func action)
    {
        if (context != nullptr)
        {
            action(context);
            return *this;
        }
    }
};

struct Address
{
    std::string *house_name = nullptr;
};

struct Person
{
    Address *address = nullptr;
};

void PrintHouseName(Person *person)
{
    maybe_maker(person)
        .With([](auto person)
              { return person->address; })
        .With([](auto address)
              { return address->house_name; })
        .Do([](auto x)
            { std::cout << x << std::endl; });
}

int main()
{
    Person bob;
    PrintHouseName(&bob);

    getchar();
    return 0;
}
