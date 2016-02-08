<?php
namespace Ds\Tests\Sequence;

trait _empty
{
    public function emptyDataProvider()
    {
        // initial, index, empty
        return [

            [[0], 0, true],

            [[null], 0, true],

            [['a'], 0, false],
            [['a'], 1, true],

            [['a', 'b'], 0, false],
            [['a', 'b'], 1, false],
            [['a', 'b'], 2, true],

            [[null, 'b'], 0, true],
            [[null, 'b'], 1, false],

            [['a', null], 1, true],
            [['a', null], 0, false],

            [['a', 'b', 'c'], 0, false],
            [['a', 'b', 'c'], 1, false],
            [['a', 'b', 'c'], 2, false],
            [['a', 'b', 'c'], 3, true],

            [[null, 'b', 'c'], 0, true],
            [['a', null, 'c'], 1, true],
            [['a', 'b', null], 2, true],
        ];
    }

    /**
     * @dataProvider emptyDataProvider
     */
    public function testArrayAccessEmpty($initial, $index, bool $empty)
    {
        $instance = $this->getInstance($initial);
        $this->assertEquals($empty, empty($instance[$index]));
    }

    /**
     * @dataProvider badIndexDataProvider
     */
    public function testArrayAccessEmptyIndexBadIndex($initial, $index)
    {
        $instance = $this->getInstance($initial);
        empty($instance[$index]);
    }

    /**
     * @dataProvider outOfRangeDataProvider
     */
    public function testArrayAccessEmptyIndexOutOfRange($initial, $index)
    {
        $instance = $this->getInstance($initial);
        empty($instance[$index]);
    }

    /**
     * @dataProvider emptyDataProvider
     */
    public function testArrayAccessEmptyByReference($initial, $index, bool $empty)
    {
        $instance = $this->getInstance([$initial]);
        $this->assertEquals($empty, empty($instance[0][$index]));
    }
}
