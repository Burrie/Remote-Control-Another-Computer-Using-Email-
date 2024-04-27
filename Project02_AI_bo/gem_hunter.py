from pysat.formula import *
import csv

height = 0
width = 0

#2D-character array
map = []

#CNFs list
cnfs = Atom(True)

#----------------------------------------------------------------------------------

#void funtion
def generate_map(filepath: str):
    global height
    global width
    global map

    #Used to mark cell - Gem or Trap
    mark = 10
    
    #Read csv file and generate map
    with open(file= filepath) as csvfile:
        csv_reader = csv.reader(csvfile)
        for row in csv_reader:
            height += 1
            width = len(row)
            tmp_list = row # character list
            for i in range(0, len(tmp_list)):
                if(tmp_list[i] == '_'):
                    tmp_list[i] = str(mark)
                    mark += 1
            map.append(tmp_list)
        csvfile.close()


#return 2D-int array
def get_position_to_generate_cnf(start: int, num_trap: int, len_mark_array: int):
    position_mark_array = []

    for i in range(start, len_mark_array):
        tmp_position_array = []
        if(num_trap > 1):
            if(i + num_trap > len_mark_array): #Still need atom but run out of element in the mark_array
                break
            
            tmp_position_array = get_position_to_generate_cnf(start= i + 1, num_trap= num_trap - 1, len_mark_array= len_mark_array)
            
            #Everything is ok -> append each array in 2D-array
            for array in tmp_position_array:
                array.append(i)
                position_mark_array.append(array)        
        else:
            tmp_position_array.append(i)
            position_mark_array.append(tmp_position_array)
    
    return position_mark_array


def generate_cnf(mark_array, num_trap_on_map: int):
    global cnfs

    #2D-int array
    position_mark_array = get_position_to_generate_cnf(start= 0, num_trap= num_trap_on_map, len_mark_array= len(mark_array))

    tmp_cnfs = Atom(False)

    for array in position_mark_array:
        cnf = Atom(True)
        for i in range(0, len(mark_array)):
            shouldAddNeg = True
            for int in array:
                if(i == int):
                    cnf = And(cnf, Atom(mark_array[i]), merge= True)
                    shouldAddNeg = False
                    break
            if(shouldAddNeg == True):
                cnf = And(cnf, Neg(Atom(mark_array[i])), merge= True)
        tmp_cnfs = Or(cnf, tmp_cnfs, merge= True)
    
    cnfs = And(cnfs, tmp_cnfs, merge= True)
  

def look_around(h : int, w : int):
    global height
    global width
    global map

    mark_array = []

    for y in range(-1, 2):
        if y + h < 0 or y + h >= height:
            continue
        for x in range(-1, 2):
            if x + w < 0 or x + w >= width:
                continue
            #Find cells with marks, not cells with numbers indicating traps around
            if int(map[y + h][x + w]) >= 10:
                mark_array.append( int(map[y + h][x + w]) )
                
    #Number of variables around is less than number of traps surround -> Error
    if len(mark_array) < int(map[h][w]):
        return
    
    generate_cnf(mark_array= mark_array, num_trap_on_map= int(map[h][w]))
    
    
def read_map():
    global height
    global width
    global map

    for h in range(0, height):
        for w in range(0, width):
            if int(map[h][w]) < 10: #Found cell with number indicating number of traps surround
                look_around(h= h, w= w)

generate_map("test.csv")
print(map)
read_map()
print(cnfs)