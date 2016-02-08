<?php

namespace Ds\Tests;

class DequeTest extends CollectionTest
{
    use Sequence\_clone;
    use Sequence\_echo;
    use Sequence\_empty;
    use Sequence\_foreach;
    use Sequence\_isset;
    use Sequence\_jsonEncode;
    use Sequence\_list;
    use Sequence\_serialize;
    use Sequence\_unset;
    use Sequence\_var_dump;

    use Deque\__construct;
    use Deque\allocate;
    use Deque\capacity;
    use Deque\insert;
    use Deque\remove;
    use Deque\slice;

    use Sequence\clear;
    use Sequence\contains;
    use Sequence\copy;
    use Sequence\count;
    use Sequence\filter;
    use Sequence\find;
    use Sequence\first;
    use Sequence\get;
    use Sequence\insert;
    use Sequence\isEmpty;
    use Sequence\join;
    use Sequence\last;
    use Sequence\map;
    use Sequence\pop;
    use Sequence\push;
    use Sequence\pushAll;
    use Sequence\reduce;
    use Sequence\remove;
    use Sequence\reverse;
    use Sequence\rotate;
    use Sequence\set;
    use Sequence\shift;
    use Sequence\slice;
    use Sequence\sort;
    use Sequence\toArray;
    use Sequence\unshift;

    protected function getInstance(array $values = [])
    {
        return new \Ds\Deque($values);
    }
}
