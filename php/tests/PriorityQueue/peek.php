<?php
namespace Ds\Tests\PriorityQueue;

trait peek
{
    public function peekDataProvider()
    {
        // initial, expected
        return [
            [['a' => 1, 'b' => 2], 'b'],
            [['a' => 2, 'b' => 1], 'a'],
            [['a' => 1, 'b' => 1], 'a'],
        ];
    }

    /**
     * @dataProvider peekDataProvider
     */
    public function testPeek(array $initial, $expected)
    {
        $instance = $this->getInstance($initial);
        $this->assertEquals($expected, $instance->peek());
        $this->assertCount(count($initial), $instance);
    }
}
