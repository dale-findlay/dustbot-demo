# Dustbot Demo Requirements

# Compiling the demo
This is a pretty old piece of code, so im super sorry if it doesnt compile super easily, this is present day me adding these instructions for how to compile :)

# Installing Mono
You'll need mono installed on your machine to build the demo from Visual studio.

Here's a link to the installer:
https://download.mono-project.com/archive/5.14.0/windows-installer/mono-5.14.0.177-x64-0.msi

It has to get installed to this dir: C:\Program Files\Mono

You also need to make doubly sure that there exists an environment variable called MONO_PATH that points to exactly this:
``C:\Program Files\Mono``

# Building the engine
To build the engine you should be good to just open the solution in Visual studio and build, all the dependencies are linked already.

Just make sure you build x64.

It may take a while as there are quite a few translation units to compile.

# Building the demo
To build to demo you first must build the script assemblies for the game. It's a little convoluted so heres the order.

First, you need to build AIM (the ai library), you can do this by opening the AIM solution file located here:

``dustbot-demo/aim/Aim.sln``

And building it as normal, you shouldnt run into any issues.

Then after building that, you can build the DustBot script assembly and horizon engine script assembly. You can do that by oppening this solution file:
``dustbot-demo/DustBot/DustBotScript.sln``

There is a build step that copies all the assemblies to the right place so you should just be able to build the DustbotGame project inside the main DustBot solution.
``dustbot-demo/DustBot/DustBot.sln``

This should output a compiled version of the demo to the Binaries folder.
``dustbot-demo/DustBot/Binaries/Debug/x64``

