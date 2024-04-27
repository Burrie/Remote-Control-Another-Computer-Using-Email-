from pysat.formula import *
import csv

height = 0
width = 0

#2D-character array
map = []

#All of CNF clauses generated
CNFs = CNF()

#----------------------------------------------------------------------------------

#void funtion
def generate_map(filepath: str):
    global height
    global width
    global map

    #Used to mark logical variable - Gem or Trap
    #Start from 10 because a cell just 8 cells surrond -> maximum number can be used to indicate traps around is 8
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


#return 2D-int array: Position of element in mark_array
def get_position_to_generate_DNF(start: int, num_trap: int, len_mark_array: int):
    position_mark_array = []

    for i in range(start, len_mark_array):
        tmp_position_array = []
        if(num_trap > 1):
            if(i + num_trap > len_mark_array): #Still need atom but run out of element in the mark_array
                break
            
            tmp_position_array = get_position_to_generate_DNF(start= i + 1, num_trap= num_trap - 1, len_mark_array= len_mark_array)
            
            #Everything is ok -> append each array in 2D-array
            for array in tmp_position_array:
                array.append(i)
                position_mark_array.append(array)        
        else:
            tmp_position_array.append(i)
            position_mark_array.append(tmp_position_array)
    
    return position_mark_array


#return 2D-int array: Each array in DNF_array is a clause in DNF form (Disjunctive Normal Form) - variables in a clause are connected via Conjunction
def generate_DNF(mark_array, num_trap_on_map: int):
    #2D-int array of positions in mark_array to convert array's elements into DNF -> then CNF 
    position_mark_array = get_position_to_generate_DNF(start= 0, num_trap= num_trap_on_map, len_mark_array= len(mark_array))

    #Get all clauses in DNF form (Disjunctive Normal Form)
    for i in range(0, len(position_mark_array)):
        array = []
        for j in mark_array:
            array.append(-j)
        for j in position_mark_array[i]: #position_mark_array is a 2D-int array contains positions of elements in mark_array
            array[j] = -array[j]
        position_mark_array[i] = array #position_mark_array now contains clauses in DNF form (Disjunctive Normal Form) - variables in a clause are connected via Conjunction
     
    return position_mark_array
        

#return 2D-int array: Each array in CNF_array is a clause in CNF form (Conjunctive Normal Form) - variables in a clause are connected via Disjunction
def convert_DNF_to_CNF(current_DNF_clause: int, DNF_array):
    CNF_array = []
  
    if current_DNF_clause < len(DNF_array) - 1:
        for variable in DNF_array[current_DNF_clause]:
            CNF_array = convert_DNF_to_CNF(current_DNF_clause= current_DNF_clause+ 1, DNF_array= DNF_array)
            for clause in CNF_array:
                clause.append(variable)
    else:
        for variable in DNF_array[current_DNF_clause]:
            clause = [variable]
            CNF_array.append(clause)
            
    return CNF_array

#void function: Generate CNF clauses and add to global variable CNFs
def generate_cnf(mark_array, num_trap_on_map: int):
    global CNFs
    DNF = generate_DNF(mark_array= mark_array, num_trap_on_map= num_trap_on_map)
    CNF = convert_DNF_to_CNF(DNF_array= DNF, current_DNF_clause= 0)
    for clause in CNF:
        CNFs.append(clause)
    
    
#void function: Look at cells surround the cell with a number in it to generate CNF clauses      
def look_around(h : int, w : int):
    global height
    global width
    global map

    #Cells surround the cell with a number in it
    mark_array = []

    for y in range(-1, 2):
        if y + h < 0 or y + h >= height:
            continue
        for x in range(-1, 2):
            if x + w < 0 or x + w >= width:
                continue
            #Found cells with marks (logical variables)
            if int(map[y + h][x + w]) >= 10:
                mark_array.append( int(map[y + h][x + w]) )
                
    #Number of variables around is less than number of traps surround -> Error
    if len(mark_array) < int(map[h][w]):
        return
    
    generate_cnf(mark_array= mark_array, num_trap_on_map= int(map[h][w]))
    
    
#void function
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
print(CNFs)
