==================================
OSU VPN (Cisco) for Linux (Debian)
==================================

Sometimes you need to access resources on OSU network but you are outside the
network, and you are using Linux.  OSU provides us *Cisco AnyConnect VPN Client
2.4* for Linux, which can be downloaded from the SLS website.  However, for
Debian users, it is more straightforward to use `vpnc
<http://www.unix-ag.uni-kl.de/~massar/vpnc/>`_ to access the VPN, since it's
already in the standard repository.  Another reason to use ``vpnc`` is that,
the software downloaded from SLS is not compatible to my ``amd64`` ``squeeze``,
i.e., it doesn't run on my laptop.  After installing ``vpnc`` using ``apt-get
install vpnc``, there are just few steps before you can connect to the VPN.

No information on the `8help page <http://8help.osu.edu/1595.html>`_ is
provided for the configuration of ``vpnc``, which does not surprise me since
Linux is not officially supported by OSU.  Peeking at the settings in the
Windows version Cicso VPN software will help you to create the corresponding
configuration file in ``/etc/vpnc`` directory for ``vpnc``.  A configuration
file (with extension file name ``.conf``) should be created in the directory::

  Local Port 0
  IPSec gateway vpn.service.ohio-state.edu
  IPSec ID <find_it_in_pcf>
  IPSec secret <find_it_in_pcf>
  IKE Authmode psk
  Xauth username <your_osu_username>

``IPSec ID`` is the ``GroupName`` field in the PCF file of Cisco VPN client,
while the ``IPSec secret`` is the ``GroupPwd`` field in the PCF file.  You can
find the PCF file in the directory you installed the Cisco VPN client that OSU
provides.  ``GroupPwd`` is encrypted, but there's a way to decrypt it, or you
can ask OIT.

After filling all relevant information for the configuration file, you can
start the VPN by using ``vpnc <conf_file_name>`` with root privilege.  ``vpnc``
will prompt for your password every time when connecting.  Warning, although
``Xauth password`` can be used to specify your personal credential to the VPN,
for security reason, you should not really write it in the configuration file.
You can verify the routing table by using ``route``.  You should see the
``tun0`` device is added into the routing table for OSU network.

To disconnect from the VPN, use ``vpnc-disconnect`` with root privilege.  Do
not forget to disconnect before suspend/hibernate your laptop.
