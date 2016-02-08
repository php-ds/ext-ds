<?php

namespace Ds\Tests;

class PairTest extends CollectionTest
{
    use Pair\__construct;
    use Pair\__get;
    use Pair\__set;
    use Pair\_clone;
    use Pair\_echo;
    use Pair\_empty;
    use Pair\_isset;
    use Pair\_jsonEncode;
    use Pair\_list;
    use Pair\_serialize;
    use Pair\_unset;
    use Pair\_var_dump;

    use Pair\toArray;

    private function getPair($key, $value)
    {
        return new \Ds\Pair($key, $value);
    }
}
