#!/usr/bin/python3

import sys
import os
import inspect

arg1 = sys.argv[1] if len(sys.argv) > 1 else "";
arg2 = sys.argv[2] if len(sys.argv) > 2 else "";
workspace = os.path.dirname( os.path.abspath( inspect.stack()[0][1] ) );
dbname = "waypoint.db"

if not os.path.exists( workspace + "/" + dbname ):
    with open( workspace + "/" + dbname, "w" ): pass

def change_directory( dest ):
    import fcntl, termios, pipes
    
    os.system("stty -echo")
    try:
        for c in ("cd " + pipes.quote(dest) + "\n"):
            fcntl.ioctl(1, termios.TIOCSTI, c)
    except:
        pass
    os.system("stty echo")

def read_waypoint_database():
    dbfile = open( workspace + "/" + dbname, "r" )
    content = dbfile.read()
    
    if content == "":
        return {}
    
    dbfile.close()
    database = {}
    
    for line in content.split("\n"):
        if line != "":
            entry = line.split(" => ", 1)
            database[entry[0]] = entry[1]
    
    return database

def save_waypoint_database( database ):
    dbfile = open( workspace + "/" + dbname, "w" )

    for waypoint in database.keys():
        dbfile.write( waypoint + " => " + database[ waypoint ] + "\n" )

    dbfile.close()

def goto_waypoint( waypoint ):
    db = read_waypoint_database()
    if waypoint in db:
        change_directory( db[waypoint] )
    else:
        print("No such waypoint!");

def list_waypoints():
    db = read_waypoint_database()
    for wp in db.keys():
        print( wp )

def remove_waypoint( waypoint ):
    db = read_waypoint_database()
    if waypoint in db:
        db.pop( waypoint, None )
        save_waypoint_database( db )
    else:
        print("No such waypoint!");

def peek_waypoint( waypoint ):
    db = read_waypoint_database()
    if waypoint in db:
        print( db[waypoint] );
    else:
        print("No such waypoint!");

def set_waypoint( waypoint ):
    if waypoint == "":
        print("Invalid argument!");
        return

    if waypoint in ["go", "set", "rem", "peek", "ls", "help"]:
        print("Unallowed waypoint name!");
        print("Set anyway? The shorthand syntax will be unavailable. y/n");
        if input() != 'y':
            return
    
    db = read_waypoint_database()
    db[waypoint] = os.getcwd()
    save_waypoint_database( db )

if arg1 == "":
    print( "Confused? Run with 'help'." );
    exit(0)

if arg1 == "go":
    goto_waypoint(arg2)
    exit(0)

if arg1 == "set":
    set_waypoint(arg2)
    exit(0)

if arg1 == "rem":
    remove_waypoint(arg2)
    exit(0)

if arg1 == "peek":
    peek_waypoint(arg2)
    exit(0)

if arg1 == "ls":
    list_waypoints()
    exit(0)

if arg1 == "help":
    print( "Directory waypoint utility v1.1" )
    print( "  go [waypoint]   - go to waypoint" )
    print( "  set [waypoint]  - set waypoint" )
    print( "  rem [waypoint]  - remove waypoint" )
    print( "  peek [waypoint] - print waypoint's target" )
    print( "  ls              - list all waypoints" )
    print( "  help            - show this help page" )
    print( "  [waypoint]      - go to waypoint" )
    exit(0)

goto_waypoint(arg1)
exit(0)

