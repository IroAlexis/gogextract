# GOG Extract

The [project] is developped by @Yepoleb.
Script for unpacking GOG Linux installers.

Explanation of how it works is in his [blog post].

I'm rewriting it in the C language with the objective of not having memory leaks

## Building

Just run `make`

## Usage

Not available for moment but you have always the initial project : `legacy/gogextract.py <input file> <output dir>`

Output files will be named `unpacker.sh`, `mojosetup.tar.gz` and `data.zip`.

## License

Available in [LICENSE](LICENSE) file

[blog post]: https://yepoleb.github.io/blog/2016/10/09/how-the-gog-linux-installers-work/
[project]: https://github.com/Yepoleb/gogextract
