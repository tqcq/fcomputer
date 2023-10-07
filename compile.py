import csv

from draftsman.blueprintable import Blueprint
from draftsman.constants import Direction
from draftsman.data import items
from draftsman.entity import ConstantCombinator
from draftsman.entity import DeciderCombinator
from draftsman.data import signals

debug = False

hide_signals = []

filename = "program.csv"

def CreateCell(all_signals, x, y):
    cell_select = DeciderCombinator()
    cell_select.direction = Direction.WEST
    cell_select.tile_position = (x, y)
    cell_select.id = "{}_{}_select".format(x, y);
    cell_select.set_decider_conditions("signal-I", "=", 1000001 + y, "signal-everything")

    x += 2
    cell_data = ConstantCombinator()
    cell_data.direction = Direction.WEST
    cell_data.tile_position = (x, y)
    cell_data.position = (x, y)
    cell_data.id = "{}_{}_data".format(x, y);

    cells = [cell_data]

    count = 0
    for key in all_signals.keys():
        cell_data.set_signal(count, key, all_signals[key])

        count += 1
        if count == 20:
            x += 1
            count = 0
            cell_data = ConstantCombinator()
            cell_data.direction = Direction.WEST
            cell_data.tile_position = (x, y)
            cell_data.position = (x, y)
            cell_data.id = "{}_{}_data".format(x, y);
            cells.append(cell_data)
    if count == 0:
        cells.pop()

    return [cell_select, *cells]

def ToSigned(x):
    x= x& 0xFFFFFFFF
    if (x& 0x80000000) != 0: 
        x -= 1
        x = (~x) & 0xFFFFFFFF
        x = -x
    return x 

def ParseProgram(blueprint):
    keys = [*hide_signals, *signals.item, *signals.fluid, *signals.virtual]
    keys.remove("signal-I")
    keys.remove("signal-W")
    keys.remove("signal-X")
    signals_count = 0

    with open(filename) as f:
        render = csv.reader(f)
        # header_row = next(render)

        # print(header_row)
        x, y = 0, 0
        idx = 0
        all_signals = {}
        for row in render:
            cnt = 0
            tmp = 0
            for item in row:
                try:
                    tmp = (tmp << 8) + int(item)
                    if debug:
                        print("tmp> ", tmp)
                    cnt = cnt+1
                    if cnt == 4:
                        all_signals[keys[idx]] =  ToSigned(tmp)
                        cnt = 0
                        tmp = 0
                        if debug:
                            print(idx, tmp, hex(tmp), (keys[idx]))
                            print("all_signals> ", all_signals)

                        idx += 1
                        if idx >= len(keys):
                            idx = 0
                            cell = CreateCell(all_signals, x, y)
                            for item in cell:
                                blueprint.entities.append(item)
                            signals_count += len(all_signals)
                            all_signals = {}
                            y += 1
                except ValueError:
                    # do nothing
                    pass
        signals_count += len(all_signals)
        cell = CreateCell(all_signals, x, y)
        for item in cell:
            blueprint.entities.append(item)

        return signals_count
            

if __name__ == "__main__":
    keys = [*hide_signals, *signals.item, *signals.fluid, *signals.virtual]
    cnt = 1
    for key in keys:
    #    print(cnt, ": ", key)
        cnt += 1

    cnt = 1
    for item in items.raw:
        if "flags" in items.raw[item]:
            if "hidden" in items.raw[item]["flags"]:
                continue
        if items.raw[item]["name"] in keys:
            continue
        print(cnt, ": ", items.raw[item]["name"])
        cnt += 1

    blueprint = Blueprint()
    signals_count= ParseProgram(blueprint) 
    print("signals_count: ", signals_count)

    row = int((signals_count + 259) / 259)
    y = 0
    for id in range(row):
        combinator_cnt = int((signals_count + 19) / 20)
        select_id = "{}_{}_select".format(0, y)
        for x in range(combinator_cnt):
            data_id = "{}_{}_data".format(x + 2, y)
            prev_id = "{}_{}_data".format(x + 2 - 1, y)
            # print(x, prev_id, data_id)
            if x == 0:
                blueprint.add_circuit_connection("green", select_id, data_id, 1, 1)
            else:
                blueprint.add_circuit_connection("green", prev_id, data_id, 1, 1)

            #blueprint.add_circuit_connection("green", prev_select_id, select_id, 2, 2)
            #blueprint.add_circuit_connection("red", prev_select_id, select_id, 1, 1)
        y += 1
        signals_count -= 259
    print(blueprint.to_string())
