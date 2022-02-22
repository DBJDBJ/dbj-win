
# DBJ-WIP

## DBJ Windows development

This is standard C, WIN32, `clang-cl.exe` development. It is as simple as that.

### CAVEAT EMPTOR: THIS IS 100% WIP

### I don't know why it took me so long? 

But it did.

**Situation 2022 JAN**

If one wants to develop software on Windows, one has to follow the style, rules and customs of Windows software development.

That is not mandatory, that is simply feasible.

Why it took me approx 30 years to clearly write down this simple truth, honestly I do not know. Obviously, I was constantly bombarded by the OOP/C++ brigade propaganda. And very likely dissuaded from this simple truth by the evil army of Microsoft marketing department, too. So it is entirely not my fault :smiley:

> This library will contain a foundation code to be used by whoever want to develop "pure" Win32 code for delivering Windows desktop applications or (more likely) libraries.

Even more likely DLL's to be used "server side" or "desktop side". As circa 2022, there is no point in developing Windows desktop applications in C or C++. Plugins and extensions maybe; libs definitely. 

And there is definitely no point in using C/C++ to develop any kind of business logic. In those scenarios it is just "getting in the way". Hint: Think [Kubernetes](https://en.wikipedia.org/wiki/Kubernetes). Extremely complex and powerful. Language? Go.

Back here. How is that Win32 purity going to be achieved, currently, I do not know. I have a pretty educated plan:

1. One can not achieve universal and pure Win32.
   1. that means linking with [`/NODEFAULTLIB`](https://docs.microsoft.com/en-us/cpp/build/reference/nodefaultlib-ignore-libraries?view=msvc-170)
   2. Tht is simply not feasible for general use code. 
      1. Some important parts (think:SEH) are tied to CRT
2. One can achieve a lot by **not** using as mach as possible of CRT.
   1. Think: `<strsafe.h>` and `<PathCch.h>` instead of `string.h`

### Windows is mature

And that simple truth is not difficult to understand and consequently follow in ones plans. After all, Windows API aka WIN32 is unchanged for at least 30 years (2022  - 1992 = 30). 

Windows started 1985 (or before) but 1992 is the significant dark date when MFC was first released.  

As an experiment and to prove myself right or wrong, I have refactored (very slightly), just yesterday, an actual WIN32  C++ Framework conceived 1992 (or before). Those were the days of [Windows 95](https://en.wikipedia.org/wiki/Windows_95); when [Turbo C++](https://en.wikipedia.org/wiki/Turbo_C%2B%2B) and [Zortech](https://en.wikipedia.org/wiki/Digital_Mars) have been considered equal to MSVC if not better.

(As early as 1992 people where reacting to the dark engineering forces in Windows division trying to replace Win32 with C++ frameworks.)

The result is one very small, simple and very functional WIN32 (2022) C++ framework, namely ["Windows++"](https://github.com/DBJDBJ/windows_plus_plus) , invented by late [Paul DiLascia](https://en.wikipedia.org/wiki/Paul_DiLascia). 

Please try and understand how early that was conceived. 1992 was the year of initial MFC release too. Accidentally `Windows++` is infinitely simpler and fully functional. Minus macros, templates and minus [ATL](https://en.wikipedia.org/wiki/Active_Template_Library). 

ATL, came into fashion circa 2003. Ten years later; also as a key deliverable of the fraction inside Windows division opposed to to the dark engineering forces in the same Windows division. Or am I just talking rubbish here?

### But still you want to be using MSVCRT or UCRT? 

Well yes, that was your plan. Remove the [MSVCRT](https://en.wikipedia.org/wiki/Microsoft_Windows_library_files#CRT) dependency. Aka "the C standard library for the Visual C++ (MSVC)". Have small and fast pure win32 code. Yay!

Looking into UCRT documentation one might immediately ask: But why this "for C++" in the title? Is it not that[ C predates C++](https://en.wikipedia.org/wiki/C_(programming_language)#History) and for few decades too (C is 50 years old this year; 2022)? 

Or next questions. Is it not  surely crucial and beneficial to have fully conformant and 100% standard [CRT](https://en.wikipedia.org/wiki/C_standard_library) inside any commercial OS? Even today 2022. 

And after all is it not that WIN32 is written in C and is exhibiting [essentially an C API](https://en.wikipedia.org/wiki/Windows_API)?

Well, that is all true. But here i the catch: MSVC as a language and CL.exe as compiler are not made and used for any form of any "standard C". MSVC aka "Microsoft C", was and is very difficult to categorise. It simply does not, and was not, conforming to any ISO C standard. The closest one  can get to some definition of the kind of C is "some kind of ANSI C". And WIN32 API is written in that "kind of a" C. Ok, not to worry. "What could possibly go wrong" (tm).

So yes. Win32 was changed. A lot. And as of today, few key parts are using [CRT](https://en.wikipedia.org/wiki/Microsoft_Windows_library_files#CRT) lib. (again think [SEH](https://docs.microsoft.com/en-us/cpp/cpp/structured-exception-handling-c-cpp?view=msvc-170))

Yes, that Wikipedia link show the mess of [Microsoft CRT](https://en.wikipedia.org/wiki/Microsoft_Windows_library_files#CRT) circa today.

#### Recap and clarification. 

Same as ever, [from Windows 1.0 to date](https://en.wikipedia.org/wiki/List_of_Microsoft_Windows_components), Windows is essentialy written (and re-written) in "some kind of ANSI C". Although by 2022 portions are written (so they say) "in some kind of C++". More closely described to me, as "some kind of C++ with classes". 

### We need to talk about "standard" CRT

[Standard CRT](https://stackoverflow.com/questions/2766233/what-is-the-c-runtime-library) on the other hand, is well known and well hated by many. It is hated by millions and in every day use by tens of millions of programmers. But why is that so?

IT infrastructure is armature in foundations of this civilization. And C is the cement in those concrete foundations. And that includes C run-time aka CRT. And that is an ever expanding critical infrastructure under you, me and all of us: rising and expanding constantly last half a century. Without an end in sight to that expansion. 

Good so what? The key problem is as simple as this: **LEGACY**. 

There is no escape from the legacy decisions everybody agrees  today are [simply wrong](https://stackoverflow.com/questions/4418708/whats-the-rationale-for-null-terminated-strings). Legacy or not, C and CRT will stay as long as this civilization stays. However [bad CRT seems to be](https://news.ycombinator.com/item?id=28012563).

### WIN32 code is not based on CRT

It is rather the opposite: MSVC CRT is based on WIN32. The lowest od the low level code that is not assembler in Windows is WIN32 code. Not CRT. 

Thus, closest to the Windows elusive core is WIN32 layer of code.  Yes, that applies even to the portions of the NT code too.

Primordial Windows developers have been C coders, I think.  To architect and build an OS is very (very) difficult. Thus Windows foundations are unchanged for 30+ years. True: Windows versions are bringing changes and additions but the Windows core idea and implementation is still the same as it ever was. [Even after Windows NT](https://en.wikipedia.org/wiki/Microsoft_Windows).

To cut the long story short, they (the Windows proto team), even all those decades ago, knew CRT is bad. They decided to develop an OS using C (from that period) but not using CRT. It was (I think) as simple as that.

Also CRT is just a C code. It needs some underlying OS services to implement the crucial parts, like memory allocation, file functions and such.  And naturally Win32 delivered those low level OS services. Thuse we say Windows CRT is **hosted**. It is not **free standing**, it is hosted by Windows.

## Conclusion is not one

And that is the same core idea of this lib too: do not use CRT while developing for Windows.

Develop Windows code using Windows API only, aka Win32. It simply offers everything required. 

This repo is me trying to develop a core for Windows upstream code that will **try** and not use MSVCRT or UCRT. 

I say: try. Because that is one very difficult thing to do. 

### /NODEAFULTLIB is not feasible

Along the long an winding road, going back in time, there are several well known "let us build a tiny CRT" kind-of-a projects. Using them with MSVC year 2022 it is just not feasible. There are few key reasons. Like not having SEH; which we must have. And other show stoppers: not having a quality float math, just to mention that one.

Hint: [MS STL](https://github.com/microsoft/STL#:~:text=README.md-,Microsoft's%20C%2B%2B%20Standard%20Library,and%20the%20Visual%20Studio%20IDE.) is using [UCRT and MSVCRT](https://stackoverflow.com/questions/61518687/where-does-c-c-runtime-come-from), I assume that was an early decision so that it could be delivered on time. They probably must have realised it is difficult not to use CRT at all. HINT: MS STL depends on SEH in the `/kernel` aka `_NO_EXCEPTION` mode.

## So what is the purpose of all of this code in this repo?

This is for Win32 only projects.

It is very simple: develop fast and simple code. Always take the shortcut to the OS. And Win32 is the last layer before the OS assembler code.

### Roadmap

I will at least try to use this repo as the core lib for all of my Windows code.

I have some  (a lot?) of core code that until recently was developed without a clear sense of direction.

Now, past that stage I will have to take it bit by bit and incorporate it into `dbjwin`. That is `dbj_capi` you can see there, one level up from this folder.

No, I have not forgotten C++. I will be using MS STL but always with the lovely `/kernel` switch. Liberating efficiently many "modern C++" aficionados. And my C++ style is apparently close to the "[Orthodox C++](https://gist.github.com/bkaradzic/2e39896bc7d8c34e042b)".

---
[dbj at dbj dot org](https://dbj.org/), 2022-01-07