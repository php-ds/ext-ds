# Data structures for PHP 7

[![Build Status](https://travis-ci.org/php-ds/ds.svg?branch=master)](https://travis-ci.org/php-ds/ds)
[![Build status](https://ci.appveyor.com/api/projects/status/in5p00vw6rk5f27q?svg=true)](https://ci.appveyor.com/project/krakjoe/ds)

[**Blog post**](https://medium.com/@rtheunissen/efficient-data-structures-for-php-7-9dda7af674cd) that covers the behaviour and performance benefits of each data structure.


## Installation

#### Ubuntu

```bash
$ ./install/ubuntu.sh
```

#### OS X

```bash
$ ./install/osx.sh
```

## Usage

##### Enabling the extension

The best way to enable the extension is to update your *ini* file by adding `extension=ds.so`.
You can use `php -i | grep php.ini` to see which *ini* file is being used.

---

You can also enable the extension from the command line:

```bash
$ php -d extension=ds.so
```

## Documentation

See [/php/include](/php/include) for now. Proper documentation is in the works.

## Testing

There is a suite of PHPUnit tests that can be installed using [**Composer**](https://getcomposer.org/doc/00-intro.md#installation-linux-unix-osx).

#### Running the tests
``` bash
$ composer install
$ php -d extension=ds.so ./vendor/bin/phpunit
```

## Contributing

Please see [CONTRIBUTING](CONTRIBUTING.md) for more information.

## Credits

- [Rudi Theunissen](https://github.com/rtheunissen)
- [Joe Watkins](https://github.com/krakjoe)

## License

The MIT License (MIT). Please see [LICENSE](LICENSE.md) for more information.
