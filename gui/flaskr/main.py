# export FLASK_APP=main.py
# flask run

#--------------------------------------- Imports -----------------------------------------#
import os
import sys
import socket
import time
import threading
from flask import Flask, url_for, redirect, jsonify, request, render_template

#-----------------------------------------------------------------------------------------#
#----------------------------------- Variables -------------------------------------------#

data = {"vitesseReel" : ['--', '--', '--', '--', '--', '--'],
        "vitesseDesiree" : ['--', '--', '--', '--', '--', '--'],
        "positions" : ['--', '--', '--', '--', '--', '--'],
        "messages" : ''
        }
#server = ("127.0.0.1", 4000)
server = ("127.0.0.1",4244)
sock = None
connFlag = True
vitesseReel = ['--', '--', '--', '--', '--', '--']
vitesseDesiree = ['--', '--', '--', '--', '--', '--']
position = ['--', '--', '--', '--', '--', '--']
messages = ''

#-----------------------------------------------------------------------------------------#

#-----------------------------------------------------------------------------------------#
os.system('cls||clear')

app = Flask(__name__)

@app.route('/')
def start():
    return render_template("painel.html")

@app.route('/getVitesse', methods=['POST', 'GET'])
def getVitesse():
    global messages, sock, connFlag

    if request.method == 'POST':
        vitesseConsigne = request.form['vitesseConsigne']
        train_Leader = request.form['train_leader']
    
    msg = "10:-1:" + str(train_Leader) + ":" + str(vitesseConsigne) + "$$"
    print(msg)
    messages += '<p><i> Train : ' + str(train_Leader) + ' -> Vitesse Consgine = ' + str(vitesseConsigne) + '</i></p>'
    
    if not connFlag:
        sock.send(msg.encode())

    return redirect(url_for('start'))

@app.route('/getData', methods=['POST', 'GET'])
def getData():
    global data, sock
    sock.send("1:-1:-1:-1$$".encode())
    return jsonify(data)

if __name__ == "__main__":
    threading.Thread(target=lambda : app.run(debug=False, host='127.0.0.1', port=5000, use_reloader=False)).start()


#-----------------------------------------------------------------------------------------#

#-----------------------------------------------------------------------------------------#

def definition():
    global server, messages, sock, data, connFlag

    try:
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        messages +="<p><i> Socket successfully created </i></p>"
        data["messages"] = messages
        print("Socket successfully created...\n")
    except socket.error as err:
        messages += ("<p><i> Socket creation failed, error : " + str(err) + "</i></p>")
        data["messages"] = messages

    print("Connecting....\n")
    messages += "<p><i> Searching Server... </i></p>"
    while(connFlag):
        try:
            sock.connect(server)
            messages += "<p><i> Connection OK! </i></p>"
            data["messages"] = messages
            print("Connection OK...\n")
            connFlag = False
            connexion()
        except socket.error as err:
            #messages += ("<p><i> Socket connection failed, error: " + str(err) + '</i></p>')
            data["messages"] = messages

def connexion():
    global messages, sock

    sock.send("1:-1:-1:-1$$".encode())

    while True:
        rec = None
        rec = sock.recv(1024).decode("utf-8")
        print(rec)
        rec2 = ''

        for k in range(len(rec)):
            if not rec[k] == "\x00":
                rec2 += str(rec[k])

        if not rec:
            print("C server closed the socket...\n")
            sock.close()
            sys.exit(1)
        else: 
            separateData(rec2)
            sock.send("1:-1:-1:-1$$".encode())
            #sock.send(rec.encode())


def separateData(rec):
    global position, vitesseReel, vitesseDesiree, data
    matrix = []
    text = rec.split("$$")
    text.pop()

    for i in range(len(text)):
        if(len(text[i].split(':')) == 4):
            matrix.append(text[i].split(':'))    

    for i in range(len(matrix)):
        position[int(matrix[i][1])-1] = matrix[i][2]

        if(matrix[i][0] == "6"):
            vitesseDesiree[int(matrix[i][1])-1] = matrix[i][3]
        elif (matrix[i][0] == "4"):
            vitesseReel[int(matrix[i][1])-1] = matrix[i][3]

    data = {"vitesseReel" : vitesseReel,
            "vitesseDesiree" : vitesseDesiree,
            "positions" : position,
            "messages" : messages
            }

threading.Thread(target=lambda : definition()).start()