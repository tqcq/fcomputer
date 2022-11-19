# Create an N x 5 grid of connected constant combinators, with every item and their stack size
import sys
import struct

from draftsman.blueprintable import Blueprint
from draftsman.constants import Direction
from draftsman.data import items
from draftsman.entity import ConstantCombinator, DeciderCombinator
from draftsman.data import signals
import string

COMBINATOR_HEIGHT = 3


def AddToList(dst_list, src_list=[]):
    for item in src_list:
        if item not in dst_list:
            dst_list.append(item)
    return dst_list


def ReadBin(rom_path):
    instructions = []
    with open(rom_path, "r") as f:
        data = f.read(-1).split(",")
        instructions = [int(x, 16) for x in data]
    result = []
    for i in range(0, len(instructions), 4):
        result.append(
            (instructions[0 + i] << 24) +
            (instructions[1 + i] << 16) +
            (instructions[2 + i] << 8) +
            (instructions[3 + i])
        )
        if result[-1] > 0x7FFF_FFFF:
            result[-1] = (-1 << 32) | result[-1]

    return result


def AddCol(data, signals_all, blueprint, x):
    y = 0
    dec_combinator = DeciderCombinator()
    dec_combinator.set_decider_conditions("signal-I", "==", x + 1, "signal-everything")
    dec_combinator.tile_position = (x, y)
    dec_combinator.direction = Direction.NORTH
    dec_combinator.id = "dec_{}_{}".format(x, y)
    blueprint.entities.append(dec_combinator)
    y += 2

    const_combinator = ConstantCombinator()
    const_combinator.direction = Direction.NORTH
    const_combinator.tile_position = (x, y)
    const_combinator.id = "con_{}_{}".format(x, y)
    for i in range(len(data)):
        idx = (i % const_combinator.item_slot_count)
        signal = signals_all[i]
        const_combinator.set_signal(idx, signal, int(data[i]))
        if (idx == const_combinator.item_slot_count-1) or (i == len(data) - 1):
            print(len(data), i)
            blueprint.entities.append(const_combinator)
            if y == 2:
                blueprint.add_circuit_connection("green", "con_{}_{}".format(x, y), "dec_{}_{}".format(x, y - 2), 1, 1)
            else:
                blueprint.add_circuit_connection("green", "con_{}_{}".format(x, y), "con_{}_{}".format(x, y - 1), 1, 1)
            y += 1
            const_combinator = ConstantCombinator()
            const_combinator.direction = Direction.NORTH
            const_combinator.tile_position = (x, y)
            const_combinator.id = "con_{}_{}".format(x, y)


def main(rom_path):
    blueprint = Blueprint()
    instructions = ReadBin(rom_path)
    for x in instructions:
        print("%#x" % x)

    signals_all = []
    signals_all = AddToList(signals_all, signals.item)
    signals_all = AddToList(signals_all, signals.fluid)
    signals_all = AddToList(signals_all, signals.virtual)
    signals_all = AddToList(signals_all, signals.raw)
    signals_all = AddToList(signals_all, signals.pure_virtual)


    # signals_all = [*set(signals_all)]

    # for ch in string.ascii_uppercase:
    #    signals_all.remove("signal-{}".format(ch))
    for ch in 'IW':
        signal = "signal-{}".format(ch)
        if signal in signals_all:
            signals_all.remove(signal)
    # signals_all.remove("signal-dot")
    # signals_all.remove("signal-info")

    for i in range(0, len(instructions), len(signals_all)):
        end_pos = i + len(signals_all)
        x = int(i / len(signals_all))
        if end_pos > len(instructions):
            AddCol(instructions[i:], signals_all, blueprint, x)
        else:
            AddCol(instructions[i:i + len(signals_all)], signals_all, blueprint, x)
        if x > 0:
            blueprint.add_circuit_connection("red", "dec_{}_0".format(x), "dec_{}_0".format(x - 1), 1, 1)

    print(blueprint.to_string())


if __name__ == "__main__":
    main(sys.argv[1])
