<?php
namespace Ds\Tests\Deque;

trait allocate
{
    public function allocateDataProvider()
    {
        // initial, allocation, expected capacity
        return [

            // Test minimum capacity (8)
            [0, 0, 8],
            [8, 0, 8],
            [8, 7, 8],

            // Test boundaries
            [16, 17, 32],
            [16, 16, 16],
            [16, 15, 16],
        ];
    }

    /**
     * @dataProvider allocateDataProvider
     */
    public function testAllocate(int $initial, int $allocate, int $expected)
    {
        $instance = $this->getInstance();

        $instance->allocate($initial);
        $instance->allocate($allocate);
        $this->assertEquals($expected, $instance->capacity());
    }
}
