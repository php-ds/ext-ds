<?php
namespace Ds\Tests\PriorityQueue;

trait pop
{
    public function popDataProvider()
    {
        // initial, expected, result
        return [
            [['a' => 1, 'b' => 2], 'b', ['a']],
            [['a' => 2, 'b' => 1], 'a', ['b']],
            [['a' => 1, 'b' => 1], 'a', ['b']],
        ];
    }

    /**
     * @dataProvider popDataProvider
     */
    public function testPop(array $initial, $expected, array $result)
    {
        $instance = $this->getInstance($initial);

        $this->assertEquals($expected, $instance->pop());
        $this->assertToArray($result, $instance);
    }

    public function testPopAll()
    {
        $instance = $this->getInstance(range(1, self::MANY));

        while ( ! $instance->isEmpty()) {
            $instance->pop();
        }

        $this->assertEquals(count($instance), 0);
    }

    public function testPopNowAllowedWhenEmpty()
    {
        $instance = $this->getInstance();
        $this->expectEmptyNotAllowedException();
        $instance->pop();
    }
}
