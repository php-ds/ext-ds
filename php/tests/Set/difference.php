<?php
namespace Ds\Tests\Set;

trait difference
{
    public function differenceDataProvider()
    {
        // Values in A but not in B.
        // A, B, expected result
        return [
            [[],          [],           []],
            [['a'],       [],           ['a']],
            [['a'],       ['b'],        ['a']],
            [['a', 'b'],  ['a'],        ['b']],
            [['a', 'b'],  ['b'],        ['a']],
            [['a', 'b'],  ['a', 'b'],   []],
        ];
    }

    /**
     * @dataProvider differenceDataProvider
     */
    public function testDifference(array $a, array $b, array $expected)
    {
        $a = $this->getInstance($a);
        $b = $this->getInstance($b);

        $this->assertEquals($expected, $a->difference($b)->toArray());
    }

    /**
     * @dataProvider differenceDataProvider
     */
    public function testDifferenceOperator(array $a, array $b, array $expected)
    {
        $a = $this->getInstance($a);
        $b = $this->getInstance($b);

        $this->assertEquals($expected, ($a - $b)->toArray());
    }

    /**
     * @dataProvider differenceDataProvider
     */
    public function testDifferenceOperatorAssign(array $a, array $b, array $expected)
    {
        $a = $this->getInstance($a);
        $b = $this->getInstance($b);

        $a -= $b;
        $this->assertEquals($expected, $a->toArray());
    }

    /**
     * @dataProvider differenceDataProvider
     */
    public function testDifferenceWithSelf(array $a, array $b, array $expected)
    {
        $a = $this->getInstance($a);
        $this->assertEquals([], $a->difference($a)->toArray());
    }

    /**
     * @dataProvider differenceDataProvider
     */
    public function testDifferenceOperatorWithSelf(array $a, array $b, array $expected)
    {
        $a = $this->getInstance($a);
        $this->assertEquals([], ($a - $a)->toArray());
    }

    /**
     * @dataProvider differenceDataProvider
     */
    public function testDifferenceOperatorAssignWithSelf(array $a, array $b, array $expected)
    {
        $a = $this->getInstance($a);

        $a -= $a;
        $this->assertEquals([], $a->toArray());
    }
}
