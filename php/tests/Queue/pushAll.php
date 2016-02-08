<?php
namespace Ds\Tests\Queue;

trait pushAll
{
    /**
     * @dataProvider pushDataProvider
     */
    public function testPushAll(array $values, array $expected)
    {
        $instance = $this->getInstance();
        $instance->pushAll($values);
        $this->assertToArray($expected, $instance);
    }

    /**
     * @dataProvider pushDataProvider
     */
    public function testPushAllUsingIterator(array $values, array $expected)
    {
        $instance = $this->getInstance();
        $instance->pushAll(new \ArrayIterator($values));
        $this->assertToArray($expected, $instance);
    }

    /**
     * @dataProvider pushDataProvider
     */
    public function testPushAllFromSet(array $values, array $expected)
    {
        $instance = $this->getInstance();
        $set = new \Ds\Set($values);
        $instance->pushAll($set);
        $this->assertToArray($expected, $instance);
    }

    /**
     * @dataProvider pushDataProvider
     */
    public function testPushAllFromMap(array $values, array $expected)
    {
        $instance = $this->getInstance();
        $map = new \Ds\Map($values);
        $instance->pushAll($map);
        $this->assertToArray($expected, $instance);
    }

    /**
     * @dataProvider pushDataProvider
     */
    public function testPushAllFromVector(array $values, array $expected)
    {
        $instance = $this->getInstance();
        $vector = new \Ds\Vector($values);
        $instance->pushAll($vector);
        $this->assertToArray($expected, $instance);
    }

    /**
     * @dataProvider pushDataProvider
     */
    public function testPushAllFromDeque(array $values, array $expected)
    {
        $instance = $this->getInstance();
        $deque = new \Ds\Deque($values);
        $instance->pushAll($deque);
        $this->assertToArray($expected, $instance);
    }

    /**
     * @dataProvider pushDataProvider
     */
    public function testPushAllFromStack(array $values, array $expected)
    {
        $instance = $this->getInstance();
        $stack = new \Ds\Stack(array_reverse($values));
        $instance->pushAll($stack);
        $this->assertToArray($expected, $instance);
        $this->assertCount(0, $stack);
    }

    /**
     * @dataProvider pushDataProvider
     */
    public function testPushAllFromQueue(array $values, array $expected)
    {
        $instance = $this->getInstance();
        $queue = new \Ds\Queue($values);
        $instance->pushAll($queue);
        $this->assertToArray($expected, $instance);
        $this->assertCount(0, $queue);
    }

    /**
     * @dataProvider pushDataProvider
     */
    public function testPushAllFromPriorityQueue(array $values, array $expected)
    {
        $instance = $this->getInstance();
        $queue = new \Ds\PriorityQueue();

        foreach ($values as $value) {
            $queue->push($value, 0);
        }

        $instance->pushAll($queue);
        $this->assertToArray($expected, $instance);
        $this->assertCount(0, $queue);
    }
}
