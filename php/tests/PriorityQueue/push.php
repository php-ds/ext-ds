<?php
namespace Ds\Tests\PriorityQueue;

trait push
{
    public function pushDataProvider()
    {
        // initial, values, expected
        return [
            [[], ['a' => 1, 'b' => 2], ['b', 'a']],
            [[], ['a' => 1, 'b' => 1], ['a', 'b']],

            [['a' => 1], ['b' => 2], ['b', 'a']],
            [['a' => 1], ['b' => 1], ['a', 'b']],
        ];
    }

    /**
     * @dataProvider pushDataProvider
     */
    public function testPush(array $initial, array $values, array $expected)
    {
        $instance = $this->getInstance($initial);

        foreach ($values as $value => $priority) {
            $instance->push($value, $priority);
        }

        $this->assertToArray($expected, $instance);
    }

    public function testPushIdenticalValues()
    {
        $instance = $this->getInstance();

        $instance->push('a', 1);
        $instance->push('a', 1);
        $instance->push('a', 1);

        $this->assertToArray(['a', 'a', 'a'], $instance);
    }

    public function testPushManyRandom()
    {
        $instance = $this->getInstance();

        $reference = range(1, self::SOME);
        shuffle($reference);

        foreach ($reference as $index => $priority) {
            $instance->push($index, $priority);
        }

        asort($reference);
        $this->assertEmpty(array_diff_key($reference, $instance->toArray()));
    }

    public function testInsertionOrder()
    {
        $instance = $this->getInstance();

        foreach (range(1, self::MANY) as $i) {
            $instance->push($i, 0);
        }

        foreach (range(1, self::MANY) as $i) {
            $this->assertEquals($i, $instance->pop());
        }
    }
}
