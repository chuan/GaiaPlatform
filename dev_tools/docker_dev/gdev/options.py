#!/usr/bin/env python3

###################################################
# Copyright (c) Gaia Platform LLC
#
# Use of this source code is governed by the MIT
# license that can be found in the LICENSE.txt file
# or at https://opensource.org/licenses/MIT.
###################################################

"""
Module to represent the entire collection of options available from the command line.
"""

from dataclasses import dataclass
from typing import FrozenSet
from gdev.mount import Mount


# pylint: disable=too-many-instance-attributes
@dataclass(frozen=True)
class Options:
    """
    Class to encapsulate the options related to the command line.
    """

    args: str
    base_image: str
    cfg_enables: FrozenSet[str]
    force: bool
    log_level: str
    mixins: FrozenSet[str]
    mounts: FrozenSet[Mount]
    platform: str
    ports: FrozenSet[str]
    registry: str
    target: str


# pylint: enable=too-many-instance-attributes
