<?php
namespace Ds\Tests\Map;

use Ds\Tests\HashableObject;

trait remove
{
    public function removeDataProvider()
    {
        $o = new \stdClass();

        // initial pairs, key, return, result
        return [

            // Test basic removal
            [[['a', 1], ['b', 2]], 'a', 1, ['b' => 2]],

            // Test removing an object works
            [[[$o, 'x'], ['a', 1]], $o, 'x', ['a' => 1]],

            // Test that removing a null key works
            [[[null, '#'], ['a', 1]], null, '#', ['a' => 1]],
        ];
    }

    public function removeHashableDataProvider()
    {
        // Two objects with the same hash code and equals.
        $h1 = new HashableObject(1);
        $h2 = new HashableObject(1);

        // put pairs, expected pairs
        return [
            [[[$h1, $h2]], $h1, $h2, []],
        ];
    }

    /**
     * @dataProvider removeHashableDataProvider
     */
    public function testRemoveHashable(array $initial, $key, $expected, array $result)
    {
        $this->testRemove($initial, $key, $expected, $result);
    }

    public function testRemoveAllFromFront()
    {
        $instance = $this->getInstance();

        for ($i = 0; $i < self::MANY; $i++) {
            $instance->put($i, $i);
        }

        for ($i = 0; $i < self::MANY; $i++) {
            $instance->remove($i);
        }

        $this->assertCount(0, $instance);
        $this->assertToArray([], $instance);
        $this->assertTrue($instance->isEmpty());
    }

    public function testRemoveHalfFromMidway()
    {
        $instance = $this->getInstance();

        for ($i = 0; $i < self::MANY; $i++) {
            $instance->put($i, $i);
        }

        for ($i = self::MANY / 2; $i < self::MANY; $i++) {
            $instance->remove($i);
        }

        $this->assertCount(self::MANY / 2, $instance);
    }

    public function testRandomRemove()
    {
        $instance  = $this->getInstance();
        $reference = [];

        for ($i = 0; $i < 10; $i++) {

            for ($i = 0; $i < self::MANY; $i++) {
                $k = rand(0, self::MANY * 2);
                $v = rand();

                $reference[$k] = $v;
                $instance[$k]  = $v;
            }

            for ($i = 0; $i < self::MANY; $i++) {
                $k = rand(0, self::MANY * 2);


                unset($reference[$k]);
                unset($instance[$k]);
            }
        }

        $this->assertToArray($reference, $instance);
    }

    /**
     * @dataProvider removeDataProvider
     */
    public function testRemove(array $initial, $key, $expected, array $result)
    {
        $instance = $this->getInstance();

        foreach ($initial as $pair) {
            $instance->put($pair[0], $pair[1]);
        }

        $this->assertEquals($expected, $instance->remove($key));
        $this->assertToArray($result, $instance);
    }

    public function testRemoveDefault()
    {
        $instance = $this->getInstance();
        $this->assertEquals('a', $instance->remove('?', 'a'));
    }

    public function testRemoveKeyNotFound()
    {
        $instance = $this->getInstance();
        $this->expectKeyNotFoundException();
        $instance->remove('?');
    }
}
