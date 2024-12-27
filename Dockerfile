FROM php:8.4-alpine

RUN apk add --no-cache $PHPIZE_DEPS valgrind

COPY --from=composer:latest /usr/bin/composer /usr/local/bin/composer
