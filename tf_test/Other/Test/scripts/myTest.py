#!/usr/bin/env python
PACKAGE = "dynamic_tutorials"

# This first lines are pretty simple, they just initialize ros and import the parameter generator.
from dynamic_reconfigure.parameter_generator_catkin import *

# Now that we have a generator we can start to define parameters. The add function adds a parameter to the list of parameters. It takes a few different arguments:
gen = ParameterGenerator()

gen.add("int_param",    int_t,    0, "An Integer parameter", 50,  0, 100)
gen.add("double_param", double_t, 0, "A double parameter",    .5, 0,   1)
gen.add("str_param",    str_t,    0, "A string parameter",  "Hello World")
gen.add("bool_param",   bool_t,   0, "A Boolean parameter",  True)

size_enum = gen.enum([ gen.const("Small",      int_t, 0, "A small constant"),
                       gen.const("Medium",     int_t, 1, "A medium constant"),
                       gen.const("Large",      int_t, 2, "A large constant"),
                       gen.const("ExtraLarge", int_t, 3, "An extra large constant")],
                     "An enum to set size")

gen.add("size", int_t, 0, "A size parameter which is edited via an enum", 1, 0, 3, edit_method=size_enum)

exit(gen.generate(PACKAGE, "dynamic_tutorials", "Tutorials"))