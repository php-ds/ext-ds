<?php

namespace Ds\Tests;

use Ds\Collection;

class StackTest extends CollectionTest
{
    use Stack\__construct;
    use Stack\_clone;
    use Stack\_echo;
    use Stack\_empty;
    use Stack\_foreach;
    use Stack\_isset;
    use Stack\_jsonEncode;
    use Stack\_list;
    use Stack\_serialize;
    use Stack\_unset;
    use Stack\_var_dump;

    use Vector\allocate;
    use Vector\capacity;

    use Stack\clear;
    use Stack\copy;
    use Stack\count;
    use Stack\isEmpty;
    use Stack\peek;
    use Stack\pop;
    use Stack\push;
    use Stack\pushAll;
    use Stack\toArray;

    protected function getInstance(array $values = [])
    {
        return new \Ds\Stack($values);
    }

    public function basicDataProvider()
    {
        // Stack should produce values in reverse order.
        return array_map(function($data) {
            return [$data[0], array_reverse($data[1])];
        }, parent::basicDataProvider());
    }

    public function serializeDataProvider()
    {
        // Stack should serialize in push order, so that it can be
        // unserialized by pushing each serialized value.
        return parent::basicDataProvider();
    }
}
