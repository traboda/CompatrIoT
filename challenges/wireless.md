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

**Hint** 

No lockout, no rate limit. The initial value even tells you the PIN is 3 digits. Just try them.

**Full guide** 

  1. Open nRF Connect, scan, and connect to the badge's BLE advertisement.
  2. Expand the one service. There's one characteristic that supports both write and notify.
  3. Tap the notify icon first, so every write's result shows up automatically below the characteristic.
  4. Tap the write icon, switch the value type to text, enter `123`, and send.
  5. Watch the value update. A wrong guess changes it to an error message, a correct one changes it to the flag.



02

## Honeypot (WiFi)

Easy

A small, realistic web-recon chain. One misconfiguration leaks a path, that path leaks a credential, and the credential unlocks a session-gated endpoint. It's exactly how real attackers chain together "boring" mistakes.

#### Tool

Any browser.

#### Approach

Join the badge's open access point. Check what the site tells crawlers _not_ to index, that's usually exactly what's worth looking at. Follow where it leads, recover the credential it leaks, and log in. Once authenticated, look for the action that view now exposes.

**Hint** 

robots.txt tells you what they didn't want indexed, which is exactly what you want to look at.

**Full guide** 

  1. Join the open access point and browse to the badge's landing page.
  2. Open `/robots.txt`. It disallows a couple of paths, which is a map of exactly where to look next.
  3. Open the disallowed notes page. It leaks a username and password in plain text.
  4. Open the maintenance page and log in with those credentials.
  5. The authenticated view exposes a button or action that wasn't there before. Use it. The response carries the flag.



Or do the whole thing from a terminal instead of a browser:
    
    
    curl http://192.168.4.1/robots.txt
    curl http://192.168.4.1/dev-notes.txt
    curl -i -X POST http://192.168.4.1/login -d 'u=technician&p;=<password from the notes page>'
    # copy the session value out of the Set-Cookie header in that response, then:
    curl -X POST http://192.168.4.1/maintenance/diagnostic -H 'Cookie: sid=<paste it here>'

03

## Double Decode (BLE HexLock)

Medium

A two-stage BLE puzzle. One characteristic hands you an encoded clue. Decoding it tells you what to do next, not what to send, which are two different things.

#### Tool

nRF Connect for Mobile, plus a hex-decode tool (CyberChef works well).

#### Approach

Read the service's characteristics in UUID order. The lowest is read-only, and that's the clue. It looks like plain ASCII at a glance, but look closer at what those particular characters actually spell once decoded properly. The decoded phrase names an action, not the value to send. Write the short word it implies to the write-only characteristic, then read the third characteristic **without disconnecting in between**.

**Hint** 

The clue tells you what to do, not what to send. Pair up the hex digits two at a time and convert each pair to an ASCII character.

**Full guide** 

  1. Connect and expand the service. Three characteristics appear in UUID order: a read-only clue, a write-only unlock, and a read-only flag.
  2. Read the clue. Its value is a string of hex digit pairs, written out as ASCII characters.
  3. Hex-decode it (CyberChef's "From Hex", or `python3 -c "print(bytes.fromhex('...').decode())"`). The decoded phrase names an action, something like "write" plus a short word.
  4. Write just that short word to the unlock characteristic.
  5. Without disconnecting, read the flag characteristic. Its value has changed.



04

## Subscribe & Conquer (BLE NotifyLock)

Medium

Tests whether you understand BLE **notifications** , not just read/write. You have to subscribe to a characteristic _and_ satisfy an authentication write before anything happens. Neither alone is enough, and the order between them doesn't matter.

#### Tool

nRF Connect for Mobile, plus a base64 decoder.

#### Approach

Read the clue characteristic, its value is base64, not plain text. Decode it to get the exact string the auth characteristic expects. Enable notifications on the notify-only characteristic, write the decoded string to the auth characteristic, and watch the notify characteristic. Once both conditions are satisfied, it pushes the flag on its own.

If nothing arrives, you're likely missing one of the two steps, or there's a stray space in what you typed. The badge stays silent on a wrong auth string by design, there's no error to lean on.

**Hint** 

Two things need to be true at once: you're listening, and you've proven who you are. Base64-decode the clue to find the exact auth string.

**Full guide** 

  1. Connect and expand the service. Three characteristics: a read-only clue, a write-only auth, and a notify-only event.
  2. Read the clue. Its value is base64. Decode it (CyberChef's "From Base64", or `python3 -c "import base64; print(base64.b64decode('...').decode())"`).
  3. Enable notifications on the event characteristic.
  4. Write the decoded string to the auth characteristic, exactly as decoded, no stray leading or trailing space.
  5. Watch the event characteristic. Once both steps are done, a notification carrying the flag arrives on its own.



05

## Packet Ghost (WiFi)

Hard

802.11 monitor-mode packet capture. The flag is split across two beacon frames sent well apart in time, so this takes patience and a filter, not just a single capture. It's the only wireless challenge requiring monitor mode.

#### Tool

A monitor-mode-capable USB WiFi adapter + Wireshark.

#### Approach

Put your adapter into monitor mode on the badge's broadcast channel, start a capture, and filter down to just this badge's SSID to cut ambient noise. Let it run for well over the badge's full beacon cycle, a short capture will only ever catch one fragment. Expand the vendor-specific information element on two different beacon frames to find both fragments, and join them in the order they occurred.

**Hint** 

One beacon won't have the whole flag. Wait for the second one, it's sent on a longer delay than the first.

**Full guide** 

  1. Put your adapter into monitor mode on the badge's channel (`airmon-ng start <iface> <channel>`, or the equivalent `iw` commands).
  2. Start Wireshark on the monitor interface and filter on `wlan.ssid == "..."` to cut the noise down to just this badge.
  3. Let it capture for at least 10-12 seconds, long enough to see a full cycle of both fragments.
  4. Click a beacon frame and expand the vendor-specific information element under the 802.11 management tag. The raw bytes there are fragment text.
  5. Check a second, later beacon frame the same way. You should find a different fragment there.
  6. Join the two fragments in the order they appeared in time.


