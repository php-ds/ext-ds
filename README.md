# Data Structures for PHP 7

[![Build Status](https://travis-ci.org/php-ds/extension.svg?branch=master)](https://travis-ci.org/php-ds/extension)
[![Build status](https://ci.appveyor.com/api/projects/status/dbcssp6flml2gher?svg=true)](https://ci.appveyor.com/project/rtheunissen/extension)
[![Code Coverage](https://scrutinizer-ci.com/g/php-ds/polyfill/badges/coverage.png?b=master)](https://scrutinizer-ci.com/g/php-ds/polyfill/?branch=master)
[![PECL](https://img.shields.io/badge/PECL-1.1.4-blue.svg)](https://pecl.php.net/package/ds)

An extension providing specialized data structures as efficient alternatives to the PHP array.

## Documentation

Documentation is available on [php.net](http://docs.php.net/manual/en/book.ds.php). You should also include the [polyfill](https://github.com/php-ds/polyfill) in your project for compatibility and IDE integration. The polyfill will not be loaded if the extension is enabled

## Installation

The easiest way to install the extension is to use PECL:

```
pecl install ds
```

If you're on Windows, you can download a compiled .dll on [PECL](https://pecl.php.net/package/ds) or under [releases](https://github.com/php-ds/extension/releases).

## Enable

You'll need to add `extension=ds.so` to your primary *php.ini* file.


If you encounter a "symbol not found" error, you'll need to make sure that the
extension is loaded *after* its dependencies (spl and json). Create a separate ini file called *30-ds.ini* in the same directory as the other extension-specific ini files (usually in "conf.d" or "mods_available"). These files will be parsed alphabetically, so the "30" prefix is there to make sure it comes after SPL and JSON.

```bash
# To see where .ini files are located
php -i | grep "\.ini"
```

---

You can also enable the extension temporarily using the command line:

```bash
php -d extension=ds.so
```

## Testing

There is a suite of PHPUnit tests that can be installed using [**Composer**](https://getcomposer.org/doc/00-intro.md#installation-linux-unix-osx).

``` bash
composer install   # Install the test suite
composer test      # Run the tests
composer memtest   # Run the tests checking for memory leaks
```

## Compatibility

It's highly recommended that you include the [polyfill](https://github.com/php-ds/polyfill) as a dependency in your project. This allows your codebase to still function in an environment where the extension is not installed. 

## Contributing

Please see [CONTRIBUTING](CONTRIBUTING.md) for more information.

## Credits

- [Rudi Theunissen](https://github.com/rtheunissen)
- [Joe Watkins](https://github.com/krakjoe)

## License

The MIT License (MIT). Please see [LICENSE](LICENSE) for more information.
