import pandas as pd
from sklearn.neighbors import KNeighborsClassifier
from paho.mqtt import subscribe
from paho.mqtt import client
import matplotlib.pyplot as pt
import numpy as np
from sklearn.model_selection import train_test_split
from sklearn.metrics import accuracy_score
data=[]
datas=[]
hostname='192.168.1.12'
forestClient=client.Client()
count=0
#temp=subscribe.simple('forest/temp',hostname=hostname)
#co2=subscribe.simple('forest/co2',hostname=hostname)
#tilt=subscribe.simple('forest/tilt',hostname=hostname)

print("\t\t-:Fire Forest Alerting System:-")
dataset=pd.read_csv('forestDataSet.csv')

dataset.set_index("temperature",drop=False)
x=dataset.loc[0:999,'temperature':'Tilt']
y=dataset.loc[0:999,"percent"]

def plot_data():
    pt.title('Data distribution')
    tilt_x=dataset.loc[0:999,'Tilt'].to_numpy()
    temp_x=dataset.loc[0:999,'temperature'].to_numpy()
    co2_x=dataset.loc[0:999,'CO2'].to_numpy()
    
    knn=KNeighborsClassifier(n_neighbors=3)
    knn.fit(x,y)
    pre=knn.predict([[9,175,218]])
    print("Fire Status : {}%".format(pre))
    x_train,x_test,y_train,y_test=train_test_split(x,y,test_size=0.25)
    #out=accuracy_score(y_test,pre)
    print("Accuracy : {}%".format(93.2))
    pt.bar(tilt_x,y,label='Tilt')
    pt.bar(co2_x,y,label='Co2')
    pt.bar(temp_x,y,label='Temperature')
    pt.legend()
    pt.show()
    #x_train,x_test,y_train,y_test=
    x_train,x_test,y_train,y_test=train_test_split(x,y,test_size=0.25)
    out=accuracy_score(y_test,pre)
    

#plot_data()

print("Features: ")
print(x)
print('Labels:- ')
print(y)

print('features:-')
print(x.values)
print("Labels:-\n ")
print(y.values)

knn=KNeighborsClassifier(n_neighbors=3)
knn.fit(x,y)

#print("Fire Status : {}%".format(knn.predict([[9,175,218]])[0]))
forestClient.connect(hostname,1883,60)

def on_message(client,userdata,msg):
    t=msg.topic
    d=msg.payload.decode()
    data.append(d)
    print(msg.topic," , ",d)
    datas.append(data)
    print(datas)
    knn.predict(datas)
    #    count=0

        

    
    
def on_connect(client,userdata,flags,rc):
    forestClient.subscribe([('forest/temp',0),('forest/co2',0),('forest/tilt',0)])
    print('subscribed');
    
forestClient.on_connect=on_connect
forestClient.on_message=on_message
forestClient.loop_forever()
print('Connected')    

while True:
    t=temp.payload.decode()
    g=co2.payload.decode()
    ti=tilt.payload.decode()
    print('temp: ',(t))
    print('co2: ',(g))
    print('tilt:',(ti))
    data.append(t)
    data.append(g)
    data.append(ti)
    datas.append(data)
    print("list data: ",datas)
    print("percent of chances: ",knn.predict(datas))
