<?php

namespace Ds\Tests;

class PriorityQueueTest extends CollectionTest
{
    use PriorityQueue\__construct;
    use PriorityQueue\_clone;
    use PriorityQueue\_echo;
    use PriorityQueue\_empty;
    use PriorityQueue\_foreach;
    use PriorityQueue\_isset;
    use PriorityQueue\_jsonEncode;
    use PriorityQueue\_list;
    use PriorityQueue\_serialize;
    use PriorityQueue\_unset;
    use PriorityQueue\_var_dump;

    use PriorityQueue\allocate;
    use PriorityQueue\capacity;
    use PriorityQueue\clear;
    use PriorityQueue\copy;
    use PriorityQueue\count;
    use PriorityQueue\isEmpty;
    use PriorityQueue\peek;
    use PriorityQueue\pop;
    use PriorityQueue\push;
    use PriorityQueue\toArray;

    public function getInstance(array $values = [])
    {
        $queue = new \Ds\PriorityQueue();

        foreach ($values as $value => $priority) {
            $queue->push($value, $priority);
        }

        return $queue;
    }

    public function basicDataProvider()
    {
        return [
            [[], []],
            [['a' => 1],           ['a']],
            [['a' => 1, 'b' => 2], ['b', 'a']],
            [['a' => 2, 'b' => 1], ['a', 'b']],
        ];
    }
}
