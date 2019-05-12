# Data Structures for PHP 7

[![Build Status](https://travis-ci.org/php-ds/ext-ds.svg?branch=master)](https://travis-ci.org/php-ds/ext-ds)
[![Build status](https://ci.appveyor.com/api/projects/status/9w0xitp3q04hdu1d?svg=true)](https://ci.appveyor.com/project/rtheunissen/ext-ds)
[![PECL](https://img.shields.io/badge/PECL-1.2.9-blue.svg)](https://pecl.php.net/package/ds)

An extension providing specialized data structures as efficient alternatives to the PHP array.
You can read about it in more detail [in this blog post](https://medium.com/p/9dda7af674cd) which highlights the API, performance and other benefits of using the extension.

## Documentation

Documentation is available on [php.net](http://docs.php.net/manual/en/book.ds.php). You should also include the [polyfill](https://github.com/php-ds/polyfill) in your project for IDE integration.

## Installation

The easiest way to install the extension is to use PECL:

```
pecl install ds
```

If you're on Windows, you can download a compiled .dll on [PECL](https://pecl.php.net/package/ds) or under [releases](https://github.com/php-ds/extension/releases).

## Enabling the extension

You'll need to add `extension=ds.so` to your primary *php.ini* file.

If you encounter an "undefined symbol" error, see [#2](https://github.com/php-ds/extension/issues/2#issuecomment-181855047).

```bash
# To see where .ini files are located
php -i | grep "\.ini"
```

---

You can also enable the extension temporarily using the command line:

```bash
php -d extension=ds.so
```

**Note**: Windows would use php_ds.dll instead.

## Testing

There is a suite of PHPUnit tests that can be installed using [**Composer**](https://getcomposer.org/doc/00-intro.md#installation-linux-unix-osx).

``` bash
composer install   # Install the test suite
composer test      # Run the tests
composer memtest   # Run the tests checking for memory leaks
```

## Compatibility

You may include the [polyfill](https://github.com/php-ds/polyfill) as a dependency in your project. This allows your codebase to still function in an environment where the extension is not installed.

## Contributing

Please see [CONTRIBUTING](CONTRIBUTING.md) for more information.

## Credits

- [Rudi Theunissen](https://github.com/rtheunissen)
- [Joe Watkins](https://github.com/krakjoe)

## License

The MIT License (MIT). Please see [LICENSE](LICENSE) for more information.
