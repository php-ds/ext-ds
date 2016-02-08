<?php
namespace Ds\Tests\Deque;

trait slice
{
    /**
     * Deque has a few edge cases that don't exist for other sequences. These
     * occur when the head of the deque wraps around, ie. h > t.
     */
    public function testSliceExtended()
    {
        $instance = $this->getInstance();

        $instance->unshift('c'); // [_, _, _, _, _, _, _, c] tail = 0, head = 7
        $instance->unshift('b'); // [_, _, _, _, _, _, b, c] tail = 0, head = 6
        $instance->unshift('a'); // [_, _, _, _, _, a, b, c] tail = 0, head = 5

        $this->assertEquals(['a'], $instance->slice(0, 1)->toArray());
        $this->assertEquals(['b'], $instance->slice(1, 1)->toArray());
        $this->assertEquals(['c'], $instance->slice(2, 1)->toArray());

        $this->assertEquals(['a', 'b'], $instance->slice(0, 2)->toArray());
        $this->assertEquals(['b', 'c'], $instance->slice(1, 2)->toArray());
        $this->assertEquals(['c'     ], $instance->slice(2, 2)->toArray());

        $this->assertEquals(['a', 'b', 'c'], $instance->slice(0, 3)->toArray());
        $this->assertEquals(['b', 'c'     ], $instance->slice(1, 3)->toArray());
        $this->assertEquals(['c'          ], $instance->slice(2, 3)->toArray());

        /* If only some values have wrapped around, slice would have to copy
           from both the wrapped and not-wrapped values */

        $instance = $this->getInstance();

        $instance->push('b');    // [b, _, _, _, _, _, _, _] tail = 1, head = 0
        $instance->push('c');    // [b, c, _, _, _, _, _, _] tail = 2, head = 1
        $instance->unshift('a'); // [b, c, _, _, _, _, _, a] tail = 2, head = 7

        $this->assertEquals(['a'], $instance->slice(0, 1)->toArray());
        $this->assertEquals(['b'], $instance->slice(1, 1)->toArray());
        $this->assertEquals(['c'], $instance->slice(2, 1)->toArray());

        $this->assertEquals(['a', 'b'], $instance->slice(0, 2)->toArray());
        $this->assertEquals(['b', 'c'], $instance->slice(1, 2)->toArray());
        $this->assertEquals(['c'     ], $instance->slice(2, 2)->toArray());

        $this->assertEquals(['a', 'b', 'c'], $instance->slice(0, 3)->toArray());
        $this->assertEquals(['b', 'c'     ], $instance->slice(1, 3)->toArray());
        $this->assertEquals(['c'          ], $instance->slice(2, 3)->toArray());
    }
}
