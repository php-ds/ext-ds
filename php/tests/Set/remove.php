<?php
namespace Ds\Tests\Set;

trait remove
{
    public function removeDataProvider()
    {
        list($unique, $duplicates) = $this->getUniqueAndDuplicateData();

        // initial, input, expected.
        return [
            [[],    [],     []],
            [[],    ['a'],  []],
            [['a'], ['a'],  []],
            [['a'], ['b'],  ['a']],

            // Test removing a few values.
            [[],         ['a', 'b'], []],
            [['a', 'b'], ['a', 'b'], []],
            [['a', 'b'], ['a', 'a'], ['b']],

            // Test that numeric strings are not treated as int.
            [[ 1 ], ['1'], [ 1 ]],
            [['1'], [ 1 ], ['1']],

            // Test removing many values from empty set does nothing.
            [[], $unique, []],

            // Test removing identical data removes everything
            [$unique, $unique, []],

            // Test removing duplicate values only remove duplicate values.
            [$unique, $duplicates, []],
        ];
    }

    /**
     * @dataProvider removeDataProvider
     */
    public function testRemove(
        array $initial,
        array $values,
        array $expected
    ) {
        $instance = $this->getInstance($initial);

        foreach ($values as $value) {
            $instance->remove($value);
        }

        $this->assertEquals(count($expected), count($instance));

        foreach ($values as $value) {
            $this->assertFalse($instance->contains($value));
        }

        foreach ($expected as $value) {
            $this->assertTrue($instance->contains($value));
        }
    }

    /**
     * @dataProvider removeDataProvider
     */
    public function testRemoveVariadic(
        array $initial,
        array $values,
        array $expected
    ) {
        $instance = $this->getInstance($initial);
        $instance->remove(...$values);

        $this->assertEquals(count($expected), count($instance));

        foreach ($values as $value) {
            $this->assertFalse($instance->contains($value));
        }

        foreach ($expected as $value) {
            $this->assertTrue($instance->contains($value));
        }
    }
}
