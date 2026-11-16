# Compatriot: Wireless Challenges

## Tools

  * **nRF Connect for Mobile** (Nordic Semiconductor, free, iOS/Android), for every BLE challenge. Install it from the App Store or Play Store, open it once, and grant it Bluetooth permission.
  * **Any phone or laptop that can join an open WiFi network** , just a browser, for the honeypot challenge. Nothing to install.
  * **A monitor-mode-capable USB WiFi adapter + Wireshark + aircrack-ng** , only for the last challenge. Most laptop WiFi chips can't do monitor mode, you'll likely need a separate USB adapter. Confirm your adapter's chipset supports it beforehand.



On a Linux machine, in a terminal, install Wireshark and the aircrack-ng suite (`airmon-ng` lives in aircrack-ng):
    
    
    sudo apt install wireshark aircrack-ng

During install you may be asked "allow non-root users to capture packets". Say yes, then log out and back in once for the permission to take effect.

01

## PIN Crack (BLE)

Easy

The simplest possible BLE attack: a short numeric PIN gate on a writable characteristic, with no rate limiting at all.

#### Tool

nRF Connect for Mobile.

#### Approach

Scan, connect, and expand the one advertised service. Subscribe to notifications on the characteristic first, so every write's result shows up automatically, then write a 3-digit guess as text. The PIN space is small enough that a short brute-force by hand is reasonable.

02

## Honeypot (WiFi)

Easy

A small, realistic web-recon chain. One misconfiguration leaks a path, that path leaks a credential, and the credential unlocks a session-gated endpoint. It's exactly how real attackers chain together "boring" mistakes.

#### Tool

Any browser.

#### Approach

Join the badge's open access point. Check what the site tells crawlers _not_ to index, that's usually exactly what's worth looking at. Follow where it leads, recover the credential it leaks, and log in. Once authenticated, look for the action that view now exposes.

03

## Double Decode (BLE HexLock)

Medium

A two-stage BLE puzzle. One characteristic hands you an encoded clue. Decoding it tells you what to do next, not what to send, which are two different things.

#### Tool

nRF Connect for Mobile, plus a hex-decode tool (CyberChef works well).

#### Approach

Read the service's characteristics in UUID order. The lowest is read-only, and that's the clue. It looks like plain ASCII at a glance, but look closer at what those particular characters actually spell once decoded properly. The decoded phrase names an action, not the value to send. Write the short word it implies to the write-only characteristic, then read the third characteristic **without disconnecting in between**.

04

## Subscribe & Conquer (BLE NotifyLock)

Medium

Tests whether you understand BLE **notifications** , not just read/write. You have to subscribe to a characteristic _and_ satisfy an authentication write before anything happens. Neither alone is enough, and the order between them doesn't matter.

#### Tool

nRF Connect for Mobile, plus a base64 decoder.

#### Approach

Read the clue characteristic, its value is base64, not plain text. Decode it to get the exact string the auth characteristic expects. Enable notifications on the notify-only characteristic, write the decoded string to the auth characteristic, and watch the notify characteristic. Once both conditions are satisfied, it pushes the flag on its own.

If nothing arrives, you're likely missing one of the two steps, or there's a stray space in what you typed. The badge stays silent on a wrong auth string by design, there's no error to lean on.

05

## Packet Ghost (WiFi)

Hard

802.11 monitor-mode packet capture. The flag is split across two beacon frames sent well apart in time, so this takes patience and a filter, not just a single capture. It's the only wireless challenge requiring monitor mode.

#### Tool

A monitor-mode-capable USB WiFi adapter + Wireshark.

#### Approach

Put your adapter into monitor mode on the badge's broadcast channel, start a capture, and filter down to just this badge's SSID to cut ambient noise. Let it run for well over the badge's full beacon cycle, a short capture will only ever catch one fragment. Expand the vendor-specific information element on two different beacon frames to find both fragments, and join them in the order they occurred.
