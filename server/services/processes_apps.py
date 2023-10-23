import psutil # remember to install psutil (pip/pip3 install psutil)
import os
import re

def determine_os():
    myOS = "linux"
    if os.name == 'nt':
        myOS = "windows"
    return myOS

# list processes
def processes():
    processes_id = []
    processes_name = []
    
    for process in psutil.process_iter():
        try:
            id = process.pid
            name = process.name()

            processes_id.append(id)
            processes_name.append(name)

        except (psutil.NoSuchProcess, psutil.AccessDenied, psutil.ZombieProcess):
            pass

    result = ''
    for item in [(id, name) for id, name in zip(processes_id, processes_name)]:
        result += ' - '.join(str(i) for i in item) + '\n'

    return result

# list apps
def apps():
    names = []
    ids = []
    
    cmd = 'powershell "gps | where {$_.mainWindowTitle} | select Description, ID"'
    proccess = os.popen(cmd).read().split("\n")
    
    infos = []
    for line in proccess:
        if line.isspace():
            continue
        infos.append(line)
    infos = infos[3:]
    
    for info in infos:
        try:
            subInfo = str(info).split(' ')
            if len(subInfo) < 2 or subInfo[0] == '' or subInfo[0] == ' ':
                continue
            name = subInfo[0]
            ID = 0
            # iteration
            cur = len(subInfo) - 1
            for i in range(cur, -1, -1):
                if len(subInfo[i]) != 0:
                    ID = subInfo[i]
                    cur = i
                    break
            for i in range(1, cur, 1):
                if len(subInfo[i]) != 0:
                    name += ' ' + subInfo[i]
            names.append(name)
            ids.append(ID)
            
        except:
            pass
    result = ''
    for item in [(id, name) for id, name in zip(ids, names)]:
        result += ' - '.join(str(i) for i in item) + '\n'

    return result

def turn_on(name):
    command = ""
    if determine_os() == "linux":
        command = name
    else:
        command = "start " + name
        
    if os.system(command) != 0:
        return (f"Error: Failed to start {name}.")
    else:
        return (f"{name} has started successfully.")
    
def turn_off(pid):
    command = ""
    # check OS to specify the command
    if determine_os() == "linux":
        command = "kill -9 " + str(pid)
    else:
        command = "taskkill /F /PID " + str(pid)

    # turn off the process

    if os.system(command) != 0:
        return (f"Error: Failed to kill process with PID {pid}.")
    else:
        return (f"Process with PID {pid} was killed.")
    
def execute_msg(msg):
    result = ""
    if "Application" in msg:
        result = "List of application\n" + "Id - Name\n" + apps()
    elif "Process" in msg:
        result = "List of process\n" + "Id - Name\n" + processes()
    elif "Start" in msg:
        try:
            name = re.search(r'Start\[name:(.*)\]', msg).group(1)
        except:
            print("Wrong format.")
        result = turn_on(name)
    elif "Kill" in msg:
        try:
            id = re.search(r'id:(\d+)', msg).group(1)
        except:
            print("Wrong format.")
        result = turn_off(id)
    return result + '\n'