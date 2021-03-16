# Class Design

- Start modelling your class such value-type i.e. `int` and benefit `rule of zero`.

- Prefer composition over inheritance.

- Prefer non-member non-friend function over member functions. Provide those `nonmember non-friend` functions in the same namespace of the class.

- Defining/disabling one of `copy ctor`, `move ctor` and `destructor`, disables implicit definition of others so if you have to define/disable one them, define/disable remaining ones.

-  Consider support for a non-throwing swap

- Avoid providing implicit conversions.
