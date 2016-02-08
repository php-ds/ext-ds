<?php
namespace Ds\Tests\Set;

trait join
{
    public function joinDataProvider()
    {
        // values, glue
        $data = [];

        $glues   = ['', '~', 1, false];
        $lengths = [0, 1, 2, 3, self::SOME, self::MANY];

        foreach ($lengths as $len) {
            foreach ($glues as $glue) {
                $data[] = [range(0, $len - 1), $glue];
            }
        }

        return $data;
    }

    /**
     * @dataProvider joinDataProvider
     */
    public function testJoin(array $values, $glue)
    {
        $instance = $this->getInstance($values);
        $expected = join($glue, $values);
        $this->assertEquals($expected, $instance->join($glue));
    }

    /**
     * @dataProvider joinDataProvider
     */
    public function testJoinWithoutGlue(array $values, $glue)
    {
        $instance = $this->getInstance($values);
        $expected = join($values);
        $this->assertEquals($expected, $instance->join());
    }
}
