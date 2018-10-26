--TEST--
Decimal object refecltion
--SKIPIF--
<?php
if (!extension_loaded("decimal")) echo "skip";
?>
--FILE--
<?php
ReflectionClass::export(Decimal\Decimal::class);
?>
--EXPECTF--
Class [ <internal:decimal> final class Decimal\Decimal implements JsonSerializable ] {

  - Constants [13] {
    Constant [ public integer ROUND_UP ] { 101 }
    Constant [ public integer ROUND_DOWN ] { 102 }
    Constant [ public integer ROUND_CEILING ] { 103 }
    Constant [ public integer ROUND_FLOOR ] { 104 }
    Constant [ public integer ROUND_HALF_UP ] { 105 }
    Constant [ public integer ROUND_HALF_DOWN ] { 106 }
    Constant [ public integer ROUND_HALF_EVEN ] { 107 }
    Constant [ public integer ROUND_HALF_ODD ] { 108 }
    Constant [ public integer ROUND_TRUNCATE ] { 109 }
    Constant [ public integer DEFAULT_PRECISION ] { 28 }
    Constant [ public integer DEFAULT_ROUNDING ] { 107 }
    Constant [ public integer MIN_PRECISION ] { 1 }
    Constant [ public integer MAX_PRECISION ] { %d }
  }

  - Static properties [0] {
  }

  - Static methods [2] {
    Method [ <internal:decimal> static public method sum ] {

      - Parameters [2] {
        Parameter #0 [ <required> $values ]
        Parameter #1 [ <optional> integer or NULL $precision ]
      }
      - Return [ Decimal\Decimal ]
    }

    Method [ <internal:decimal> static public method avg ] {

      - Parameters [2] {
        Parameter #0 [ <required> $values ]
        Parameter #1 [ <optional> integer or NULL $precision ]
      }
      - Return [ Decimal\Decimal ]
    }
  }

  - Properties [0] {
  }

  - Methods [39] {
    Method [ <internal:decimal, ctor> public method __construct ] {

      - Parameters [2] {
        Parameter #0 [ <optional> $value ]
        Parameter #1 [ <optional> integer $precision ]
      }
    }

    Method [ <internal:decimal> public method copy ] {

      - Parameters [0] {
      }
      - Return [ Decimal\Decimal ]
    }

    Method [ <internal:decimal> public method add ] {

      - Parameters [1] {
        Parameter #0 [ <required> $value ]
      }
      - Return [ Decimal\Decimal ]
    }

    Method [ <internal:decimal> public method sub ] {

      - Parameters [1] {
        Parameter #0 [ <required> $value ]
      }
      - Return [ Decimal\Decimal ]
    }

    Method [ <internal:decimal> public method mul ] {

      - Parameters [1] {
        Parameter #0 [ <required> $value ]
      }
      - Return [ Decimal\Decimal ]
    }

    Method [ <internal:decimal> public method div ] {

      - Parameters [1] {
        Parameter #0 [ <required> $value ]
      }
      - Return [ Decimal\Decimal ]
    }

    Method [ <internal:decimal> public method rem ] {

      - Parameters [1] {
        Parameter #0 [ <required> $value ]
      }
      - Return [ Decimal\Decimal ]
    }

    Method [ <internal:decimal> public method mod ] {

      - Parameters [1] {
        Parameter #0 [ <required> $value ]
      }
      - Return [ Decimal\Decimal ]
    }

    Method [ <internal:decimal> public method pow ] {

      - Parameters [1] {
        Parameter #0 [ <required> $value ]
      }
      - Return [ Decimal\Decimal ]
    }

    Method [ <internal:decimal> public method ln ] {

      - Parameters [0] {
      }
      - Return [ Decimal\Decimal ]
    }

    Method [ <internal:decimal> public method exp ] {

      - Parameters [0] {
      }
      - Return [ Decimal\Decimal ]
    }

    Method [ <internal:decimal> public method log10 ] {

      - Parameters [0] {
      }
      - Return [ Decimal\Decimal ]
    }

    Method [ <internal:decimal> public method sqrt ] {

      - Parameters [0] {
      }
      - Return [ Decimal\Decimal ]
    }

    Method [ <internal:decimal> public method floor ] {

      - Parameters [0] {
      }
      - Return [ Decimal\Decimal ]
    }

    Method [ <internal:decimal> public method ceil ] {

      - Parameters [0] {
      }
      - Return [ Decimal\Decimal ]
    }

    Method [ <internal:decimal> public method truncate ] {

      - Parameters [0] {
      }
      - Return [ Decimal\Decimal ]
    }

    Method [ <internal:decimal> public method round ] {

      - Parameters [2] {
        Parameter #0 [ <optional> integer or NULL $places ]
        Parameter #1 [ <optional> integer or NULL $rounding ]
      }
      - Return [ Decimal\Decimal ]
    }

    Method [ <internal:decimal> public method shift ] {

      - Parameters [1] {
        Parameter #0 [ <required> integer $places ]
      }
      - Return [ Decimal\Decimal ]
    }

    Method [ <internal:decimal> public method precision ] {

      - Parameters [0] {
      }
      - Return [ integer ]
    }

    Method [ <internal:decimal> public method signum ] {

      - Parameters [0] {
      }
      - Return [ integer ]
    }

    Method [ <internal:decimal> public method parity ] {

      - Parameters [0] {
      }
      - Return [ integer ]
    }

    Method [ <internal:decimal> public method abs ] {

      - Parameters [0] {
      }
      - Return [ Decimal\Decimal ]
    }

    Method [ <internal:decimal> public method negate ] {

      - Parameters [0] {
      }
      - Return [ Decimal\Decimal ]
    }

    Method [ <internal:decimal> public method isEven ] {

      - Parameters [0] {
      }
      - Return [ boolean ]
    }

    Method [ <internal:decimal> public method isOdd ] {

      - Parameters [0] {
      }
      - Return [ boolean ]
    }

    Method [ <internal:decimal> public method isPositive ] {

      - Parameters [0] {
      }
      - Return [ boolean ]
    }

    Method [ <internal:decimal> public method isNegative ] {

      - Parameters [0] {
      }
      - Return [ boolean ]
    }

    Method [ <internal:decimal> public method isNaN ] {

      - Parameters [0] {
      }
      - Return [ boolean ]
    }

    Method [ <internal:decimal> public method isInf ] {

      - Parameters [0] {
      }
      - Return [ boolean ]
    }

    Method [ <internal:decimal> public method isInteger ] {

      - Parameters [0] {
      }
      - Return [ boolean ]
    }

    Method [ <internal:decimal> public method isZero ] {

      - Parameters [0] {
      }
      - Return [ boolean ]
    }

    Method [ <internal:decimal> public method toFixed ] {

      - Parameters [3] {
        Parameter #0 [ <optional> integer or NULL $places ]
        Parameter #1 [ <optional> boolean or NULL $commas ]
        Parameter #2 [ <optional> integer or NULL $rounding ]
      }
      - Return [ string ]
    }

    Method [ <internal:decimal> public method toString ] {

      - Parameters [0] {
      }
      - Return [ string ]
    }

    Method [ <internal:decimal> public method toInt ] {

      - Parameters [0] {
      }
      - Return [ integer ]
    }

    Method [ <internal:decimal> public method toFloat ] {

      - Parameters [0] {
      }
      - Return [ float ]
    }

    Method [ <internal:decimal> public method equals ] {

      - Parameters [1] {
        Parameter #0 [ <required> $other ]
      }
      - Return [ boolean ]
    }

    Method [ <internal:decimal> public method compareTo ] {

      - Parameters [1] {
        Parameter #0 [ <required> $other ]
      }
      - Return [ integer ]
    }

    Method [ <internal:decimal> public method __toString ] {

      - Parameters [0] {
      }
      - Return [ string ]
    }

    Method [ <internal:decimal, prototype JsonSerializable> public method jsonSerialize ] {

      - Parameters [0] {
      }
      - Return [ string ]
    }
  }
}
