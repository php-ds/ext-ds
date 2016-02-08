<?php
namespace Ds\Tests\Sequence;

trait _unset
{
    /**
     * @dataProvider removeDataProvider
     */
    public function testArrayAccessUnset($initial, $index, $return, array $expected)
    {
        $instance = $this->getInstance($initial);
        unset($instance[$index]);
        $this->assertToArray($expected, $instance);
        $this->assertEquals(count($expected), count($instance));
    }

    /**
     * @dataProvider badIndexDataProvider
     */
    public function testArrayAccessUnsetIndexBadIndex($initial, $index)
    {
        $instance = $this->getInstance($initial);
        unset($instance[$index]);
    }

    /**
     * @dataProvider outOfRangeDataProvider
     */
    public function testArrayAccessUnsetIndexOutOfRange($initial, $index)
    {
        $instance = $this->getInstance($initial);
        unset($instance[$index]);
    }


    public function testArrayAccessUnsetByReference()
    {
        $instance = $this->getInstance([[1]]);
        unset($instance[0][0]);

        $this->assertToArray([[]], $instance);
    }
}
