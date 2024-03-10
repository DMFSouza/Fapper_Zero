# FastLED NeoPixel Library Documentation

The source code for the FastLED NeoPixel library is fully documented with comments formatted for the [Doxygen](https://en.wikipedia.org/wiki/Doxygen) documentation generator. This folder contains the files needed to generate the Doxygen documentation.

[The generated documentation is hosted online on GitHub Pages](https://dmadison.github.io/FastLED_NeoPixel/docs). The documentation generation has been automated using [GitHub Actions](https://docs.github.com/en/actions) and should be up to date with [the latest release](https://github.com/dmadison/FastLED_NeoPixel/releases/latest).

You can generate this documentation locally by [installing Doxygen](https://www.doxygen.nl/manual/install.html) and running the program with the included `Doxyfile`:

```
doxygen Doxyfile
```

The project is configured to use the [Doxygen Awesome theme](https://github.com/jothepro/doxygen-awesome-css) for a more modern look. This is optional - the documentation will build successfully without these extra CSS files. To add them, clone the repository and place it into the 'docs' folder alongside the Doxyfile.
