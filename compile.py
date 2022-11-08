import csv

from draftsman.blueprintable import Blueprint
from draftsman.constants import Direction
from draftsman.data import items
from draftsman.entity import ConstantCombinator
from draftsman.entity import DeciderCombinator
from draftsman.data import signals

filename = "program.csv"

def CreateCell(signals, x, y):
    cell_select = DeciderCombinator()
    cell_select.direction = Direction.WEST
    cell_select.tile_position = (x, y)
    cell_select.id = "{}_select".format(y);
    cell_select.set_decider_conditions("signal-P", "=", 0, "signal-everything")

    cell_data = ConstantCombinator()
    cell_data.direction = Direction.WEST
    cell_data.tile_position = (x + 2, y)
    cell_data.id = "{}_data".format(y);

    count = 0
    for key in signals.keys():
        cell_data.set_signal(count, "signal-{}".format(key), signals[key])
        count += 1

    return [cell_select, cell_data]

def ParseProgram(blueprint):
    with open(filename) as f:
        render = csv.reader(f)
        header_row = next(render)

        # print(header_row)
        x, y = 0, 0
        for row in render:
            signals = {}
            for i in range(len(header_row)):
                try:
                    if i == 0:
                        signals[header_row[i].strip()] = -int(row[i])
                    else:
                        signals[header_row[i].strip()] = int(row[i])
                except ValueError:
                    # do nothing
                    pass

            print(y, " : ", signals)
            cell = CreateCell(signals, x, y)
            y += 1
            for item in cell:
                blueprint.entities.append(item)

if __name__ == "__main__":
    blueprint = Blueprint()
    ParseProgram(blueprint)
    count = int(len(blueprint.entities) / 2)
    for id in range(count):
        select_id = "{}_select".format(id)
        data_id = "{}_data".format(id)
        prev_select_id = "{}_select".format(id-1)
        blueprint.add_circuit_connection("green", data_id, select_id, 1, 1)
        if id > 0:
            blueprint.add_circuit_connection("green", prev_select_id, select_id, 2, 2)
            blueprint.add_circuit_connection("red", prev_select_id, select_id, 1, 1)
    print(blueprint.to_string())
