============================================
Things to do after a new Ubuntu installation
============================================

As a modern operating system, Ubuntu features online update, not only for
system component, but also for various application software.  You can find all
kinds of software in Ubuntu **repository**.

The first thing to do after you have installed Ubuntu, is to go to your
*System* -> *Administration* -> *Software Sources* to **change the location of
system repository**.  After open to setting window, you will see a *Ubuntu
Software* tab and there's a drop-down box named *Download from*.  Hit the
drop-down box and select *Others ...*, another dialog will pop out to let you
to *Choose a Download Server*.  You will notice that there is a button named
*Select Best Server* in the right portion of the dialog.  Click it to make it
to detect the fastest mirror site for you.  Press *Choose Server* button.

*Software Sources* will try to reload the index of the online repository after
you change the location so you need to have Internet connection.  After the
index is reloaded, you can use *System* -> *Administration* -> *Synaptic
Package Manager*.  If you are unfamiliar with how the software is organized on
the advanced system such as Ubuntu, you should not skip the introductory
message shown when you first run *Synaptic*.  You can install any software you
want in the interface offered by *Synaptic*.

You should press *Mark All Upgrade* to upgrade your system up-to-date.  Ubuntu
constantly update their repository and you should upgrade to the most current
version timely for your goodness.  Your system will automatically check the
online to see if upgrade is needed, but in general it doesn't happen right
after your installation of system.  Therefore you should manually upgrade.
After *Mark All Upgrade*, if there's anything needs to be upgraded, the *Apply*
button will be enable.  Just click it and *Synaptic* will download the software
packages and install them.

The current release of Ubuntu (9.10) included a buggy ``network-manager``
package.  It is responsible for manage your network connection such as Ethernet
or WiFi.  Usually it works fine, but when it comes to WiFi connection with
**PEAP**, you could experience random disconnection and it can't remember your
seetings.  The workaround now is not to use the default ``network-manager``
package, but to install another "network manager" named ``wicd``.  You can
install it in *Synaptic*.


``wicd`` is far more stable than the ``network-manager`` included in Ubuntu
9.10.  Although the user interface of ``wicd`` is a little bit different than
``network-manager``, the network settings is still trivial.
