<?php
namespace Ds\Tests\Set;

trait slice
{
    public function sliceDataProvider()
    {
        $a = ['a', 'b', 'c'];
        $n = count($a);

        $data = [];

        for ($i = -$n; $i <= $n; $i++) {
            for ($j = -$n; $j <= $n; $j++) {
                $data[] = [[], $i, $j];
                $data[] = [$a, $i, $j];
            }
        }

        return $data;
    }

    /**
     * @dataProvider sliceDataProvider
     */
    public function testSlice(array $values, int $index, int $length)
    {
        $instance = $this->getInstance($values);

        $sliced = $instance->slice($index, $length);
        $expected = array_slice($values, $index, $length);

        $this->assertToArray($values, $instance);
        $this->assertToArray($expected, $sliced);
    }

    /**
     * @dataProvider sliceDataProvider
     */
    public function testSliceWithoutLength(array $values, int $index, int $length)
    {
        $instance = $this->getInstance($values);

        $sliced = $instance->slice($index);
        $expected = array_slice($values, $index, null);

        $this->assertToArray($values, $instance);
        $this->assertToArray($expected, $sliced);
    }

    public function testSliceAfterRemoveOutsideOfSlice()
    {
        $instance = $this->getInstance(['a', 'b', 'c', 'd', 'e']);
        $instance->remove('d');

        $this->assertToArray(['a', 'b', 'c'], $instance->slice(0, 3));
    }

    public function testSliceAfterRemoveAtStartOfSlice()
    {
        $instance = $this->getInstance(['a', 'b', 'c', 'd', 'e']);
        $instance->remove('b');

        $this->assertToArray(['c', 'd', 'e'], $instance->slice(1));
    }

    public function testSliceAfterRemoveWithinSlice()
    {
        $instance = $this->getInstance(['a', 'b', 'c', 'd', 'e']);
        $instance->remove('c');

        $this->assertToArray(['b', 'd', 'e'], $instance->slice(1));
    }

    public function testLargeSliceHalf()
    {
        $n = self::MANY;
        $x = 0;
        $y = $n / 2;

        $instance = $this->getInstance(range(0, $n));

        $this->assertToArray(range($x, $y - 1), $instance->slice($x, $y));
        $this->assertToArray(range($y, $n),     $instance->slice($y));
    }

    public function testLargeSliceOffset()
    {
        $n = self::MANY;
        $x = $n / 4;
        $y = $n / 4 + $n / 2;

        $instance = $this->getInstance(range(0, $n));

        $this->assertToArray(range($x, $y - 1), $instance->slice($x, $y - $x));
    }
}
