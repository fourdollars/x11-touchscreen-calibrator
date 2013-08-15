x11-ts-cal
==========================

X Window System's Touchscreen Calibrator

The purpose is to calibrate the touchscreen's coordinates automatically.

Build from source
=================

Prerequisite
------------

Debian/Ubuntu

    libxi-dev
    libx11-dev
    x11proto-input-dev

Fedora/openSUSE/Mageia

    libXi-devel
    libX11-devel

Compilation
-----------

    autoreconf -if
    ./configure
    make

Coordinate Transformation Matrix
================================

Zoom
----

Restrict the cursor movement within the left half side of the touchscreen display.

    ⎡ 0.5 , 0 , 0 ⎤
    ⎜ 0   , 1 , 0 ⎥
    ⎣ 0   , 0 , 1 ⎦

$ xinput set-prop '&lt;device name&gt;' 'Coordinate Transformation Matrix' 0.5 0 0 0 1 0 0 0 1

Restrict the cursor movement within the top half side of the touchscreen display.

    ⎡ 1 , 0   , 0 ⎤
    ⎜ 0 , 0.5 , 0 ⎥
    ⎣ 0 , 0   , 1 ⎦

$ xinput set-prop '&lt;device name&gt;' 'Coordinate Transformation Matrix' 1 0 0 0 0.5 0 0 0 1

Map the cursor movement to the left half touchscreen area.

    ⎡ 2 , 0 , 0 ⎤
    ⎜ 0 , 1 , 0 ⎥
    ⎣ 0 , 0 , 1 ⎦

$ xinput set-prop '&lt;device name&gt;' 'Coordinate Transformation Matrix' 2 0 0 0 1 0 0 0 1

Map the cursor movement to the top half touchscreen area.

    ⎡ 1 , 0 , 0 ⎤
    ⎜ 0 , 2 , 0 ⎥
    ⎣ 0 , 0 , 1 ⎦

$ xinput set-prop '&lt;device name&gt;' 'Coordinate Transformation Matrix' 1 0 0 0 2 0 0 0 1

Translation
-----------

Make the cursor shift right by half the width of the touchscreen.

    ⎡ 1 ,  0 ,  0.5 ⎤
    ⎜ 0 ,  1 ,  0   ⎥
    ⎣ 0 ,  0 ,  1   ⎦

$ xinput set-prop '&lt;device name&gt;' 'Coordinate Transformation Matrix' 1 0 0.5 0 1 0 0 0 1

Make the cursor shift left by half the width of the touchscreen.

    ⎡ 1 ,  0 ,  -0.5 ⎤
    ⎜ 0 ,  1 ,   0   ⎥
    ⎣ 0 ,  0 ,   1   ⎦

$ xinput set-prop '&lt;device name&gt;' 'Coordinate Transformation Matrix' 1 0 -0.5 0 1 0 0 0 1

Make the cursor shift down by half the height of the touchscreen.

    ⎡ 1 ,  0 ,  0   ⎤
    ⎜ 0 ,  1 ,  0.5 ⎥
    ⎣ 0 ,  0 ,  1   ⎦

$ xinput set-prop '&lt;device name&gt;' 'Coordinate Transformation Matrix' 1 0 0 0 1 0.5 0 0 1

Make the cursor shift up by half the height of the touchscreen.

    ⎡ 1 ,  0 ,   0   ⎤
    ⎜ 0 ,  1 ,  -0.5 ⎥
    ⎣ 0 ,  0 ,   1   ⎦

$ xinput set-prop '&lt;device name&gt;' 'Coordinate Transformation Matrix' 1 0 0 0 1 -0.5 0 0 1

Zoom x Translation
------------------

For example, if we want the center 1/4 area of the touchscreen to operate the fullscreen cursor movement.

    ⎡ 2 , 0 , 0 ⎤   ⎡ 1 , 0 , 0 ⎤   ⎡ 1 ,  0 ,  -0.25 ⎤   ⎡ 1 ,  0 ,   0    ⎤
    ⎜ 0 , 1 , 0 ⎥ X ⎜ 0 , 2 , 0 ⎥ X ⎜ 0 ,  1 ,   0    ⎥ X ⎜ 0 ,  1 ,  -0.25 ⎥
    ⎣ 0 , 0 , 1 ⎦   ⎣ 0 , 0 , 1 ⎦   ⎣ 0 ,  0 ,   1    ⎦   ⎣ 0 ,  0 ,   1    ⎦

equals to

    ⎡ 2 , 0 , 0 ⎤   ⎡ 1 ,  0 ,  -0.25 ⎤
    ⎜ 0 , 2 , 0 ⎥ X ⎜ 0 ,  1 ,  -0.25 ⎥
    ⎣ 0 , 0 , 1 ⎦   ⎣ 0 ,  0 ,   1    ⎦

equals to

    ⎡ 2 , 0 , -0.5 ⎤
    ⎜ 0 , 2 , -0.5 ⎥
    ⎣ 0 , 0 ,  1   ⎦

$ xinput set-prop '&lt;device name&gt;' 'Coordinate Transformation Matrix' 2 0 -0.5 0 2 -0.5 0 0 1

Scaling mode
============

The touchscreen display may support four different modes, such as 'None', 'Full', 'Center' and 'Full aspect'.

    pw = preferred width
    ph = preferred height
    (The preferred resolution of touchscreen display.)

    sw = screen width
    sh = screen height
    (The virtual screen of X Window System.)

    dw = display width
    dh = display height
    (The actual resolution of touchscreen display.)

'None' mode
-----------

The behavior is undefined. It may work like 'Full' mode or 'Center' mode.

'Full' mode
-----------

    ⎡ dw / sw ,  0       ,  dx / sw ⎤
    ⎜ 0       ,  dh / sh ,  dy / sh ⎥
    ⎣ 0       ,  0       ,  1       ⎦

equals to

    ⎡ 1 ,  0 ,  dx / sw ⎤   ⎡ dw / sw , 0       , 0 ⎤
    ⎜ 0 ,  1 ,  dy / sh ⎥ X ⎜ 0       , dh / sh , 0 ⎥
    ⎣ 0 ,  0 ,  1       ⎦   ⎣ 0       , 0       , 1 ⎦


'Center' mode
-------------

    ⎡ pw / dw ,  0       ,  (dw - pw) / sw / 2 + dx / sw ⎤
    ⎜ 0       ,  ph / sh ,  (dh - ph) / sh / 2 + dy / sh ⎥
    ⎣ 0       ,  0       ,  1                            ⎦

equals to

    ⎡ dw / sw ,  0       ,  dx / sw ⎤   ⎡ pw / dw , 0       , (dw - pw) / dw / 2 ⎤
    ⎜ 0       ,  dh / sh ,  dy / sh ⎥ X ⎜ 0       , ph / dh , (dh - ph) / dh / 2 ⎥
    ⎣ 0       ,  0       ,  1       ⎦   ⎣ 0       , 0       , 1                  ⎦

equals to

    ⎡ dw / sw ,  0       ,  dx / sw ⎤   ⎡ pw / dw , 0       , 0 ⎤   ⎡ 1 , 0 , - (1 - dw / pw) / 2 ⎤
    ⎜ 0       ,  dh / sh ,  dy / sh ⎥ X ⎜ 0       , ph / dh , 0 ⎥ X ⎜ 0 , 1 , - (1 - dh / ph) / 2 ⎥
    ⎣ 0       ,  0       ,  1       ⎦   ⎣ 0       , 0       , 1 ⎦   ⎣ 0 , 0 , 1                   ⎦

'Full aspect' mode
------------------

(with left and right side blank)

    ⎡ pw * dh / ph / sw ,  0       ,  (1 - pw * dh / ph / dw) * dw / sw / 2 + dx / sw ⎤
    ⎜ 0                 ,  dh / sh ,  dy / sh                                         ⎥
    ⎣ 0                 ,  0       ,  1                                               ⎦

equals to

    ⎡ dw / sw ,  0       ,  dx / sw ⎤   ⎡ pw * dh / ph / dw , 0 , (1 - pw * dh / ph / dw) / 2 ⎤
    ⎜ 0       ,  dh / sh ,  dy / sh ⎥ X ⎜ 0                 , 1 , 0                           ⎥
    ⎣ 0       ,  0       ,  1       ⎦   ⎣ 0                 , 0 , 1                           ⎦

equals to

    ⎡ dw / sw ,  0       ,  dx / sw ⎤   ⎡ pw * dh / ph / dw , 0 , 0 ⎤   ⎡ 1 , 0 , (1 - pw * dh / ph / dw) / 2 ⎤
    ⎜ 0       ,  dh / sh ,  dy / sh ⎥ X ⎜ 0                 , 1 , 0 ⎥ X ⎜ 0 , 1 , 0                           ⎥
    ⎣ 0       ,  0       ,  1       ⎦   ⎣ 0                 , 0 , 1 ⎦   ⎣ 0 , 0 , 1                           ⎦

Rotation
========

$ xrandr -o left

    ⎡ 0 -1 1 ⎤
    ⎜ 1  0 0 ⎥
    ⎣ 0  0 1 ⎦

$ xrandr -o right

    ⎡  0 1 0 ⎤
    ⎜ -1 0 1 ⎥
    ⎣  0 0 1 ⎦

$ xrandr -o inverted

    ⎡ -1  0 1 ⎤
    ⎜  0 -1 1 ⎥
    ⎣  0  0 1 ⎦

Reflection
==========

$ xrandr -x

    ⎡ -1 0 1 ⎤
    ⎜  0 1 0 ⎥
    ⎣  0 0 1 ⎦

$ xrandr -y

    ⎡ 1  0 0 ⎤
    ⎜ 0 -1 1 ⎥
    ⎣ 0  0 1 ⎦
