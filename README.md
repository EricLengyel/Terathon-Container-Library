# Terathon-Container-Library

This is a C++ library that implements various types of containers. The specific classes are the following:

* **Array** – A growable array.
* **List** - A linked list.
* **Map** - A key-value map.
* **HashTable** - A hash table.
* **Tree** - A tree structure.
* **Graph** - A general graph structure.

With the exception of the array, these containers are all *intrusive*, meaning that objects that can be stored in each container inherit from helper classes that take care of all the bookkeeping. For example, if you have a class called `Object` that you want to store in a linked list, then `Object` must have `ListElement<Object>` as a base class, and it can then be stored in a container of type `List<Object>`.

## API Documentation

There is API documentation embedded in the header files. The formatted equivalent can be found in the [C4 Engine documentation](https://c4engine.com/docs/Utilities/index.html).

## Licensing

Separate proprietary licenses are available from Terathon Software. Please [send an email](mailto:service@terathon.com) with details about your particular use case if you are interested.
