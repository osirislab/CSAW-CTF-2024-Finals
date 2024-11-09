import requests
import json

from flask import Flask, request, Response
from tools import *

app = Flask(__name__)

@app.route('/enc/oracle', methods=['POST'])
def auth_encryption_oracle():
    client_message = request.get_json()
    plaintext = client_message['plaintext']
    print(plaintext)
    print(type(plaintext))
    if type(plaintext) != str:
        resp = {"error": "Provided plaintext must be a string"}
        return Response(
        response=json.dumps(resp),
        status=200,
        mimetype="application/json"
    )
    else:
        message = auth_encrypt_oracle2(plaintext).decode('latin1')
        resp = {"ciphertext": message}
        return Response(
            response=json.dumps(resp),
            status=200,
            mimetype="application/json"
        )

if __name__ == "__main__":
    app.run(host="0.0.0.0", debug=False, port=80)
    

    
