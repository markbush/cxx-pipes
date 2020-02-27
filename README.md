# cxx-pipes
Functional pipelines in C++

Construct pipes from containers:

```c++
    std::vector<int> v {1, 2, 3, 4, 5};
    Pipe<int> pipe {v};
    std::array<int,5> a {1, 2, 3, 4, 5};
    Pipe<int> pipe {a};
```

String operations together in a pipeline:

```c++
pipe.map<int>([](int i){return i*2;});
pipe.filter([](int i){return i%2==0;}).map<int>([](int i){return i*2;});
pipe.find([](int i){return i%2==0;});
pipe.takeWhile([](int i){return i<4;});
```

Collect the results:

```c++
pipe.toVector();
pipe.toArray<4>();
pipe.toSet();
pipe.max();
pipe.collect(0, [](int z, int i){return z+i;});
pipe.join(", ");
```

### Note

Converting to an array is only useful when you can be certain how many elements you have.

## Full API

See the tests for examples of all of the available methods.
