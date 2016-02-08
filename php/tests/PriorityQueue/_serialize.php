<?php
namespace Ds\Tests\PriorityQueue;

trait _serialize
{
    public function serializeDataProvider()
    {
        return [
            [
                ['a' => 1, 'b' => 2], ['b' => 2, 'a' => 1]
            ],
        ];
    }

    /**
     * @dataProvider serializeDataProvider
     */
    public function testSerialize(array $values, array $expected)
    {
        $instance = $this->getInstance($values);
        $this->assertSerialized($expected, $instance, true);
    }
}
