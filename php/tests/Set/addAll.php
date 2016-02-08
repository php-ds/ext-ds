<?php
namespace Ds\Tests\Set;

trait addAll
{
    /**
     * @dataProvider addDataProvider
     */
    public function testAddAll(array $initial, array $values, array $expected)
    {
        $instance = $this->getInstance($initial);
        $instance->addAll($values);
        $this->assertToArray($expected, $instance);
    }

    /**
     * @dataProvider addDataProvider
     */
    public function testAddAllUsingIterator(array $initial, array $values, array $expected)
    {
        $instance = $this->getInstance($initial);
        $instance->addAll(new \ArrayIterator($values));
        $this->assertToArray($expected, $instance);
    }

    /**
     * @dataProvider addDataProvider
     */
    public function testAddAllFromSet(array $initial, array $values, array $expected)
    {
        $instance = $this->getInstance($initial);
        $set = new \Ds\Set($values);
        $instance->addAll($set);
        $this->assertToArray($expected, $instance);
    }

    /**
     * @dataProvider addDataProvider
     */
    public function testAddAllFromMap(array $initial, array $values, array $expected)
    {
        $instance = $this->getInstance($initial);
        $map = new \Ds\Map($values);
        $instance->addAll($map);
        $this->assertToArray($expected, $instance);
    }

    /**
     * @dataProvider addDataProvider
     */
    public function testAddAllFromVector(array $initial, array $values, array $expected)
    {
        $instance = $this->getInstance($initial);
        $vector = new \Ds\Vector($values);
        $instance->addAll($vector);
        $this->assertToArray($expected, $instance);
    }

    /**
     * @dataProvider addDataProvider
     */
    public function testAddAllFromDeque(array $initial, array $values, array $expected)
    {
        $instance = $this->getInstance($initial);
        $deque = new \Ds\Deque($values);
        $instance->addAll($deque);
        $this->assertToArray($expected, $instance);
    }

    /**
     * @dataProvider addDataProvider
     */
    public function testAddAllFromStack(array $initial, array $values, array $expected)
    {
        $instance = $this->getInstance($initial);
        $stack = new \Ds\Stack(array_reverse($values));
        $instance->addAll($stack);
        $this->assertToArray($expected, $instance);
        $this->assertCount(0, $stack);
    }

    /**
     * @dataProvider addDataProvider
     */
    public function testAddAllFromQueue(array $initial, array $values, array $expected)
    {
        $instance = $this->getInstance($initial);
        $queue = new \Ds\Queue($values);
        $instance->addAll($queue);
        $this->assertToArray($expected, $instance);
        $this->assertCount(0, $queue);
    }

    /**
     * @dataProvider addDataProvider
     */
    public function testAddAllFromPriorityQueue(array $initial, array $values, array $expected)
    {
        $instance = $this->getInstance($initial);
        $queue = new \Ds\PriorityQueue();

        foreach ($values as $value) {
            $queue->push($value, 0);
        }

        $instance->addAll($queue);
        $this->assertToArray($expected, $instance);
        $this->assertCount(0, $queue);
    }
}
