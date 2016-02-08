<?php
namespace Ds\Tests\Sequence;

trait set
{
    public function setDataProvider()
    {
        // initial, index, value, expected
        return [

            [['a'], 0, 'x', ['x']],

            [['a', 'b'], 0, 'x', ['x', 'b']],
            [['a', 'b'], 1, 'y', ['a', 'y']],

            [['a', 'b', 'c'], 0, 'x', ['x', 'b', 'c']],
            [['a', 'b', 'c'], 1, 'y', ['a', 'y', 'c']],
            [['a', 'b', 'c'], 2, 'z', ['a', 'b', 'z']],
        ];
    }


    /**
     * @dataProvider setDataProvider
     */
    public function testSet($initial, $index, $value, array $expected)
    {
        $instance = $this->getInstance($initial);

        $instance->set($index, $value);

        $this->assertToArray($expected, $instance);

        // set should not affect count
        $this->assertEquals(count($initial), count($instance));
    }

    /**
     * @dataProvider outOfRangeDataProvider
     */
    public function testSetOutOfRange($initial, $index)
    {
        $instance = $this->getInstance($initial);
        $this->expectIndexOutOfRangeException();
        $instance->set($index, 1);
    }

    /**
     * @dataProvider badIndexDataProvider
     */
    public function testSetIndexBadIndex($initial, $index)
    {
        $instance = $this->getInstance();
        $this->expectWrongIndexTypeException();
        $instance->set($index, 1);
    }

    /**
     * @dataProvider setDataProvider
     */
    public function testArrayAccessSet($initial, $index, $value, array $expected)
    {
        $instance = $this->getInstance($initial);
        $instance[$index] = $value;
        $this->assertToArray($expected, $instance);
        $this->assertEquals(count($expected), count($instance));
    }

    /**
     * @dataProvider badIndexDataProvider
     */
    public function testArrayAccessSetIndexBadIndex($initial, $index)
    {
        $instance = $this->getInstance($initial);
        $this->expectWrongIndexTypeException();
        $instance[$index] = 1;
    }

    /**
     * @dataProvider outOfRangeDataProvider
     */
    public function testArrayAccessSetIndexOutOfRange($initial, $index)
    {
        $instance = $this->getInstance($initial);
        $this->expectIndexOutOfRangeException();
        $instance[$index] = 1;
    }


    public function testArrayAccessSetByReference()
    {
        $instance = $this->getInstance([[1]]);
        $instance[0][0] = 2;

        $this->assertToArray([[2]], $instance);
    }
}
