#!/bin/bash
set -xe
west build -b nrf52840dk/nrf52840 app
west flash
