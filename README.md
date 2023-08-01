# Vespula

A prototype software stack for controlling a RoboCup SSL team's robots (or
"an AI", in RoboCup SSL parlance). Showcases node-based communication in pure
C++, without the overhead of ROS. Designed to be compatible with Mac and Linux
(I can't personally test on Windows).

## Running

0. Install [meson](https://mesonbuild.com/Getting-meson.html).
1. Install subprojects: `chmod +x install.sh && ./install.sh`
2. Build: `meson setup build`
3. Compile: `cd build && meson compile`
4. Run: `./vespula`

After the first build, meson will automatically update the build configuration when you modify the meson.build file. So you can start from "Compile".

Note: currently there is no UI because I didn't want to learn QT/some other GUI
platform for C++.

**Expected Behavior:** TODO

## Design Choices

I use [concurrentqueue](https://github.com/cameron314/concurrentqueue) for
inter-node communication because it is thread-safe, header-only, and I liked
the syntax. This allows nodes to live on threads (or the same one) and still
communicate. There are other ways to achieve this, like Boost's
signals-and-slots or rolling a custom mutex-locked data object, but I felt more
comfortable with this syntax coming from ROS. Moreover, this package was easy to install with Meson.

### Node Organization

The three central sources of truth in a RoboCup SSL game are Vision, Referee, and
Robot Status, and should be separate nodes and live on different threads. All
three take in some data from the network and output a read-only estimate of
what they get. (Estimate because Vision needs to be filtered, and we could also
filter Robot Status in the same way.) **For this prototype, I only implement
the Referee node.**

The autonomy side can be implemented as several nodes (e.g. Planning, Control,
Agents). **For this prototype, I implement it as a single node.**

Finally, the output of the whole stack, radio commands to the robots, should
also be a separate node. I term the output side Radio and the input side Robot
Status to minimize confusion, though obviously both communicate with the same
radio boards on the robots. Currently since we use UDP this is fine. If ever we
use a TCP connection, this design would need to be altered. **For this
prototype, Radio simply logs to console.**

### Style

I chose to follow the [Google style
guide](https://google.github.io/styleguide/cppguide.html) as much as possible.
To ensure compliance I use [cpplint](https://github.com/cpplint/cpplint).

For me, one of the more surprising parts of the style guide was about
inheritance. Google's style guide recommends almost never using implementation
inheritance (aka, non-interface inheritance), instead preferring composition +
interface inheritance to achieve the same thing in a more flexible way. [Here
are](https://youtu.be/hxGOiiR9ZKg) [two videos](https://youtu.be/wfMtDGfHWpA)
that convinced me this was a reasonable thing to do, in spite of years of OOP
education from school.

### Build system: Meson

I chose Meson because the leading C++ build tool alternatives, CMake and Bazel,
both have flaws that I considered more dealbreaking than Meson did.

While CMake is the standard for C++ projects because it has a rich history,
this history makes it difficult for students to pick up and learn quickly.
Often searching for "how to do X in CMake" yields many possible solutions, some
of which "modern" CMake has phased out. A good build system for students should
be fast to learn and have one definitive source-of-truth to refer to.

Bazel is Google's internal build tool, and with that comes a lot of assumptions
that suit big tech companies but not our specific needs. For instance, Bazel
handles installing dependencies before building with `bazel run`,
cross-platform, which is really nice, except that means to build with Bazel
requires an internet connection. At RoboCup this may not be feasible.

Thus, Meson. It is CMake-like in that you create a `build/` folder, then run
the tool, then run some executable, but has modern
[documentation](https://mesonbuild.com/) and an
easy-to-learn syntax (which is almost Python but not quite).

### External libaries

As much as possible, I tried to use header-only external libraries. This is to
limit build times and increase cross-platform compatibility.

Naturally, some libraries are too handy to ignore:
 * 

### The Name

Vespula squamosa is the scientific name for the [southern
yellowjacket](https://en.wikipedia.org/wiki/Vespula_squamosa). Squamosa sounds
a little ridiculous, but "Vespula" has a cool vibe. This solves one of
RoboJackets' largest issues, which is that our software stack was named
"Soccer", and brings us closer to TIGERS, who have an AI stack named "Sumatra".
(I'm half-joking, but in all seriousness "Soccer" is a truly confusing name).

Vespula Software used to exist but was [bought
out](https://vespulav2.azurewebsites.net/) by a different company. In any
event, our application is different enough from "electrical certification
software" that trademark shouldn't be an issue.
