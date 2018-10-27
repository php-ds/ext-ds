--TEST--
Decimal object reflection
--SKIPIF--
<?php
if (!extension_loaded("decimal")) echo "skip";
?>
--FILE--
<?php

/**
 * Decimal class.
 */
$class = new ReflectionClass(Decimal\Decimal::class);

/**
 * Constants
 */
var_dump($class->getConstants());

/**
 * Properties
 */
var_dump($class->getProperties());

/**
 * Method summary.
 */
function print_method_summary(ReflectionClass $class, string $method)
{
    $method = $class->getMethod($method);

    printf("%s\n", $method->getName());
    printf("Number of parameters: %d\n", $method->getNumberOfParameters());
    printf("Number of required parameters: %d\n", $method->getNumberOfRequiredParameters());
    printf("Return type: %s\n", $method->hasReturnType() ? $method->getReturnType() : "void");
    printf("Allows return null? %s\n", $method->hasReturnType() ? ($method->getReturnType()->allowsNull() ? "yes" : "no") : "no");
    printf("Returns reference? %s\n", $method->returnsReference() ? "yes" : "no");
    printf("Parameters:\n");
    foreach ($method->getParameters() as $parameter) {
        printf("Name: %s\n", $parameter->getName());
        printf("Type: %s\n", ((string) $parameter->getType()) ?: "mixed");
        printf("Reference? %s\n", $parameter->isPassedByReference() ? "yes" : "no");
        printf("Allows null? %s\n", $parameter->allowsNull() ? "yes" : "no");
        printf("Optional? %s\n", $parameter->isOptional() ? "yes" : "no");
    }
    printf("\n");
}

print_method_summary($class, "__construct");
print_method_summary($class, "copy");
print_method_summary($class, "add");
print_method_summary($class, "sub");
print_method_summary($class, "mul");
print_method_summary($class, "div");
print_method_summary($class, "rem");
print_method_summary($class, "mod");
print_method_summary($class, "pow");
print_method_summary($class, "ln");
print_method_summary($class, "exp");
print_method_summary($class, "log10");
print_method_summary($class, "sqrt");
print_method_summary($class, "floor");
print_method_summary($class, "ceil");
print_method_summary($class, "truncate");
print_method_summary($class, "round");
print_method_summary($class, "shift");
print_method_summary($class, "precision");
print_method_summary($class, "signum");
print_method_summary($class, "parity");
print_method_summary($class, "abs");
print_method_summary($class, "negate");
print_method_summary($class, "isEven");
print_method_summary($class, "isOdd");
print_method_summary($class, "isPositive");
print_method_summary($class, "isNegative");
print_method_summary($class, "isNaN");
print_method_summary($class, "isInf");
print_method_summary($class, "isInteger");
print_method_summary($class, "isZero");
print_method_summary($class, "toFixed");
print_method_summary($class, "toString");
print_method_summary($class, "toInt");
print_method_summary($class, "toFloat");
print_method_summary($class, "equals");
print_method_summary($class, "compareTo");
print_method_summary($class, "sum");
print_method_summary($class, "avg");
print_method_summary($class, "__toString");
print_method_summary($class, "jsonSerialize");

?>
--EXPECTF--
array(13) {
  ["ROUND_UP"]=>
  int(101)
  ["ROUND_DOWN"]=>
  int(102)
  ["ROUND_CEILING"]=>
  int(103)
  ["ROUND_FLOOR"]=>
  int(104)
  ["ROUND_HALF_UP"]=>
  int(105)
  ["ROUND_HALF_DOWN"]=>
  int(106)
  ["ROUND_HALF_EVEN"]=>
  int(107)
  ["ROUND_HALF_ODD"]=>
  int(108)
  ["ROUND_TRUNCATE"]=>
  int(109)
  ["DEFAULT_PRECISION"]=>
  int(28)
  ["DEFAULT_ROUNDING"]=>
  int(107)
  ["MIN_PRECISION"]=>
  int(1)
  ["MAX_PRECISION"]=>
  int(%d)
}
array(0) {
}
__construct
Number of parameters: 2
Number of required parameters: 0
Return type: void
Allows return null? no
Returns reference? no
Parameters:
Name: value
Type: mixed
Reference? no
Allows null? no
Optional? yes
Name: precision
Type: int
Reference? no
Allows null? no
Optional? yes

copy
Number of parameters: 0
Number of required parameters: 0
Return type: Decimal\Decimal
Allows return null? no
Returns reference? no
Parameters:

add
Number of parameters: 1
Number of required parameters: 1
Return type: Decimal\Decimal
Allows return null? no
Returns reference? no
Parameters:
Name: value
Type: mixed
Reference? no
Allows null? no
Optional? no

sub
Number of parameters: 1
Number of required parameters: 1
Return type: Decimal\Decimal
Allows return null? no
Returns reference? no
Parameters:
Name: value
Type: mixed
Reference? no
Allows null? no
Optional? no

mul
Number of parameters: 1
Number of required parameters: 1
Return type: Decimal\Decimal
Allows return null? no
Returns reference? no
Parameters:
Name: value
Type: mixed
Reference? no
Allows null? no
Optional? no

div
Number of parameters: 1
Number of required parameters: 1
Return type: Decimal\Decimal
Allows return null? no
Returns reference? no
Parameters:
Name: value
Type: mixed
Reference? no
Allows null? no
Optional? no

rem
Number of parameters: 1
Number of required parameters: 1
Return type: Decimal\Decimal
Allows return null? no
Returns reference? no
Parameters:
Name: value
Type: mixed
Reference? no
Allows null? no
Optional? no

mod
Number of parameters: 1
Number of required parameters: 1
Return type: Decimal\Decimal
Allows return null? no
Returns reference? no
Parameters:
Name: value
Type: mixed
Reference? no
Allows null? no
Optional? no

pow
Number of parameters: 1
Number of required parameters: 1
Return type: Decimal\Decimal
Allows return null? no
Returns reference? no
Parameters:
Name: value
Type: mixed
Reference? no
Allows null? no
Optional? no

ln
Number of parameters: 0
Number of required parameters: 0
Return type: Decimal\Decimal
Allows return null? no
Returns reference? no
Parameters:

exp
Number of parameters: 0
Number of required parameters: 0
Return type: Decimal\Decimal
Allows return null? no
Returns reference? no
Parameters:

log10
Number of parameters: 0
Number of required parameters: 0
Return type: Decimal\Decimal
Allows return null? no
Returns reference? no
Parameters:

sqrt
Number of parameters: 0
Number of required parameters: 0
Return type: Decimal\Decimal
Allows return null? no
Returns reference? no
Parameters:

floor
Number of parameters: 0
Number of required parameters: 0
Return type: Decimal\Decimal
Allows return null? no
Returns reference? no
Parameters:

ceil
Number of parameters: 0
Number of required parameters: 0
Return type: Decimal\Decimal
Allows return null? no
Returns reference? no
Parameters:

truncate
Number of parameters: 0
Number of required parameters: 0
Return type: Decimal\Decimal
Allows return null? no
Returns reference? no
Parameters:

round
Number of parameters: 2
Number of required parameters: 0
Return type: Decimal\Decimal
Allows return null? no
Returns reference? no
Parameters:
Name: places
Type: int
Reference? no
Allows null? yes
Optional? yes
Name: rounding
Type: int
Reference? no
Allows null? yes
Optional? yes

shift
Number of parameters: 1
Number of required parameters: 1
Return type: Decimal\Decimal
Allows return null? no
Returns reference? no
Parameters:
Name: places
Type: int
Reference? no
Allows null? no
Optional? no

precision
Number of parameters: 0
Number of required parameters: 0
Return type: int
Allows return null? no
Returns reference? no
Parameters:

signum
Number of parameters: 0
Number of required parameters: 0
Return type: int
Allows return null? no
Returns reference? no
Parameters:

parity
Number of parameters: 0
Number of required parameters: 0
Return type: int
Allows return null? no
Returns reference? no
Parameters:

abs
Number of parameters: 0
Number of required parameters: 0
Return type: Decimal\Decimal
Allows return null? no
Returns reference? no
Parameters:

negate
Number of parameters: 0
Number of required parameters: 0
Return type: Decimal\Decimal
Allows return null? no
Returns reference? no
Parameters:

isEven
Number of parameters: 0
Number of required parameters: 0
Return type: bool
Allows return null? no
Returns reference? no
Parameters:

isOdd
Number of parameters: 0
Number of required parameters: 0
Return type: bool
Allows return null? no
Returns reference? no
Parameters:

isPositive
Number of parameters: 0
Number of required parameters: 0
Return type: bool
Allows return null? no
Returns reference? no
Parameters:

isNegative
Number of parameters: 0
Number of required parameters: 0
Return type: bool
Allows return null? no
Returns reference? no
Parameters:

isNaN
Number of parameters: 0
Number of required parameters: 0
Return type: bool
Allows return null? no
Returns reference? no
Parameters:

isInf
Number of parameters: 0
Number of required parameters: 0
Return type: bool
Allows return null? no
Returns reference? no
Parameters:

isInteger
Number of parameters: 0
Number of required parameters: 0
Return type: bool
Allows return null? no
Returns reference? no
Parameters:

isZero
Number of parameters: 0
Number of required parameters: 0
Return type: bool
Allows return null? no
Returns reference? no
Parameters:

toFixed
Number of parameters: 3
Number of required parameters: 0
Return type: string
Allows return null? no
Returns reference? no
Parameters:
Name: places
Type: int
Reference? no
Allows null? yes
Optional? yes
Name: commas
Type: bool
Reference? no
Allows null? yes
Optional? yes
Name: rounding
Type: int
Reference? no
Allows null? yes
Optional? yes

toString
Number of parameters: 0
Number of required parameters: 0
Return type: string
Allows return null? no
Returns reference? no
Parameters:

toInt
Number of parameters: 0
Number of required parameters: 0
Return type: int
Allows return null? no
Returns reference? no
Parameters:

toFloat
Number of parameters: 0
Number of required parameters: 0
Return type: float
Allows return null? no
Returns reference? no
Parameters:

equals
Number of parameters: 1
Number of required parameters: 1
Return type: bool
Allows return null? no
Returns reference? no
Parameters:
Name: other
Type: mixed
Reference? no
Allows null? no
Optional? no

compareTo
Number of parameters: 1
Number of required parameters: 1
Return type: int
Allows return null? no
Returns reference? no
Parameters:
Name: other
Type: mixed
Reference? no
Allows null? no
Optional? no

sum
Number of parameters: 2
Number of required parameters: 1
Return type: Decimal\Decimal
Allows return null? no
Returns reference? no
Parameters:
Name: values
Type: mixed
Reference? no
Allows null? no
Optional? no
Name: precision
Type: int
Reference? no
Allows null? yes
Optional? yes

avg
Number of parameters: 2
Number of required parameters: 1
Return type: Decimal\Decimal
Allows return null? no
Returns reference? no
Parameters:
Name: values
Type: mixed
Reference? no
Allows null? no
Optional? no
Name: precision
Type: int
Reference? no
Allows null? yes
Optional? yes

__toString
Number of parameters: 0
Number of required parameters: 0
Return type: string
Allows return null? no
Returns reference? no
Parameters:

jsonSerialize
Number of parameters: 0
Number of required parameters: 0
Return type: string
Allows return null? no
Returns reference? no
Parameters:
