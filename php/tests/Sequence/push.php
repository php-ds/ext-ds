<?php
namespace Ds\Tests\Sequence;

trait push
{
    public function pushDataProvider()
    {
        return $this->basicDataProvider();
    }

    /**
     * @dataProvider pushDataProvider
     */
    public function testPushVariadic(array $values, array $expected)
    {
        $instance = $this->getInstance();
        $instance->push(...$values);

        $this->assertToArray($expected, $instance);
        $this->assertEquals(count($expected), count($instance));
    }

    /**
     * @dataProvider pushDataProvider
     */
    public function testPush(array $values, array $expected)
    {
        $instance = $this->getInstance();

        foreach ($values as $value) {
            $instance->push($value);
        }

        $this->assertToArray($expected, $instance);
        $this->assertEquals(count($expected), count($instance));
    }

    /**
     * @dataProvider pushDataProvider
     */
    public function testArrayAccessPush(array $values, array $expected)
    {
        $instance = $this->getInstance();

        foreach ($values as $value) {
            $instance[] = $value;
        }

        $this->assertToArray($expected, $instance);
        $this->assertEquals(count($expected), count($instance));
    }

    public function testPushCircularReference()
    {
        $instance = $this->getInstance(['a', 'b', 'c']);
        $instance->push($instance);
        $this->assertToArray(['a', 'b', 'c', $instance], $instance);
    }

    public function testPushIndirectCircularReference()
    {
        $a = $this->getInstance();
        $b = $this->getInstance();

        $a->push($b);
        $b->push($a);

        $this->assertToArray([$b], $a);
        $this->assertToArray([$a], $b);
    }
}
