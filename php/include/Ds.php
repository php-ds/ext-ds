<?php

class Ds
{
    /**
     * @return Ds\Vector
     */
    public static function vector($values = null): Ds\Vector {}

    /**
     * @return Ds\Deque
     */
    public static function deque($values = null): Ds\Deque {}

    /**
     * @return Ds\Set
     */
    public static function set($values = null): Ds\Set {}

    /**
     * @return Ds\Map
     */
    public static function map($values = null): Ds\Map {}

    /**
     * @return Ds\Stack
     */
    public static function stack($values = null): Ds\Stack {}

    /**
     * @return Ds\Queue
     */
    public static function queue($values = null): Ds\Queue {}

    /**
     * @return Ds\PriorityQueue
     */
    public static function priority_queue(): Ds\PriorityQueue {}
}
