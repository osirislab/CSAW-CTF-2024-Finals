import requests

def main():
    '''
    This script shows how to communicate with the authenticated encryption
    oracle which is accessible via a HTTP POST request. 
    '''

    url = "http://127.0.0.1/enc/oracle"
    data = {"plaintext": "test"}
    req = requests.post(url=url, json=data)
    resp=req.json()
    print(resp) #Should print: {'ciphertext': 'ûnZ\x96:§®w¡{[\x03!\x03Zó}ñùÎ\x16w(\x9e\x8cváÙ\x9fÑüg\x96ñ2]\x0e\x84²\x1cS\x81O9Uú4¤'}
if __name__ == "__main__":
    main()