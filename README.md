# wasm-experiments

Some experiments with [Emscripten](https://emscripten.org/index.html).

No real reason for this to be on GitHub other than that I just don't want to worry about
deleting it from my PC. This README is a bit of a stream of consciousness rather than
a formal piece of documentation.

As always for me, build with `./build.sh`.

## Brief overview of Emscripten

Emscripten is a toolchain that allows you to compile C code to WebAssembly (WASM).
I am not really sure how many people are building WASM applications. It seems kinda
niche but maybe that is because I live in a systems bubble.

Emscripten can target Node.js or the browser. I feel like the Node.js target is super
niche, like why would you not just write a native server application? So the examples
here are all intended for the browser.

Anyways, The Emscripten compiler `emcc` takes C code and generates:
1. A `.wasm` file, which contains the actual binary code,
2. A `.js` file, which provides some glue between the C runtime and JS land,
3. (Sometimes) a `.html` file, which is served to the browser.

What happens is the HTML file loads the JS file, and then some magic happens in there
that I don't fully understand yet and the C `main()` is invoked. One such step of the
magic process is a `fetch` against the WASM file, which is a CORS request. As such,
you need to serve these applications over HTTP and not just on the filesystem.

### hello-world
A hello world C program compiled with `emcc` just to see what would happen. The
`-o` flag specifies the output file, which can be HTML or JS (or object file, but I
have no need for partial builds here). If the output file is HTML, Emscripten will
generate a "shell" that basically turns a C application into a webpage.

Optionally you can specify a `--shell-file` which, um, does something? There's
preprocessor directives in there so I guess something is happening with those.
To my eyes `out.html` and `out2.html` look identical minus the header at the top
of one of them.

### headless-test
So the first order of business was to get rid of the horrifically ugly default
Emscripten shell. If you specify a JS file for `-o`, no HTML will be generated, and
the onus is on you to do the load. And everything else on the frontend actually.

Fortunately, putting the magic Emscripten glue JS file in a `<script src=>` handles
the setup of the runtime and invocation of `main()`. But I noticed a couple things:
1. stdout is redirected to the JS console, but somehow it was going to the DOM before,
2. An application that takes no input and terminates is pretty useless.

Emscripten provides a way for you to register event handlers for HTML5 events. So your
standard onclicks, mousemoves, keydown, etc can trigger WASM code. That's (2) handled,
but what Emscripten doesn't provide is a way to manipulate the DOM. In fact, it's
not in the WASM spec.

But what you can do is call JS. So you make a small JS function that does the DOM functions
you care about and just call it from WASM. Or if you're feeling villainous, you can execute
arbitrary JS code at runtime, which could include DOM manipulation. My intuition is that
this is less performant than defining the functions ahead of time and it feels less clean
anyways.

### sdl-test
So now we come to my real use case, which is that I am getting rinsed in every
game jam I do because in 2026 people care about silly things like "computer security"
and "malware" so they won't download random executables from itch. Most other game devs
solve this by using an engine that has a web export (which uses WASM). Traditionally I
would have opted to write the entire game in JS instead, which I prefer over using a
game engine, but I am not a very good JS developer. If only I could make an application
in C and deploy it to the web!

Well, now I can. Emscripten supports SDL out of the box, as well as a number of other
libraries. I'm curious about WebGL support but more likely I will jump straight into
WebGPU when I want to make something more sophisticated. I'm actually pretty excited
about Emscripten so far and I think with some better tooling on my end this could be
the way I choose to do game jams from now on.

As of writing, the [Very Serious Juniper Dev Game Jam](https://itch.io/jam/theveryseriousjuniperdevgamejam)
is happening and I am learning how to use Emscripten on day 2 instead of actually making
progress on something. But hey, I've always liked to use game jams as an excuse to try
new toolchains. Hopefully I don't get rinsed this time!
