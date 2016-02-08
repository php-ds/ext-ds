<?php
namespace Ds\Tests\Sequence;

trait rotate
{
    public function rotateDataProvider()
    {
        // values, rotation, expected
        return [
            [['a', 'b', 'c'],  0, ['a', 'b', 'c']],
            [['a', 'b', 'c'],  1, ['b', 'c', 'a']],
            [['a', 'b', 'c'],  2, ['c', 'a', 'b']],
            [['a', 'b', 'c'],  3, ['a', 'b', 'c']],
            [['a', 'b', 'c'],  4, ['b', 'c', 'a']],

            [['a', 'b', 'c'], -1, ['c', 'a', 'b']],
            [['a', 'b', 'c'], -2, ['b', 'c', 'a']],
            [['a', 'b', 'c'], -3, ['a', 'b', 'c']],
            [['a', 'b', 'c'], -4, ['c', 'a', 'b']],
        ];
    }

    /**
     * @dataProvider rotateDataProvider
     */
    public function testRotate(array $values, int $rotation, array $expected)
    {
        $instance = $this->getInstance($values);
        $instance->rotate($rotation);
        $this->assertToArray($expected, $instance);
    }

    public function testRandomRotate()
    {
        // This isn't the most efficient algorithm but is more obvious.
        $array_rotate = function(array &$a, int $n) {
            if ($n < 0) {
                $n = abs($n); // Take the absolute value because we only need to
                              // know how many rotations are needed. Direction
                              // is handled in the loop using unshift and pop.

                for ($n %= count($a); $n > 0; $n--) {
                    array_unshift($a, array_pop($a));
                }

            } else {
                for ($n %= count($a); $n > 0; $n--) {
                    array_push($a, array_shift($a));
                }
            }
        };

        $values = range(1, self::MANY);
        $instance = $this->getInstance($values);

        for ($i = 0; $i < 100; $i++) {
            $rotation = rand(-(self::MANY * 2), self::MANY * 2);
            $instance->rotate($rotation);
            $array_rotate($values, $rotation);
        }

        $this->assertToArray($values, $instance);
    }
}
