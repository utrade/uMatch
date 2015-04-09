# uMatch

uMatch is a simple matching engine, which simulates the exchange functionality. uMatch uses the widely-popular QuickFIX FIX messaging engine to get Order messages in and to give out Market Data over FIX.

## Features

- Works on FIX version 4.2
- Supports basic FIX messages NewOrderSingle, OrderCancelReplaceRequest, OrderCancelRequest
- Supports Market, Limit, Stop and Stop Limit order types
- Option to take market data on UDP or via a FIX session

## Possible Uses

- It can be used as a dummy exchange to test trading products.
- Can be used a matching engine for running a Bitcoin or betting exchange etc

## Pre-requisites

uMatch depends on the following libraries:
- Boost 1.51.0+
- QuickFIX (and libxml2)
- CMake 2.6+

## How to Build

    $ mkdir build
    $ cmake ..
    $ make

## License

    uMatch, a simplified exchange matching engine
    Copyright (C) 2015  uTrade Solutions Pvt Ltd

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

## Contributions

Pull Requests and Bug reports are welcome via Github.