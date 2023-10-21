import psutil
import os

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
    
    cmd = 'powershell "gps | where {$_.mainWindowTitle} | select Description, ID'
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
            
        except:
            pass