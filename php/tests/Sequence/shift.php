<?php
namespace Ds\Tests\Sequence;

trait shift
{
    public function shiftDataProvider()
    {
        // initial, expected, result
        return [
            [['a'],           'a',    []],
            [['a', 'b'],      'a',    ['b']],
            [['a', 'b', 'c'], 'a',    ['b', 'c']],

            [range(1, self::MANY), 1, range(2, self::MANY)],
        ];
    }

    /**
     * @dataProvider shiftDataProvider
     */
    public function testShift(array $initial, $expected, array $result)
    {
        $instance = $this->getInstance($initial);

        $this->assertEquals($expected, $instance->shift());
        $this->assertToArray($result, $instance);
        $this->assertEquals(count($initial) - 1, count($instance));
    }

    public function testShiftNotAllowedWhenEmpty()
    {
        $instance = $this->getInstance();
        $this->expectEmptyNotAllowedException();
        $instance->shift();
    }

    public function testShiftAll()
    {
        $instance = $this->getInstance(range(1, self::MANY));

        while ( ! $instance->isEmpty()) {
            $instance->shift();
        }

        $this->assertEquals(count($instance), 0);
        $this->assertToArray([], $instance);
    }
}
