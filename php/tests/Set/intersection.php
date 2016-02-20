<?php
namespace Ds\Tests\Set;

trait intersection
{
    public function intersectionDataProvider()
    {
        // Values in A that are also in B.
        // A, B, expected result
        return [
            [[],                [],         []],
            [['a'],             ['b'],      []],
            [['a'],             ['a'],      ['a']],
            [['a', 'b', 'c'],   ['a', 'b'], ['a', 'b']],
            [['a', 'b', 'c'],   ['b', 'c'], ['b', 'c']],
            [['a', 'b', 'c'],   ['c', 'd'], ['c']],
        ];
    }

    /**
     * @dataProvider intersectionDataProvider
     */
    public function testIntersection(array $initial, array $values, array $expected)
    {
        $a = $this->getInstance($initial);
        $b = $this->getInstance($values);

        $this->assertEquals($expected, $a->intersection($b)->toArray());
    }

    /**
     * @dataProvider intersectionDataProvider
     */
    public function testIntersectionOperator(array $initial, array $values, array $expected)
    {
        $a = $this->getInstance($initial);
        $b = $this->getInstance($values);

        $this->assertEquals($expected, ($a & $b)->toArray());
    }

    /**
     * @dataProvider intersectionDataProvider
     */
    public function testIntersectionOperatorAssign(array $initial, array $values, array $expected)
    {
        $a = $this->getInstance($initial);
        $b = $this->getInstance($values);

        $a &= $b;
        $this->assertEquals($expected, $a->toArray());
    }

    /**
     * @dataProvider intersectionDataProvider
     */
    public function testIntersectionWithSelf(array $initial, array $values, array $expected)
    {
        $a = $this->getInstance($initial);
        $this->assertEquals($initial, $a->intersection($a)->toArray());
    }

    /**
     * @dataProvider intersectionDataProvider
     */
    public function testIntersectionOperatorWithSelf(array $initial, array $values, array $expected)
    {
        $a = $this->getInstance($initial);
        $this->assertEquals($initial, ($a & $a)->toArray());
    }

    /**
     * @dataProvider intersectionDataProvider
     */
    public function testIntersectionOperatorAssignWithSelf(array $initial, array $values, array $expected)
    {
        $a = $this->getInstance($initial);

        $a &= $a;
        $this->assertEquals($initial, $a->toArray());
    }
}
