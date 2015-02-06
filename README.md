uMatch
======
uMatch is a simple matching engine, which simulates the exchange functionality. uMatch uses the widely-popular QuickFIX FIX messaging engine to get Order messages in and to give out Market Data over FIX.

Features
========
- Works on FIX version 4.2
- Supports basic FIX messages NewOrderSingle, OrderCancelReplaceRequest, OrderCancelRequest
- Supports Market, Limit, Stop and Stop Limit order types
- Option to take market data on UDP or via a FIX session

Possible Uses
=============
- It can be used as a dummy exchange to test trading products.
- Can be used a matching engine for running a Bitcoin or betting exchange etc

Pre-requisites
==============
uMatch depends on the following libraries:
- Boost 1.51.0+
- QuickFIX (and libxml2)
- CMake 2.6+

How to Build
============
$ mkdir build
$ cmake ..
$ make

License
=======

Copyright 2015, uTrade Solutions Pvt Ltd

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

Contributions
=============
Pull Requests and Bug reports are welcome via Github.