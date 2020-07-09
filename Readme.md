#Dustbot Demo Requirements

You'll need mono installed on your machine to run the demo in visual studio.

Here's a link to the installer:
https://download.mono-project.com/archive/5.14.0/windows-installer/mono-5.14.0.177-x64-0.msi

it has to get installed to this dir: C:\Program Files\Mono\

You'll need to make sure the $(MONO_PATH) variable is setup like this:
C:\Program Files\Mono

And if you install it while the project is open, you'll need to restart visual studio for it to load the environment variable changes.
