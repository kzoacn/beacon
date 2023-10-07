import requests
import json
import time
url = 'https://randomnessbeacon.com/index/random/find'
headers = {'Authorization': 'Bearer ' + '21e1e27c029cd1e0adc2cf5080229c1a070115822310d79ea48b93cbbc613e96'}


randomness=[]

start=650000
for i in range(start,start+1500):
    time.sleep(1)
    print(i)
    data={'external_id' : '1', 'pulse_index' : i}
    response = requests.post(url, data=data, headers=headers)
    js = json.loads(response.text)
    r=js['data']['output_value']
    randomness.append(r)

with open('random', 'w') as f:
    for item in randomness:
        f.write("%s" % item)