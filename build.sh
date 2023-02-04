#!/usr/bin/env bash

third_party/gn/out/gn gen out
ninja -C out

patchelf --replace-needed libcpr.so.1 /usr/local/lib/libcpr.so.1 out/kscord
patchelf --replace-needed libcpr.so.1 /usr/local/lib/libcpr.so.1 out/ut_kscord
